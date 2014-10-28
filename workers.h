#ifndef WORKERS_H
#define WORKERS_H

/*-------------------STRUCTS----------------------------*/
typedef struct workerParams workerParams;

/*------------------PROTOTYPES--------------------------*/
void *sender(void *context);
void *receiver(void *context);

#endif