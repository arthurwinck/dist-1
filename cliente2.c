#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int op;
    char body[30];
    int line;
} request_t;

int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  request_t request, response;
  int result;
  char ch = 'D';

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //address.sin_addr.s_addr = inet_addr("192.168.0.15");
  //address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(9734);

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result != 0) {
		perror("oops: client2");
		exit(1);
  }
  char* inter = "dale inter";
  strcpy(&request.body[0], inter);
  request.op = 0;
  request.line = 0;
	write(sockfd, &request, sizeof(request_t));
	sleep(2);
	read(sockfd, &response, sizeof(request_t));
	printf("op from server: %d\n", response.op);
  printf("body from server: %s\n", response.body);
	close(sockfd);
	exit(0);
}
