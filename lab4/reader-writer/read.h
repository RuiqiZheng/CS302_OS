#include"init.h"

void *reader(int *buffer){
    // Reader should read the value of buffer
    // Reader cannot start to work when there is any writer working
    // You should implement this using semaphore

    // You should output this just after this thread gets semaphore
    printf("Reader gets sem\n");
    sem_wait(&rc);
    if (!readcount)
    {
        sem_wait(&db);
    }
    readcount++;
    sem_post(&rc);
    sem_wait(&rc);
    readcount--;
    if (!readcount)
    {
        sem_post(&db);
    }
    sem_post(&rc);
    // you should output this just before this thread releases the semaphore
    printf("Reader reads %d and releases\n", *buffer);
}

