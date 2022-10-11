// gcc server_text_editor.c thread_node.h -o server_text_editor

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "thread_node.h"
#include <semaphore.h>

// Usar fprintf
// usar fscanf

typedef struct  {
    int client_sockfd;
    int thread_id;
    //FILE* text;
    char* file_name;
    sem_t* sem;
} connection_param;

typedef struct {
    int op;
    char body[30];
    int line;
} request_t;

int start_server (int port, int max_con) {
    unsigned int server_len, client_len;
    struct sockaddr_in server_address;

    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    server_address.sin_family = AF_INET;
    //server_address.sin_addr.s_addr = inet_addr("192.168.0.15");
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(port);

    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    int result = listen(server_sockfd, max_con);

    if (result == -1) {
        return -1;
    }

    return server_sockfd;
}

int read_command(char* command) {

}

int connect_client(connection_param* c_params) {
    c_params = (connection_param*) c_params;
    
    request_t command;
    request_t response;
    char body_response[30];
    FILE* text;


    read(c_params->client_sockfd, &command, sizeof(request_t));
    //printf("AUX:%d - TI:%d Operação Cliente: %d\n", getpid(), c_params->thread_id, command.op);
    //printf("AUX:%d - TI:%d Body Cliente: %s\n", getpid(), c_params->thread_id, command.body);
    //fseek(text, 0, SEEK_SET);
    //fscanf(text, "%s\n", &buffer[0]);
    //printf("AUX:%d - TI:%d - %s\n", getpid(), c_params->thread_id, &buffer[0]);
    //printf("AUX:%d - TI:%d - %p\n", getpid(), c_params->thread_id, c_params->text);

    sem_wait(c_params->sem);
    text = fopen("text.txt", "a+");

    if (command.op == 0 || command.op == 1) {
        // Leitura
        if (command.op == 0) {
            printf("AUX:%d - TI:%d - Lendo a linha n: %d no arquivo\n", getpid(), c_params->thread_id, command.line);
            fseek(text, 0, SEEK_SET);
            //printf("Setei o cursor do arquivo\n");
            int line = 0;
        
            while(1) {
                //printf("Line: %d\n", line);
                if (fscanf(text, "%s", body_response)) {
                    if (line == command.line) {
                        response.op = 1;
                        break;
                    }
                }  else {
                        response.op = -1;
                        printf("Erro ao ler o arquivo\n");
                        break;
                }
                line++;
            }
        } 
        // Escrita
        else {
            fseek(text, 0, SEEK_END);
            printf("AUX:%d - TI:%d - Escrevendo %s no arquivo\n", getpid(), c_params->thread_id, command.body);
            if (fprintf(text, "%s\n", &command.body[0])) {
                response.op = 1;
                strcpy(body_response, "sucesso");
                printf("Sucesso!\n");
            } else {
                response.op = -1;
                strcpy(body_response, "falha");
                printf("Falha!\n");
            };
        }
    }
    fclose(text);
    sem_post(c_params->sem);
    strcpy(&response.body[0], body_response);
    write(c_params->client_sockfd, &response, sizeof(request_t));
    return 0;
}

int main() {
    // FILE *text;
    int server_sockfd, client_sockfd, thread_id;
    struct sockaddr_in client_address;

    // FILE* text = fopen("text.txt", "wr");
    // fprintf(text, "%s\n", "Bloco de Notas Colaborativo  ");
    // fclose(text);

    thread_node_t* head = NULL;
    sem_t sem;
    sem_init(&sem, 0, 1);

    pthread_t* thread_teste = malloc(sizeof(pthread_t));

    thread_id = 0;
    server_sockfd = start_server(9734, 5);
    
    if (server_sockfd == -1) {
        printf("MAIN:%d Erro ao iniciar servidor\n", getpid());
        return 1;
    }

    while(1) {
        printf("MAIN:%d Servidor esperando conexões\n", getpid());
        printf("MAIN:%d Número de threads aux ativas %d\n", getpid(), thread_id);
        int client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    
        connection_param c_param;
        c_param.client_sockfd = client_sockfd;
        c_param.thread_id = thread_id;
        c_param.file_name = "text.txt";
        c_param.sem = &sem;
        //c_param.text = text;

        if (head == NULL) {
            head = create_thread_node(thread_id);
            pthread_create(head->thread, NULL, (void*) connect_client, &c_param);
        } else {
            thread_node_t* last = add_thread_array(head, thread_id);
            pthread_create(last->thread, NULL, (void*) connect_client, &c_param);
        }

        
        //printf("dale\n");
        //pthread_create(thread_teste, NULL, (void*) connect_client, &c_param);
        //break;
        thread_id ++;
	    }

    //pthread_join(*thread_teste, NULL);
    
    free(thread_teste);
    //free(head);
    // Dá join em todas as threads e libera a memória
    forNodeJoin(head);

    return 0;
}