#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_node {
    pthread_t* thread;
    struct thread_node* next;
    struct thread_node* prev;
    int thread_id;
} thread_node_t;

thread_node_t* create_thread_node(int thread_id) {
    thread_node_t* head = (thread_node_t*)malloc(sizeof(thread_node_t));
    head->next = NULL;
    head->prev = NULL;
    head->thread = (pthread_t*)malloc(sizeof(pthread_t));
    head->thread_id = thread_id;

    return head;
}

thread_node_t* get_last_node(thread_node_t* head) {
    thread_node_t* last = head;
    while (last->next != NULL) {
        last = last->next;
    }
    return last;
}

thread_node_t* add_thread_array(thread_node_t* head, int thread_id) {
    if (head != NULL) {
        thread_node_t* last = get_last_node(head);
        last->next = create_thread_node(thread_id);
        return last->next;
    } else {
        head = create_thread_node(thread_id);
        return head;
    }
}

void forNodeJoin(thread_node_t* head) {
    thread_node_t* current = head;
    while (current != NULL) {
        printf("NODE - JOIN: %d\n", current->thread_id);
        pthread_join(*current->thread, NULL);
        thread_node_t* temp = current;
        current = current->next;
        free(temp);
        free(temp->thread);
    }
}

