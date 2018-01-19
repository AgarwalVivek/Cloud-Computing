#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>

#define block_size 65536 
int op_count = 128;

void *dispatcher(void *);
void operation_time(double totalTime);
int sock,charcount,len,num_threads;
char *src;
char *dest;

