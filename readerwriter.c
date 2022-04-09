// READER WRITER PROBLEM

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

// go to link below for details on these header files
// http://faculty.cs.niu.edu/~hutchins/csci480/semaphor.htm

sem_t wrt; // initializes semaphore
pthread_mutex_t mutex; // initializes mutex

int cnt = 1;
int readcount = 0;

void *writer(void *wno)
{   
    sem_wait(&wrt); // equivalent to wait(wrt)

        printf("\nWriter %d is entering.\n", (*((int *)wno)));
    
        // Writer Operation
        cnt = cnt * 2;
        printf("Writer %d modified cnt to %d\n", (*((int *)wno)), cnt);

        printf("Writer %d has exited.\n\n", (*((int *)wno)));
    sem_post(&wrt); // equivalent to signal(wrt)
}

void *reader(void *rno)
{   
    // Reader acquire the lock before modifying readcount
    pthread_mutex_lock(&mutex);
        readcount++;

        if(readcount == 1) {
            sem_wait(&wrt); // If this is the first reader, then it will block the writer
        }
        printf("\nReader %d is entering.\n", (*((int *)rno)));

    pthread_mutex_unlock(&mutex); // other readers are allowed now

    // Reading Operation
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);

    // Reader acquires the lock before modifying readcount
    pthread_mutex_lock(&mutex);
        printf("\nReader %d has exited.\n", (*((int *)rno)));
        readcount--;

        if(readcount == 0) {
            sem_post(&wrt); // If this is the last reader, you got to allow other writers.
        }

    pthread_mutex_unlock(&mutex); // removes restrictions on readcount
}

int main()
{   
    pthread_t read[5], write[5]; // threads // 5 readers and 3 writers
    
    pthread_mutex_init(&mutex, NULL);
    // Prototype: int pthread_mutex_init(pthread_mutex_t * restrict mutex, const pthread_mutexattr_t * restrict attr);
    // If attr is NULL, a default set of attributes is used. The initial state of *mutex will be "initialized and unlocked".

    sem_init(&wrt, 0, 1); 
    // Prototype: int sem_init(sem_t * sem, int pshared, unsigned int value);
    // '1' is the initial value of the semaphore
    // pshared is 0 => the semaphore is shared among all threads of a process

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the reader and writer

    // pthread_create subroutine creates a new thread
    for(int i = 0; i < 5; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]); // creating threads 
        // int pthread_create (thread, attr, start_routine (void *), arg)
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }
    // for(int i = 0; i < 5; i++) {
        
    // }


    // int pthread_join (thread, status)
    // pthread_t thread;
    // void **status;
    // pthread_join subroutine blocks the calling thread until the thread 'thread' terminates

    for(int i = 0; i < 5; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // uninitializes mutex
    sem_destroy(&wrt); // uninitializes semaphore

    return 0;    
}