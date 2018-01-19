#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Set memory space
unsigned long long op_count = 1024*1024*1024*10;
#define MAX_SIZE 10737418240//1024*1024*1024*10
unsigned long long  number_of_iterations;
int block_size;
unsigned long long starts[8];
FILE *fp;
char read_c[100];
void *Rand_readWrite_Access(unsigned long long start_index)
{

	unsigned long long i, ran;
	int fd = fileno(fp);
        unsigned long long end_index = start_index + op_count-1;
	        fseek(fp, start_index, SEEK_SET);
        while(start_index<end_index) {
                ran = rand()%(op_count/block_size);
                fread(read_c, block_size*sizeof(char),1, fp);
                fwrite(read_c,block_size*sizeof(char),1, fp);
		fflush(fp);
		fsync(fd);
		start_index = start_index+block_size;
                //printf("start_index:%llu\n", start_index);
                fseek(fp, start_index+ran, SEEK_SET);
        }	
}

void *Seq_readWrite_Access(unsigned long long start_index)
{

  unsigned long long end_index = start_index + op_count-1;
	//printf("end_index:%llu\n", end_index);
	int fd  = fileno(fp);
	fseek(fp, start_index, SEEK_SET);
	while(start_index<end_index) {
		fread(read_c, block_size*sizeof(char),1, fp);
	    	fwrite(read_c, block_size*sizeof(char),1,fp);
		fflush(fp);
		fsync(fd);
		start_index = start_index+block_size;
		//printf("start_index:%llu\n", start_index);
		fseek(fp, start_index, SEEK_SET);

	}

}

void *seq_Access(unsigned long long start_index)
{
	unsigned long long end_index = start_index + op_count-1;
	fseek(fp, start_index, SEEK_SET);
	while(start_index<end_index) {
		fread(read_c, block_size*sizeof(char),1, fp);
		start_index = start_index+block_size;
		fseek(fp, start_index, SEEK_SET);

	}

}


void *ran_Access(unsigned long long start_index)
{
	unsigned long  long i, ran;
	unsigned long long end_index = start_index + op_count-1;
        fseek(fp, start_index, SEEK_SET);
        while(start_index<end_index) {
                ran = rand()%(op_count/block_size);
		fread(read_c, block_size*sizeof(char),1, fp);
                start_index = start_index+block_size;
                fseek(fp, start_index+ran, SEEK_SET);
	}

}

int main(int argc, char* argv[]){

	int access, size, num_threads;
	printf("Access\n  1Random\n 2-Sequential\n 3-Seq_ReadWrite\n 4-Ran_ReadWrite\n");
	fflush(stdout);
  	scanf("%d",&access);
	printf("\n Block size:(Select 1 for 8B=8\n 2 for 8KB=8*1024\n 3 for 8MB=8*1024*1024\n 4 for 80MB=80*1024*1024:\n");
  	fflush(stdout);
 	scanf("%d",&size);
	printf("Enter thread count\n");
   	fflush(stdout);
  	scanf("%d",&num_threads);

	int index = 0;

	unsigned long long start = 0;
        unsigned long long offset = 0;
        offset = MAX_SIZE/num_threads;
        while(index<num_threads) {
        	starts[index] =  start+(offset*index);
        	index++;
        }

	fp = fopen("file.txt","w+");
	int fd = fileno(fp);


	fseek(fp, MAX_SIZE-1, SEEK_SET);
	fputc('\0', fp);
	fseek(fp, 0, SEEK_SET);
	float execution_timer;
        unsigned  long start_timer, end_timer;


	pthread_t thread[num_threads];
	struct timeval startTime, endTime;
	float totalTime;




	// Get block size
	switch(size) {
		case 1: block_size = 8;
			break;
		case 2: block_size = 8*1024;
			break;
		case 3: block_size  = 8 * 1024 * 1024;
			break;
		case 4: block_size  = 80 * 1024 *  1024;
			break;
		default: printf("Invalid size\n");
	}
	op_count = MAX_SIZE/(num_threads?num_threads:1);
	number_of_iterations = op_count/block_size;
	// Start time
	gettimeofday(&startTime, NULL);

	// Choose which method to run
	int i;
	if(access == 1)
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, ran_Access, (void*)(int)starts[i]);
		}
	else if(access == 2)
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, seq_Access, (void*)(int)starts[i]);
		}
	else if(access == 3)
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, Seq_readWrite_Access, (void*)(int)starts[i]);
		}
	else if(access == 4)
		for (i = 0; i < num_threads; i++){
			pthread_create(&thread[i], NULL, Rand_readWrite_Access, (void *)(int)starts[i]);
		}
	else
	{
		printf("Error\n");
		exit(0);
	}

	for (i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], NULL);
	}

	gettimeofday(&endTime, NULL);
	fclose(fp);
	remove("file.txt");


	start_timer = (unsigned long long)startTime.tv_sec * 1000000 + startTime.tv_usec;
        end_timer = (unsigned long long)endTime.tv_sec * 1000000 + endTime.tv_usec;

        execution_timer = (float)(end_timer -  start_timer) / 1000;

	float throughPut = (float)MAX_SIZE / execution_timer * 1000 / (1024*1024);
	float latency = (float)(execution_timer*1000)/(op_count);

	printf("Run time: %.3f ms, ThroughPut: %.3f MB/sec, Latency: %f ms/bit\n", execution_timer, throughPut,latency);
	return 0;

}
