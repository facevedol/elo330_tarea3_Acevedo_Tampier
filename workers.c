#include "workers.h"
//#include "buffer_list.h"
#include "constants.h"
#include <pthread.h>

#include <unistd.h>
#include <stdlib.h>			
#include <sys/socket.h>     // sockets
#include <netinet/in.h>
#include <netdb.h>			// hostent
#include <string.h>			// memcpy
#include <time.h>
#include <sys/time.h>



void *sender(void *context){

	packet_t *pkt;
	workerParams_t *params = context;
	double at; 			// Arrival time in usecs;
	struct timeval ctv;	// Current Time in timeval
	double ct;			// Current Time in usecs
	double st;			// Sleep time in usec
	

    /*--------SERVING FOR EVER----------*/
    for(;;){

    	/* Wait until producer pruduce with some semaphore */
    	sem_wait(*(params->sem));
    	pthread_mutex_lock( params->buffer_mutex );
    		pkt = pop_packet(params->head);
    	pthread_mutex_unlock( params->buffer_mutex );

    	/* Sleeps for (pkt->time + params->delay_avg +/- params->delay_var) - now */
    	at = pkt->at->tv_sec*1000000 +  pkt->at->tv_usec;
    	gettimeofday(&ctv, NULL);
    	ct = ctv.tv_sec*1000000 + ctv.tv_usec;
    	if ( rand()/RAND_MAX > 0.5) {
    		st = at + params->delay_avg + params->delay_var - ct;
    	}
    	else {
    		st = at + params->delay_avg - params->delay_var - ct;
    	}
    	/* Sleep if needed */
    	if ( st > 0)
    		usleep( st );
    	
    	/* If the packet is not lost, send it */
    	if ( rand()/RAND_MAX > params->lost_perc){
	    	pthread_mutex_lock( params->addr_mutex );
	    		sendto(*(params->socket), pkt->buffer, pkt->len, 0, (struct sockaddr*) params->addr, *(params->addr_len));
			pthread_mutex_unlock( params->addr_mutex );
    	}
		free(pkt);
    }

	/*---------NEVER REACHED-----------*/
}

void *receiver(void *context){
	
	int buffer_len;
	char buffer[MTU];
	workerParams_t *params = context;

    /*--------SERVING FOR EVER----------*/
    for(;;){
    	pthread_mutex_lock( params->addr_mutex );
    		buffer_len = recvfrom(*(params->socket), buffer, sizeof(buffer), 0, (struct sockaddr*)params->addr, params->addr_len);
	    pthread_mutex_unlock( params->addr_mutex );
	    
	    if (buffer_len > 0){
			pthread_mutex_lock( params->addr_mutex );
				push_packet(params->head, params->tail, buffer, buffer_len);
			pthread_mutex_unlock( params->addr_mutex );
			/* Signal that are packets availables */
			sem_signal(*(params->sem));
		}
	}

	/*---------NEVER REACHED-----------*/
}