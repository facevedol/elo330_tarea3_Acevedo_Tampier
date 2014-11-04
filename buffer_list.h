#ifndef BUFFER_LIST_H
#define BUFFER_LIST_H

#include "constants.h"

/*-------------------STRUCTS----------------------------*/
typedef struct packet{
    char buffer[MTU];
    int len;
    struct timeval *at;     // Arrival Time
    struct packet *next;
} packet_t;

/*------------------PROTOTYPES--------------------------*/
packet_t *create_packet(char* buffer, int len);
void 	 push_packet(packet_t **head, packet_t **tail, char *buffer, int len);
packet_t *pop_packet(packet_t **head);

#endif
