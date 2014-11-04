#include <stdio.h>          // printf
#include <stdlib.h>         // exit
#include <pthread.h>
#include <sys/socket.h>     // sockets
#include "buffer_list.h"    // buffer handlers
#include "workers.h"        // conection handlers, here is where the job is done
#include "constants.h"      // constants


#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <time.h>




/*main function*/
int main(int argc, char *argv[]){

    printf("Starting\n");
    packet_t *head_c2s = NULL;
    packet_t *tail_c2s = NULL;
    packet_t *head_s2c = NULL;
    packet_t *tail_s2c = NULL;
    
    int client_socket, server_socket;
    struct sockaddr_in server_snd, server_rcv, client;
    unsigned int server_snd_len, server_rcv_len, client_len;
    
    /* Init Mutex */
    pthread_mutex_t client_addr_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t server_addr_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t buffer_c2s_mutex  = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t buffer_s2c_mutex  = PTHREAD_MUTEX_INITIALIZER;
    
    /* Init semaphores */
    
    int *sem_c2s = (int *)malloc(sizeof(int));
    int *sem_s2c = (int *)malloc(sizeof(int));

    /*input vars*/
    int delay_avg;
	int delay_var;
    float lost_perc;
    int client_port;
    int server_port;
    char *server_host = (char *)malloc(60*sizeof(char));
    
    /*extra vars*/
    int error;
    struct hostent *hp;
    workerParams_t *params_c2s_snd = (workerParams_t *)malloc(sizeof(workerParams_t));
    workerParams_t *params_c2s_rcv = (workerParams_t *)malloc(sizeof(workerParams_t));
    workerParams_t *params_s2c_snd = (workerParams_t *)malloc(sizeof(workerParams_t));
    workerParams_t *params_s2c_rcv = (workerParams_t *)malloc(sizeof(workerParams_t));
    pthread_t Mythread[4];
	
	/*------COMMAND LINE ARGUMENTS-------*/
	printf("Getting the arguments\n");
    if(argc == 6){
        delay_avg = atoi(argv[1]);
    	delay_var = atoi(argv[2]);
	    lost_perc = atof(argv[3]);
	    client_port = atoi(argv[4]);
	    server_port = atoi(argv[5]);
	    server_host = "localhost";
    }
    else if(argc == 7){
        delay_avg = atoi(argv[1]);
	    delay_var = atoi(argv[2]);
    	lost_perc = atof(argv[3]);
    	client_port = atoi(argv[4]);
        server_host = argv[5];
	    server_port = atoi(argv[6]);
	    
    }
    else{
        printf("wrong arguments\n");
         exit(-1);
    }
    
    /*----------------WORKFLOW--------------*/
    /* 1: CREATE WORKER PARAMS              */
    /* 2: CREATE PTHREAD FOR:               */
    /* 2.1: CLIENT  -> THIS     RECEIVER    */
    /* 2.2: THIS    -> SERVER   SENDER      */
    /* 2.3: SERVER  -> THIS     RECEIVER    */
    /* 2.4: THIS    -> CLIENT   SENDER      */
    /*--------------------------------------*/
    /* 2.1 CAN WORK IN THE MAIN, 
     * OTHERWISE MAIN WAITS A JOIN FOREVER  */

    /*------------SEMAPHORES-----------------*/
    *sem_c2s = sem_create(KEY_C2S, 0);
    *sem_s2c = sem_create(KEY_S2C, 0);
            
    /*-------------SOCKETS------------------*/
    
    /*------------------CLIENT SOCKET--------------------*/
    /* Socket to be passed to the client handler threads */
	/*Create the sockets*/
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	/* Create the address of the client server.*/
    server_rcv.sin_family = AF_INET;
    server_rcv.sin_port = htons(client_port);
    server_rcv.sin_addr.s_addr = htonl(INADDR_ANY);
    server_rcv_len = sizeof(struct sockaddr_in);

    /*----------END SERVER SOCKETS---------*/
    
    /*-------------SERVER SOCKET-----------------*/
    /* Socket to be passed to the server handler threads */
    /* Create the sockets */
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* Look up our host's network address */	
	hp = gethostbyname(server_host); 
	
    /* Create the address of the server. */
    server_snd.sin_family = AF_INET;
    server_snd.sin_port = htons(server_port);
    memcpy(&server_snd.sin_addr, hp->h_addr_list[0], hp->h_length);
    server_snd_len = sizeof(struct sockaddr_in);
    /*------------END CLIENT SOCKETS-----------*/

    /* Params for client to server sender threads */
    params_c2s_snd->addr        = &server_snd;
    params_c2s_snd->addr_len    = &server_snd_len;
    params_c2s_snd->socket      = &server_socket;
    params_c2s_snd->delay_avg   = delay_avg;
    params_c2s_snd->delay_var   = delay_var;
	params_c2s_snd->lost_perc   = lost_perc;
	params_c2s_snd->head        = &head_c2s;
	params_c2s_snd->tail        = &tail_c2s;
	params_c2s_snd->sem         = sem_c2s;
	params_c2s_snd->buffer_mutex= &buffer_c2s_mutex;
	params_c2s_snd->addr_mutex  = &server_addr_mutex;
	params_c2s_snd->from_addr   = NULL;
	params_c2s_snd->from_addr_len = NULL;	
	
	/* Params for client to server receiver threads */
    params_c2s_rcv->addr        = &server_rcv;
    params_c2s_rcv->addr_len    = &server_rcv_len;
    params_c2s_rcv->socket      = (int *)&client_socket;
    params_c2s_rcv->delay_avg   = delay_avg;
    params_c2s_rcv->delay_var   = delay_var;
	params_c2s_rcv->lost_perc   = lost_perc;
	params_c2s_rcv->head        = &head_c2s;
	params_c2s_rcv->tail        = &tail_c2s;
	params_c2s_rcv->sem         = sem_c2s;
	params_c2s_rcv->buffer_mutex= &buffer_c2s_mutex;
	params_c2s_rcv->addr_mutex  = &client_addr_mutex;
	params_c2s_rcv->from_addr   = &client;
	params_c2s_rcv->from_addr_len = &client_len;
	
    /* Params for server to client sender threads */
    params_s2c_snd->addr        = &client;
    params_s2c_snd->addr_len    = &client_len;
    params_s2c_snd->socket      = (int *)&client_socket;
    params_s2c_snd->delay_avg   = delay_avg,
    params_s2c_snd->delay_var   = delay_var;
	params_s2c_snd->lost_perc   = lost_perc;
	params_s2c_snd->head        = &head_s2c;
	params_s2c_snd->tail        = &tail_s2c;
	params_s2c_snd->sem         = sem_s2c;
	params_s2c_snd->buffer_mutex= &buffer_s2c_mutex;
	params_s2c_snd->addr_mutex  = &client_addr_mutex;
	params_s2c_snd->from_addr   = NULL;
    params_s2c_snd->from_addr_len = NULL;
        
	/* Params for server to client receiver threads */
    params_s2c_rcv->addr        = &server_snd;
    params_s2c_rcv->addr_len    = &server_snd_len;
    params_s2c_rcv->socket      = &server_socket;
    params_s2c_rcv->delay_avg   = delay_avg,
    params_s2c_rcv->delay_var   = delay_var;
	params_s2c_rcv->lost_perc   = lost_perc;
	params_s2c_rcv->head        = &head_s2c;
	params_s2c_rcv->tail        = &tail_s2c;
	params_s2c_rcv->sem         = sem_s2c;
	params_s2c_rcv->buffer_mutex= &buffer_s2c_mutex;
	params_s2c_rcv->addr_mutex  = &server_addr_mutex;
	params_s2c_rcv->from_addr = NULL;
	params_s2c_rcv->from_addr_len = NULL;
    
    /* Theads initialization*/
    if ((error = pthread_create(&(Mythread[0]),NULL,&sender,params_c2s_snd)))
        fprintf(stderr,"Error creating thread 0: %s\n",strerror(error));
    if ((error = pthread_create(&(Mythread[1]),NULL,&sender,params_s2c_snd)))
	fprintf(stderr,"Error creating thread 0: %s\n",strerror(error));
    if ((error = pthread_create(&(Mythread[2]),NULL,&receiver,params_c2s_rcv)))
        fprintf(stderr,"Error creating thread 0: %s\n",strerror(error));
    if ((error = pthread_create(&(Mythread[3]),NULL,&receiver,params_s2c_rcv)))
        fprintf(stderr,"Error creating thread 0: %s\n",strerror(error));


    pthread_join(Mythread[2], NULL);
    
    return 0;
}
