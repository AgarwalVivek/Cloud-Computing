#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<malloc.h>

#define block_size 65536 
void *dispatcher(void *socket_desc)
{

  int sock = *(int*)socket_desc;
  int read_size;
  char *mem_dst=(char *) malloc(block_size);

  while( (read_size = recv(sock , mem_dst , block_size , 0)) > 0 )
  {

      write(sock , mem_dst , strlen(mem_dst));
  }

 
  if(read_size == 0)
  {
      puts("Client disconnected");
      fflush(stdout);
  }
  else if(read_size == -1)
  {
      perror("recv failed");
    }

    free(socket_desc);
    free(mem_dst);	
    return 0;
}


int socket_init() {

	return (socket(AF_INET, SOCK_STREAM, 0));
}

void sockaddr_init(struct sockaddr_in *serv) {
	serv->sin_family = AF_INET;
	serv->sin_addr.s_addr = INADDR_ANY;
	serv->sin_port = htons(8888);
}
int main(int argc , char *argv[])
{
    int client_sock , socket_desc, client_socklen , *new_socket_t, read_size;
    struct sockaddr_in server , client;
    //char *mem_dst=(char *) malloc(block_size);
        	
    socket_desc = socket_init();
    client_socklen = sizeof(struct sockaddr_in);
	if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");	
    sockaddr_init(&server);	

    //server.sin_family = AF_INET;
    //server.sin_addr.s_addr = INADDR_ANY;
    //server.sin_port = htons( 8888 );

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {

              perror("Couldnt bind");
              return 1;
          }
         

          listen(socket_desc , 3);

          puts("Accepting connections::");
          //c = sizeof(struct sockaddr_in);

          while (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&client_socklen) )
          {
              puts("Connection accepted");

              pthread_t sniffer_thread;
	      new_socket_t = malloc(1);
              *new_socket_t = client_sock;

              if( pthread_create( &sniffer_thread , NULL ,  dispatcher , (void*) new_socket_t) < 0)
              {
                  perror("problem with thread creation");
                  return 1;
              }

              puts("thread created");
          }

          if (client_sock < 0)
          {
              perror("unable to accept clientconnect");
              return 1;
  }

  return 0;
}

