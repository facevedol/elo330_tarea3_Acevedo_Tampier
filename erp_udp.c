#include <stdio.h>          // printf
#include <pthread.h>        // threads
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


packet *head, *tail;


/*main function*/
int main(int argc, char *argv[]){
    
    
    int delay_avg = atoi(argv[1]);
	int delay_var = atoi(argv[2]);
	int lost_perc = atoi(argv[3]);
    
    /*----------------WORKFLOW--------------*/
    /* 1: CREATE WORKER PARAMS              */
    /* 2: CREATE PTHREAD FOR:               */
    /* 2.1: CLIENT  -> THIS     RECEIVER    */
    /* 2.2: THIS    -> SERVER   SENDER      */
    /* 2.3: SERVER  -> THIS     RECEIVER    */
    /* 2.4: THIS    -> CLIENT   SENDER      */
    /*--------------------------------------*/
    /* 2.1 CAN WORK IN THE MAIN, 
     * OTHERWISE NO MAIN IS REQUIRED        */
    
    return 0;
}
