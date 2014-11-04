#ifndef WORKERS_H
#define WORKERS_H

#include "buffer_list.h"
#include <pthread.h>

/*-------------------STRUCTS----------------------------*/
typedef struct workerParams{
	struct sockaddr_in *addr;
	unsigned int *addr_len;
	struct sockaddr_in *from_addr;
	unsigned int *from_addr_len;
	int *socket;
	int delay_avg;
	int delay_var;
	float lost_perc;
	packet_t **head;
	packet_t **tail;
	int *sem;
	pthread_mutex_t *buffer_mutex;
	pthread_mutex_t *addr_mutex;
} workerParams_t;

/*------------------PROTOTYPES--------------------------*/
void *sender(void *context);
void *receiver(void *context);


#endif
