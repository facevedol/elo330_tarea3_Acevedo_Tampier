#include "buffer_list.h"
#include "constants.h"


struct packet{
       char buffer[MTU];
       int len;
      // struct timeval time,
       struct packet *next;
};

packet* create_packet(char* buffer, int len){
    packet *new_packet  = (packet*)malloc(sizeof(packet));
    new_packet->buffer  = buffer;
    new_packet->len     = len;
    new_packet->next    = NULL;
    //new_packet->time = now();
    return new_packet;
}

void push_packet(packet *head, packet *tail, char *buffer, int len){
    packet *new_packet = create_packet(buffer, len);
    /// Empty packet list
    if(head == NULL){
        head = new_packet;
        return
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

packet* pop_packet(packet* head){
    packet *next = head;
    head = head->next;

    return next;    
}