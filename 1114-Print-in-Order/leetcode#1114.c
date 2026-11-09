
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdatomic.h>
// ===============================
// Your implementation
// ===============================


typedef struct {
    // Your synchronization data
    atomic_int counter;
} Foo;

Foo* fooCreate(void) {
    Foo* obj = malloc(sizeof(Foo));
    if(obj==NULL)
        return NULL;
    atomic_init(&obj->counter,0);
    // TODO: Initialize your data

    return obj;
}

void first(Foo* obj) {
    // TODO: Ensure execution order
    printf("first");
    atomic_store(&obj->counter,1);
    
    
}

void second(Foo* obj) {
    // TODO: Ensure execution order

    while (atomic_load(&obj->counter) != 1)
    {

    }
    printf("second");    
    atomic_store(&obj->counter,2);
        
    
}

void third(Foo* obj) {
    // TODO: Ensure execution order
    while(atomic_load(&obj->counter)!=2)
    {
    }
    printf("third");
    atomic_store(&obj->counter,3);
        
}

void fooFree(Foo* obj) {
    // TODO: Release synchronization resources
    free(obj);
}

// ===============================
// Test harness
// ===============================

typedef struct {
    Foo* obj;
    int id;
} ThreadArg;

void* threadEntry(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;

    switch (data->id) {
        case 0: first(data->obj);  break;
        case 1: second(data->obj); break;
        case 2: third(data->obj);  break;
    }

    return NULL;
}

int main(void) {
    srand((unsigned int)time(NULL));

    for (int test = 0; test < 20; test++) {
        Foo* obj = fooCreate();

        pthread_t threads[3];
        ThreadArg args[3];

        int order[3] = {0, 1, 2};

        // Shuffle thread creation order
        for (int i = 2; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }

        printf("Test %02d [%d,%d,%d]: ",
               test + 1,
               order[0] + 1,
               order[1] + 1,
               order[2] + 1);

        for (int i = 0; i < 3; i++) {
            args[i].obj = obj;
            args[i].id = order[i];

            pthread_create(
                &threads[i],
                NULL,
                threadEntry,
                &args[i]
            );
        }

        for (int i = 0; i < 3; i++) {
            pthread_join(threads[i], NULL);
        }

        printf("\n");
        fooFree(obj);
    }

    return 0;
}
