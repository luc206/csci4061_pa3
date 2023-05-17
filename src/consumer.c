/*  test machine: atlas.cselabs.umn.edu (ssh)
*   group number: G19
*   name: Timothy Anderson, Luc Rubin, Hank Hanson
*   x500: and09985, rubin206, hans7186 */

#include "consumer.h"
#include <ctype.h>

/**
 * parse lines from the queue, calculate balance change
 * and update to global array
 */
void parse(char *line){
    
    //get customer id
    int id; 
    sscanf(line, "%d,", &id); // Reads in the account id
    if (id == -1) return; // EOF packet, nothing to parse
    
    double transaction, total = 0;
    while (*line != '\n' && *line != '\0') {
        while (*line != ',' && *line != '\n' && *line != '\0') line++; // Moves the string pointer to the next data point
        if (*line == ',') line++; // Puts the pointer on the first character in the next number
        if (sscanf (line, "%lf%*c", &transaction) != EOF) total += transaction; // If a number is read, add it to the total
    }

    //update the global array
    pthread_mutex_lock(&acctsLock[id]); // Lock for the specific account this thread is modifying
    balance[id] += total;
    pthread_mutex_unlock(&acctsLock[id]);
}

// consumer function
void *consumer(void *arg){

    int id = *(int *) arg;
    if (log_fp) fprintf(log_fp, "consumer %d\n", id); 

    struct Packet* packet = NULL;
    do {
        if (packet) {
            free(packet->data); // Frees the string data of the previously used packet
            free(packet); // Frees up the previously used packet
        }
        sem_wait(&sem_cond);
        sem_wait(&sem_mutex); // Mutual exclusion between consumers, only one consumer should modify the head of the list

        packet = extract();
        if (log_fp) fprintf(log_fp, "consumer %d: line %d\n", id, packet->line);
        
        if (bound) sem_post(&sem_bound); // Signal the upper bound that a spot in the list has been freed up
        sem_post(&sem_mutex);
        
        parse(packet->data);
    } while (strcmp(packet->data, "-1")); // Stops the loop when the special EOF packet is reached
    
    if (packet) { // Frees the last packet held by the consumer
        free(packet->data);
        free(packet);
    }

    return NULL; 
}


