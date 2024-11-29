#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define DEPOT_CAPACITY 10
#define BUS_CAPACITY 5

/* Mutex */
pthread_mutex_t mutex;

/* Semaphores */
sem_t passenger_ready;
sem_t bus_ready;
sem_t boarding_complete;

/* Shared Variables */
int waiting_passengers = 0;  // Number of passengers waiting at the depot
int boarded_passengers = 0; // Number of passengers that have boarded the bus
int bus_departed = 0;       // Flag to indicate bus departure

void *bus_function(void *arg);
void *passenger_function(void *arg);

void board_passenger() {
    printf("Passenger boards the bus.\n");
    sleep(1); // Simulate boarding time
}

void *bus_function(void *arg) {
    while (1) {
        /* Wait for passengers to be ready */
        sem_wait(&passenger_ready);

        pthread_mutex_lock(&mutex);
        int boarding_passengers = (waiting_passengers < BUS_CAPACITY) ? waiting_passengers : BUS_CAPACITY;
        printf("Bus arrives: %d passengers ready to board.\n", boarding_passengers);
        pthread_mutex_unlock(&mutex);

        /* Signal that the bus is ready */
        for (int i = 0; i < boarding_passengers; i++) {
            sem_post(&bus_ready);
        }

        /* Wait for all passengers to board */
        for (int i = 0; i < boarding_passengers; i++) {
            sem_wait(&boarding_complete);
        }

        pthread_mutex_lock(&mutex);
        waiting_passengers -= boarding_passengers;
        printf("Bus departs with %d passengers.\n", boarding_passengers);
        bus_departed = 1;  // Set the bus departure flag
        pthread_mutex_unlock(&mutex);

        sleep(2); // Simulate bus travel time
    }
}

void *passenger_function(void *arg) {
    pthread_mutex_lock(&mutex);

    /* Check depot capacity */
    if (waiting_passengers < DEPOT_CAPACITY) {
        waiting_passengers++;
        printf("Passenger arrives at the depot. Waiting passengers: %d\n", waiting_passengers);

        /* Signal the bus if it's ready */
        sem_post(&passenger_ready);

        pthread_mutex_unlock(&mutex);

        /* Wait for the bus to be ready */
        sem_wait(&bus_ready);
        board_passenger();

        /* Signal that boarding is complete */
        sem_post(&boarding_complete);
    } else {
        printf("Depot is full. Passenger leaves.\n");
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    int num_passengers;
    printf("Enter the number of passengers: ");
    scanf("%d", &num_passengers);

    /* Initialize mutex and semaphores */
    pthread_mutex_init(&mutex, NULL);
    sem_init(&passenger_ready, 0, 0);
    sem_init(&bus_ready, 0, 0);
    sem_init(&boarding_complete, 0, 0);

    /* Create bus thread */
    pthread_t bus_thread;
    if (pthread_create(&bus_thread, NULL, bus_function, NULL) != 0) {
        perror("Failed to create bus thread");
        exit(EXIT_FAILURE);
    }

    /* Create passenger threads */
    pthread_t *passenger_threads = malloc(num_passengers * sizeof(pthread_t));
    if (passenger_threads == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_passengers; i++) {
        if (pthread_create(&passenger_threads[i], NULL, passenger_function, NULL) != 0) {
            perror("Failed to create passenger thread");
            exit(EXIT_FAILURE);
        }

        /* Sleep to simulate arrival interval */
        sleep(rand() % 3);
    }

    /* Join passenger threads */
    for (int i = 0; i < num_passengers; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    /* Cleanup */
    pthread_mutex_destroy(&mutex);
    sem_destroy(&passenger_ready);
    sem_destroy(&bus_ready);
    sem_destroy(&boarding_complete);
    free(passenger_threads);

    printf("Simulation complete.\n");
    return 0;
}
