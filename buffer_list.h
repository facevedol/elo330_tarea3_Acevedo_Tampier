#ifndef BUFFER_LIST_H
#define BUFFER_LIST_H
/*------------------STRUCTS----------------------------*/
typedef struct packet packet;

/*-----------------PROTOTYPES---------------------------*/
packet* create_packet(char* buffer);
void append_packet(packet *head, packet *tail, char *buffer);
packet* get_next_packet(packet* head);

#endif