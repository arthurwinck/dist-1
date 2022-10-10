#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_node.h"

void printar(int* thread_id) {
    printf("Olá eu sou a thread %d\n", *thread_id);
}

int main(int argc, char const *argv[])
{
    thread_node_t* head = NULL;
    pthread_t t[10];

    for (int i = 0; i < 10; i++) {
        printf("Tentando criar thread %d\n", i);
        //pthread_create(&t[i], NULL, (void*) printar, &i);
        if (head == NULL) {
            printf("Criando thread_node %d\n", i);
            head = create_thread_node(i);
            printf("Alocando thread %d\n para node\n", i);
            pthread_create((head->thread), NULL, (void*) printar, &i);
        } else {
            thread_node_t* last = add_thread_array(head, i);
            pthread_create(last->thread, NULL, (void*) printar, &i);
        }
    }

    forNode(head);

    return 0;
}
