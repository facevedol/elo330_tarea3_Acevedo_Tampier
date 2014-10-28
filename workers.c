#include "workers.h"
//#include "buffer_list.h"
#include "constants.h"

#include <stdlib.h>			
#include <sys/socket.h>     // sockets
#include <netinet/in.h>
#include <netdb.h>			// hostent
#include <string.h>			// memcpy



void *sender(void *context){

	int client_socket;
	int addr_len;
	struct sockaddr_in server;
	struct hostent *hp;
	packet_t *pkt;
	workerParams_t *params = context;

	/*-----------SOCKETS----------------*/   
    /*Create the sockets*/
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* Look up our host's network address */	
	hp = gethostbyname(params->hostname);
	
    /* Create the address of the server. */
    server.sin_family = AF_INET;
    server.sin_port = htons(params->port);
    memcpy(&server.sin_addr, hp->h_addr_list[0], hp->h_length);
    addr_len = sizeof(struct sockaddr_in);
    /*----------END SOCKETS-------------*/

    /*--------SERVING FOR EVER----------*/
    for(;;){

    	/* [TODO] Wait until producer pruduce with some semaphore */
    	/* [TODO] Lock the list for this pop */
    	pkt = pop_packet(params->head);

    	/* [TODO] Sleeps for (pkt->time + params->delay_avg + (rand()/RAND_MAX-1)*2*params->delay_var) - now */
    	/* [TODO] If rand()/RAND_MAX < params->lost_perc */
    	sendto(client_socket, pkt->buffer, pkt->len, 0, (struct sockaddr*) &server, addr_len);

    }

	/*---------NEVER REACHED-----------*/
}

void *receiver(void *context){
	
	int server_socket;
	int addr_len;
	int buffer_len;
	char buffer[MTU];
	struct sockaddr_in server;
	workerParams_t *params = context;

	/*-----------SOCKETS----------------*/ 
	/*Create the sockets*/
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

	/* Create the address of the server.*/
    server.sin_family = AF_INET;
    server.sin_port = htons(params->port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_len = sizeof(struct sockaddr_in);
    bind(server_socket, (struct sockaddr *) &server, addr_len);
    /*----------END SOCKETS-------------*/

    /*--------SERVING FOR EVER----------*/
    for(;;){
	    if ((buffer_len = recv(server_socket, buffer, sizeof(buffer), 0)) > 0){
			/* [TODO] Lock the list for this push */
			push_packet(params->head, params->tail, buffer, buffer_len);
			/* [TODO] Add to the produced counter */
		}
	}

	/*---------NEVER REACHED-----------*/
}