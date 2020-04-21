#include "stdio.h"
#include "string.h"
#include "motor/motor.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
char buffer[512];
motor motor;
static int server_sockfd;
static int client_sockfd;

void Stop(int signo) 

{

    printf("oops! stop!!!\n");
	close(client_sockfd);
	close(server_sockfd);
    _exit(0);

}

int main(int argc,char *argv[])
{
	struct  sockaddr_in local_addr;
	struct  sockaddr_in remote_addr;
	int heartbeat_cnt = 0;
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;
	local_addr.sin_port=htons(8000);
	signal(SIGINT, Stop); 
	if((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error");
		return -1;
	}
	
	socklen_t reuse = 1;
	if(setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
       {
                perror("setsockopet error\n");
                return -1;
       }

	if(bind(server_sockfd,(struct sockaddr *)&local_addr,sizeof(struct sockaddr))<0){		
		perror("bind error");		
		return -1;	
	}	
	if(listen(server_sockfd,5)<0){		
		perror("listen error");		
		return -1;	
	};
    
	socklen_t sin_size=sizeof(struct sockaddr_in);

	if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0){		
		perror("accept error");		
		return 1;	
	}	
	//int flags = fcntl(server_sockfd, F_GETFL, 0); 
    //fcntl(server_sockfd, F_SETFL, flags | O_NONBLOCK);
	printf("accept client \n");	
	//send(client_sockfd,"Welcome to my server\n",21,0);

	motor.start();

	for(;;){
		memset(buffer, 0,sizeof(buffer));
		int len=recv(client_sockfd,buffer,sizeof(buffer),0);
		heartbeat_cnt++;
		if(len > 0){
			heartbeat_cnt = 0;
			if(strncmp(buffer, "turn_left", strlen("turn_left")) == 0){
				motor.turn_left(30);
				printf("turn_left \n");	
			}
			else if(strncmp(buffer, "trun_left_done", strlen("trun_left_done")) == 0){
				motor.trun_left_done();
				printf("trun_left_done \n");	
			}
			else if(strncmp(buffer, "turn_right", strlen("turn_left")) == 0){
				motor.turn_right(30);
				printf("turn_right \n");	
			}
			else if(strncmp(buffer, "trun_right_done", strlen("trun_right_done")) == 0){
				motor.trun_right_done();
				printf("trun_right_done \n");	
			}
			else if(strncmp(buffer, "direct_line", strlen("direct_line")) == 0){
				motor.turn_over();
				printf("direct_line \n");	
			}
			else if(strncmp(buffer, "speed1", strlen("speed1")) == 0){
				motor.direct_line(50);
				printf("speed1 \n");	
			}
			else if(strncmp(buffer, "speed2", strlen("speed2")) == 0){
				motor.direct_line(75);
				printf("speed2 \n");	
			}
			else if(strncmp(buffer, "speed3", strlen("speed3")) == 0){
				motor.direct_line(100);
				printf("speed3 \n");	
			}
			else if(strncmp(buffer, "speed_backoff", strlen("speed_backoff")) == 0){
				motor.backoff();
				printf("speed_backoff \n");	
			}
			else if(strncmp(buffer, "stop", strlen("stop")) == 0){
				motor.direct_line(0);
				printf("stop \n");	
			}
			else if(strncmp(buffer, "heartbeat", strlen("heartbeat")) == 0){
				printf("heartbeat \n");	
			}
		}
		else if(len < 0){
			perror("write error");
			if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0){		
				perror("accept error");		
				return 1;	
			}				
		}	
		if(heartbeat_cnt > 5){
			motor.direct_line(0);
			perror("reconnect");
			if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0){		
				perror("accept error");		
				return 1;	
			}				
		}
		printf("loop %s\n", buffer);	
		usleep(1000);
	}


	return 0;
}