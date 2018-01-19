#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>
#define NUM_INSTR 10000000
unsigned long long total_instr;
float floatc;
int intc;
int m = 1;
void *func_flops() {
	float floatc = 0.98, counter = 0;
	while(counter++<total_instr) floatc *=2.9*4.3+4.54*9.32*8.23*3.96*9.32*7.33*4.9*3.23*23.3*3.1*9.1*2.3*4.1*5.1*0.2*32.9*92.3*33.1*88.3*92.2*67.2*29.2*24.2*22.3*12.23*25.2*12.4*2.8; 
	return 0;
}
void *func_iops() {
	 intc = 0;
	while(intc<total_instr){
	m = m+1+2+3+4+5+6+7+8+9+10+11+12+13+15+156+177+152+122+111+444+555+666+999;
	intc = intc + 1;
	}
	return 0;
}
double dispatcher_flops(int num_threads) {

	int iterator = 0;
	pthread_t th[8];
	double cpu_time = 0.0, flops = 0.0;
	struct timeval startTime, endTime;
	unsigned  long start_timer, end_timer;
	gettimeofday(&startTime, NULL);
	while(iterator<num_threads) {
		pthread_create(&th[iterator], NULL, func_flops, NULL);
		iterator++;
	}
        iterator = 0;	
	while(iterator<num_threads){
		pthread_join(th[iterator], NULL);
		iterator++;
	}

	gettimeofday(&endTime, NULL);
	start_timer = (unsigned long long)startTime.tv_sec * 1000000 + startTime.tv_usec;
        end_timer = (unsigned long long)endTime.tv_sec * 1000000 + endTime.tv_usec;

	cpu_time=((double)(end_timer - start_timer))/ 1000;
	flops=(double)(total_instr * num_threads*10000*4/(cpu_time*1000000000.00));
	return flops;
}

double dispatcher_iops(int num_threads) {
	
	int iterator = 0;
	pthread_t th[8];
	double cpu_time = 0.0, iops = 0.0;
	struct timeval startTime, endTime;
        unsigned  long start_timer, end_timer;
        gettimeofday(&startTime, NULL);

	while(iterator<num_threads) {
		pthread_create(&th[iterator], NULL, func_iops, NULL);
                //pthread_join(th[iterator], NULL);
                iterator++;
	}
	iterator = 0;	
	while (iterator<num_threads){
		pthread_join(th[iterator], NULL);
		iterator++;
	}
	
	gettimeofday(&endTime, NULL);
        start_timer = (unsigned long long)startTime.tv_sec * 1000000 + startTime.tv_usec;
        end_timer = (unsigned long long)endTime.tv_sec * 1000000 + endTime.tv_usec;

	cpu_time =((double)(end_timer - start_timer))/ 1000;
        iops=(double)(total_instr * num_threads*10000*9/(cpu_time*1000000000.00));
        return iops;
}
int main(int argc, char **argv) {

	int test_case = 0;
	double flops_duration = 0.0, iops_duration = 0.0;
	
	if (argc<=1) {
		printf("No. of threads:\tNULL\n");
		return 0;	
	}
	//test_case is number of threads

	test_case = atoi(*(argv+1));
	printf("%d", test_case);
	total_instr = NUM_INSTR/test_case;
	flops_duration = dispatcher_flops(test_case);
	iops_duration = dispatcher_iops(test_case);
	printf("Flops/%d:%f\t iops/%d:%f\n",test_case, flops_duration, test_case, iops_duration);
	floatc+= intc;	
	return 0;
}


