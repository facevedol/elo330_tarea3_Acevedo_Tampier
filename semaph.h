#ifndef SEMAPH_H
#define SEMAPH_H

#include	<sys/types.h>

int sem_create(key_t key, int initval);
int sem_open(key_t key);
void sem_rm(int id);
void sem_close(int id);
void sem_wait(int id);
void sem_signal(int id);
void sem_op(int id, int value);


#endif