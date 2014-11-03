#include <stdlib.h>
#include "buffer_list.h"
#include "constants.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>

packet_t *create_packet(char *buffer, int len){
    packet_t *new_packet    = (packet_t *)malloc(sizeof(packet_t));
    new_packet->len         = len;
    new_packet->next        = NULL;
    strcpy(new_packet->buffer, buffer);
    gettimeofday(new_packet->at, NULL);
    return new_packet;
}

void push_packet(packet_t *head, packet_t *tail, char *buffer, int len){
    packet_t *new_packet = create_packet(buffer, len);
    /// Empty packet list
    if(head == NULL){
        head = new_packet;
        return;
    }
    /// No tail defined yet
    if(tail == NULL){
        tail = new_packet;
        head->next = tail;
        return;
    }
    /// Append packet in the end of the list
    tail->next = new_packet;
    tail = new_packet;
    return;
}

packet_t *pop_packet(packet_t *head){
    packet_t *next = head;
    head = head->next;

    return next;    
}