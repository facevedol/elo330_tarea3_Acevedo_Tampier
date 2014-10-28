#ifndef BUFFER_LIST_H
#define BUFFER_LIST_H

/*-------------------STRUCTS----------------------------*/
typedef struct packet packet;

/*------------------PROTOTYPES--------------------------*/
packet* create_packet(char* buffer);
void 	push_packet(packet *head, packet *tail, char *buffer, int len);
packet* pop_packet(packet* head);

#endif