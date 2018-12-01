#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024


void * queue_server(void * args) 
{
    clock_t start, end;
    mqd_t mq;
    ssize_t bytes_read;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    int min = 2000;
    int max = 0;
    int data;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    
    /* construct the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);
    
    /*Start timer*/
    start = clock();
    while(1) 
    {	
        memset(buffer, 0x00, sizeof(buffer));
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if(bytes_read >= 0) 
        {
		
            data = atoi(buffer);
            if(data!=-1) //loop exit flag
            {
                printf("Server Process: Received Data: %s", buffer);
                if(data>max)
                {	
                    max = data;
                }
                if(data<min)
                {
                    min = data;
                }
                printf("Max: %d\n",max);
                printf("Min: %d\n",min);
                printf("\n");
            }
            else
            {
                end = clock();
                double totalTime = ((double) (end - start))/CLOCKS_PER_SEC;
                printf("The total execution time is: %f s\n", totalTime);
                exit(0);
            }
		
        }		
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return NULL;
}


void * queue_client(void * args) {
	
    char * endStr = "-1";
    mqd_t mq;
    char buffer[MAX_SIZE];
    FILE* fp;
    /* open the mail queue */
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if ((fp = fopen("UnsortedData.txt","r")) == NULL)
    { 
    	printf("Cannot Open The File!\n");

    }
	int count = 0;
    printf("Client Process: Start Sending Data... \n");
    printf("\n");
    while(fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        mq_send(mq, buffer, MAX_SIZE, 0);
        buffer[strlen(buffer) - 1] = '\0'; /*Eat \n for reading convenience*/
    }
    /* Send ending string */
    mq_send(mq, endStr, MAX_SIZE, 0);
    fclose(fp);
    mq_close(mq);
    
    return NULL;
}


int main(int argc, char** argv) {

	pthread_t client, server;
	
	printf("Start Analyzing...\n");
	
	pthread_create(&server, NULL, &queue_server, NULL);
	pthread_create(&client, NULL, &queue_client, NULL);
	
	pthread_join(server, NULL);
	pthread_join(client, NULL);
	
	printf("Done...\n");
	
	return (EXIT_SUCCESS);
}
