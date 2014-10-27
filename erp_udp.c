#include <stdio.h>          //printf
#include <pthread.h>
#include <sys/socket.h>     //sockets
#include "buffer_list.h"    //buffer handlers
#include "constants.h"      //constants

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define PORTNUMBER_IN  1234
#define PORTNUMBER_OUT  12345

packet *head, *tail;


/*main function*/
int main(int argc, char *argv[]){
    
    
    int delay_avg = atoi(argv[1]);
	int delay_var = atoi(argv[2]);
	int lost_perc = atoi(argv[3]);
    
    struct sockaddr_in server;
	struct sockaddr_in client;
	struct hostent *hp;

    char hostname[64];
    int socket_in, socket_out;
    
    int len, len2;
    
    /*-----------SOCKETS----------------*/
    
    /*Create the sockets*/
    socket_in = socket(AF_INET, SOCK_DGRAM, 0);
    socket_out = socket(AF_INET, SOCK_DGRAM, 0);
    
    /* Create the address of the server.*/
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTNUMBER_IN);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(struct sockaddr_in);
    bind(socket_in, (struct sockaddr *) &server, len);
    
    
    gethostname(hostname, sizeof(hostname)); //debemos obtener el host entregado como parametro...
	/* Look up our host's network address */	
	hp = gethostbyname(hostname);
	
    /* Create the address of the client. */
    client.sin_family = AF_INET;
    client.sin_port = htons(PORTNUMBER_OUT);
    memcpy(&client.sin_addr, hp->h_addr_list[0], hp->h_length);
    len2 = sizeof(struct sockaddr_in);
    
    /*----------END SOCKETS-------------*/
    
    return 0;
}
