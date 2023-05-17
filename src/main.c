/*  test machine: atlas.cselabs.umn.edu (ssh)
*   group number: G19
*   name: Timothy Anderson, Luc Rubin, Hank Hanson
*   x500: and09985, rubin206, hans7186 */

#include "header.h"

// Global variable 
double balance[acctsNum]; // global array of balance changes for each account

// Linked list of packets containing a string of data to be parsed by a consumer thread
struct Packet* head = NULL;
struct Packet* tail = NULL;

sem_t sem_mutex; // Semaphore for mutual exclusion between consumer threads when extracting a packet from the queue
sem_t sem_cond; // Semaphore for allowing the consumer to extract packets the producer thread is done with
sem_t sem_bound; // Semaphore for the bound option, limiting the total amount of packets that can be in the queue
pthread_mutex_t acctsLock[acctsNum]; // Locks for each individual account in the balance array, stops consumer threads from writing to the same account

int consumer_num = 0;
int bound = 0;

FILE *log_fp = NULL; // File for log output, if NULL no log will be created

void writeBalanceToFiles(void) {
    FILE *fp; 

    fp = fopen(finalDir, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s in main.c\n", finalDir);
        exit(EXIT_FAILURE);
    }

    double total = 0.0;

    for (int i = 0; i < acctsNum; ++i) { 
        fprintf(fp, "%d\t%lf\n", i, balance[i]);
        total += balance[i]; // Calculates the total assets changed
    }

    fprintf(fp, "All: \t%lf\n", total);

    fclose(fp);
}

int main(int argc, char *argv[]){
    
    if (argc < 3) { // Checking that the required arguments where passed
        fprintf(stderr, "Error: received %d arguments, but expected 3 arguments \n"
                        "Usage: ./bank #consumers inputFile [option] [#queueSize] \n", argc);
        return EXIT_FAILURE;
    }

    bookeepingCode();
    
    // #consumer
    consumer_num = atoi(argv[1]);
    if (consumer_num < 1) {
        printf("Error: #consumer should be greater than 0, recieved %d \n", consumer_num);
        return EXIT_FAILURE;
    }
    
    // inputFile
    char* path = argv[2];

    // option and #queueSize
    if (argc > 3) {
        if (strchr(argv[3], 'p')) { // Option to produce an log file
            log_fp = fopen(logDir, "w");
        }

        if (strchr(argv[3], 'b') && argc > 4) { // Option to bound how many packets can be in the queue
            bound = atoi(argv[4]);
            if (bound < 1) {
                printf("Error: #queueSize should be greater than 0, recieved %d \n", bound);
                return EXIT_FAILURE;
            }
        }
    }

    // Initialize semaphores, locks, and balance array
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_cond, 0, 0);
    sem_init(&sem_bound, 0, bound);
    int i;
    for (i = 0; i < acctsNum; i++) {
        pthread_mutex_init(&acctsLock[i], NULL);
        balance[i] = 0.0;
    }

    // Create producer and consumer threads
    pthread_t producert;
    pthread_t consumert[consumer_num];

    if (pthread_create(&producert, NULL, producer, path)) {
        printf("Failed to create the producer thread\n");
        return EXIT_FAILURE;
    }

    int arg_arr[consumer_num];
    for (i = 0; i < consumer_num; ++i) {
        arg_arr[i] = i;
        if (pthread_create(&(consumert[i]), NULL, consumer, &arg_arr[i])) {
            printf("Failed to create the consumer[%d] thread\n", i);
            return EXIT_FAILURE;
        }
    }

    // Join producer and consumer threads
    if (pthread_join(producert, NULL)) {
        printf("Failed to join the producer thread\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < consumer_num; i++) {
        if (pthread_join(consumert[i], NULL)) {
            printf("Failed to join the consumer[%d] thread\n", i);
            return EXIT_FAILURE;
        }
    }
    	
    writeBalanceToFiles();
    if (log_fp) fclose(log_fp); // Close the log file if it's open
    
    return 0; 
}
