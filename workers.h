#ifndef WORKERS_H
#define WORKERS_H

#include "buffer_list.h"

/*-------------------STRUCTS----------------------------*/
typedef struct workerParams{
	int delay_avg;
	int delay_var;
	int lost_perc;
	int port;
	char *hostname;
	packet_t *head;
	packet_t *tail;
} workerParams_t;

/*------------------PROTOTYPES--------------------------*/
void *sender(void *context);
void *receiver(void *context);

#endif