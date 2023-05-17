/*  test machine: atlas.cselabs.umn.edu (ssh)
*   group number: G19
*   name: Timothy Anderson, Luc Rubin, Hank Hanson
*   x500: and09985, rubin206, hans7186 */

#include "producer.h"

/**
 *
 * Producer thread will read from the file and write data to 
 * the end of the shared queue
 */
void *producer(void *arg){
    
    //if the log file exists, then print to it
    if (log_fp) {
    	fprintf(log_fp, "producer\n");
    }
    
    //open input file or if error then terminate and print error message
    char *path = (char *) arg;
    FILE * fp = getFilePointer(path);
    if (fp == NULL){
    	fprintf(stderr, "Error opening file: %s in producer.c\n", path);
        return NULL;
    }
    
    //local vars for reading input file
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i=0;
    
    
    while ((read = getline(&line, &len, fp)) != -1){
    	//no need to wait on first element
    	if (bound > 0) {
    		sem_wait(&sem_bound);
    	}	
    	insert(line, i);
        if (log_fp) {
    		fprintf(log_fp, "producer: line %d\n", i);
        }
    	if (i!=0 || bound==1) {		//no need to post on first element unless the bound is 1
    		sem_post(&sem_cond);
    	}
    	i++;
    }
    
    //EOF reached, send notifications to all of the consumers
    for (i=0; i<consumer_num; i++){
        if (bound > 0) {
    		sem_wait(&sem_bound);
    	}	
    	insert("-1", -1);
    	if (log_fp) {
    		fprintf(log_fp, "producer: line -1\n");
    	}
    	sem_post(&sem_cond);
    }
    
    if (bound != 1) sem_post(&sem_cond); //allows a consumer to extract the last packet
    
    fclose(fp);
    free(line);
    return NULL; 
}
