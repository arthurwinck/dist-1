// gcc client_text_editor.c -o client_text_editor

// Na mensagem usar _ ao invés de espaço

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

int main(int argc, char** argv)
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  request_t request, response;
  int result;
  char* req_message;
  int req_line;

  if ((argc < 3) || (argc > 4)) {
    printf("./programa OP LINHA MENSAGEM\n");
    printf("OP: 0: leitura, 1: escrita\n");
    printf("LINHA do bloco a ser lida (-1 se escrita)\n");
    printf("MENSAGEM a ser escrita (nula se leitura)\n");
    exit(1);
  }
  int op = atoi(argv[1]);


  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //address.sin_addr.s_addr = inet_addr("192.168.0.15");
  //address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(9734);

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result != 0) {
		printf("Erro ao criar conexão\n");
		exit(1);
  }

  if (op == 0 || op == 1) {
    if ((op == 0) && argv[2] != NULL) {
      req_line = atoi(argv[2]);
      req_message = "no_message";
    } else if ((op == 1) && argv[3] != NULL) {
      req_line = -1;
      req_message = argv[3];
    }
  } else {
    printf("Uso errado, execute o programa sem arg para ajuda\n");
  }

  strcpy(&request.body[0], req_message);
  request.op = op;
  request.line = req_line;

	write(sockfd, &request, sizeof(request_t));
	sleep(1);
	read(sockfd, &response, sizeof(request_t));

  if (response.op != 1) {
    printf("Não foi possível realizar requisição\n");
  } else {
	  printf("Operação resposta (1 indica sucesso): %d\n", response.op);
    printf("Mensagem resposta: %s\n", response.body);
  }
	close(sockfd);
	exit(0);
}
