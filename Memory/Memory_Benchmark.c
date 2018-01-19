#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Set memory space
int op_count = 1024*1024*1024;
int block_size;
#define MAX_SIZE 1024*1024*1024
int number_of_iterations;
void *Random_rw()
{

        int i, offset;
        char *mem_src = (char*)malloc(sizeof(char) * op_count);
        char *mem_dst = (char*)malloc(sizeof(char) * op_count);
        for(i = 0; i < number_of_iterations; i++)
        {       offset = rand()%(op_count/block_size);
                memcpy(mem_dst+offset, mem_src + offset, block_size);
        }
        free(mem_src);
        free(mem_dst);

}

void *Sequential_rw()
{
	
	int i, offset;
	char *mem_src = (char*)malloc(sizeof(char) * op_count);
	char *mem_dst = (char*)malloc(sizeof(char) * op_count);

	for(i = 0; i < number_of_iterations; i++)
	{	offset = i*block_size;
		memcpy(mem_dst+offset, mem_src + offset, block_size);
	}
	free(mem_src);
	free(mem_dst);	
	
}

void *Sequential_Write()
{
	
	int i;
	char *mem_dst = (char*)malloc(sizeof(char) * op_count);
	for(i = 0; i < number_of_iterations; i++)
	{		
		memset(mem_dst, 1, block_size);
	}
	free(mem_dst);	
	
}

 
void *Random_Write()
{
	int i, offset;
	
	char *mem_dst = (char*)malloc(sizeof(char) * op_count);
	for(i = 0; i < number_of_iterations; i++)
	{
		offset = rand()%(op_count/block_size);
		memset((mem_dst+offset), 1, block_size);	
	}
	free(mem_dst);
}

void decide_block_size(int size) {

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
	
	float execution_timer, throughput, latency;
        unsigned  long start_timer, end_timer;


	pthread_t thread[num_threads];
	struct timeval startTime, endTime;
	float totalTime;
	decide_block_size(size);	
	
	

	printf("%d\n",block_size);
	op_count = MAX_SIZE/(num_threads?num_threads:1);
	number_of_iterations = op_count/block_size;
	printf("MEMORY SIZE:%d\n", op_count);
	// Start time
	gettimeofday(&startTime, NULL);

	// Choose which method to run
	int i;
	switch(access){
	
	case 1:
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, Random_Write, NULL);
		}	
		break;
	case 2:
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, Sequential_Write, NULL);
		}
		break;

	case 3:
		for (i = 0; i < num_threads; i++)
		{
			pthread_create(&thread[i], NULL, Sequential_rw,NULL);
		}	
		break;
	case 4: 
		for (i = 0; i < num_threads; i++){
			pthread_create(&thread[i], NULL, Random_rw, NULL);
		}
		break;
	default:	printf("No such operation allowed\n");
			exit(0);
	}	

	for (i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], NULL);
	}
	
	gettimeofday(&endTime, NULL);
	


	start_timer = (startTime.tv_sec * 1000000 + startTime.tv_usec);
        end_timer = (endTime.tv_sec * 1000000 + endTime.tv_usec);

        execution_timer = (float)(end_timer -  start_timer) / 1000;

	throughput = (float)MAX_SIZE / execution_timer * 1000 / (1024*1024);
	latency = (float)(execution_timer*1000)/(op_count);

	printf("Run time: %.5f ms, ThroughPut: %.5f MB/sec, Latency: %f ms/bit\n", execution_timer, throughput,latency);
	return 0;

}
