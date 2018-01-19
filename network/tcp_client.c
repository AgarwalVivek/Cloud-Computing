#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>
#include "tcp_client.h"

int socket_init(){
	return(socket(AF_INET, SOCK_STREAM, 0));
}
int main(int argc , char *argv[])
{
    int i;
    struct sockaddr_in server;
    src = (char *) malloc(block_size);
    dest = (char *) malloc(block_size);
    struct timeval startTime, endTime;
    float totalTime;
    unsigned long start_timer, end_timer;

    scanf("%d", &num_threads);
    pthread_t thread_arr[num_threads];
    op_count = op_count/num_threads;
    printf("op_count:%d\n", op_count);
    sock = socket_init();
    if (sock == -1)
    {
        printf("Could not create socket");
      }
      puts("Socket created");
      server.sin_addr.s_addr = inet_addr("127.0.0.1");
      server.sin_family = AF_INET;
      server.sin_port = htons( 8888 );
      
      memset(src,'1',block_size);
      if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
      {
          perror("connect failed. Error");
          return 1;
      }

      puts("Connected\n");
      int true = 1;
      if(true)
      {
	  gettimeofday(&startTime, NULL);
          for(i=0;i<num_threads;i++)
           {
                  pthread_create(&thread_arr[i],NULL,dispatcher,NULL);
           }
          for(i=0;i<num_threads;i++)
           {
                  pthread_join(thread_arr[i],NULL);
           }
           gettimeofday(&endTime, NULL);
	   start_timer = (unsigned long long)startTime.tv_sec * 1000000 + startTime.tv_usec;
	   end_timer = (unsigned long long)endTime.tv_sec * 1000000 + endTime.tv_usec;
           totalTime = ((double) (end_timer - start_timer))/1000;
           printf("CPU TIME:%f", totalTime);
	   operation_time(totalTime);
	       
	}
       close(sock);
   }

   void *dispatcher(void *socket_desc)
   {
	
       int i;
       for(i=0;i<op_count;i++)
       {
       if( send(sock , src , strlen(src) , 0) < 0)
           {
               puts("Send failed");

           }

           if( recv(sock , dest , block_size , 0) < 0)
           {
               puts("recv failed");
           }

           puts("Response received from server..");

       }
	printf("op_count:%d\n", op_count);	

       return 0;
   }

   void operation_time(double time)
   {
    
     printf("time taken:%f\n",time);
     double latency = (double)(time)/op_count;
     double bandwidth;
     bandwidth = (double)((8388608)/(time*1024));
     printf("latency in milliseconds:%f\n",latency);
     printf("throughput in Mb/sec:%f\n",bandwidth);

   }

