/*
** test.c for lkm-ourmb
**
** Originally built by xsyann
** Contact <contact@xsyann.com>
**
** Current-1 version built by Yuan Xiao
** Contact <xiao.465@osu.edu>
**
** Current version built by Scott Thomas, Jiacheng Liu, Quinn Wu
*/


#include "ourmb_user.h"
#define MAX_SIZE    1024

void * queue_server(void * args) 
{
    clock_t start, end;
    
    char buffer[MAX_SIZE + 1];
    const char * mailboxID = "MAILBOX_1";
    int min = 2000;
    int max = 0;
    int data;
    int bytes_read;
    pid_t procID = getpid();
    
    
    /* Open mailbox */
    if (ourmb_open(mailboxID, procID, MAX_SIZE , __FLAG_RECV) != 0)
    {
        printf("Error opening mailbox for writer (receiver) \n");
 
    }
    
    /*Start timer*/
    start = clock();
    while(1) 
    {	
		ourmb_recv(mailboxID,procID,buffer,&bytes_read);
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
    
    /*Close mailbox*/
    ourmb_clos(mailboxID, procID);

    return NULL;
}


void * queue_client(void * args) {
	
    char * endStr = "-1";
    const char * mailboxID = "MAILBOX_1";
    pid_t procID = getpid();
    int bytes_read;
    char buffer[MAX_SIZE];
    FILE* fp;
    
    
    /* open mailbox */
    if (ourmb_open(mailboxID, procID, MAX_SIZE, __FLAG_OPEN_SEND) != 0)
    {
        printf("Error opening mailbox for reader1 (sender) \n");
    
    }
        
    if ((fp = fopen("UnsortedData.txt","r")) == NULL)
    { 
    	printf("Cannot Open The File!\n");

    }
	int count = 0;
    printf("Client Process: Start Sending Data... \n");
    printf("\n");
    while(fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        ourmb_send(mailboxID,procID,buffer,MAX_SIZE,&bytes_read);
        buffer[strlen(buffer) - 1] = '\0'; /*Eat \n for reading convenience*/
	
    }
    /* Send ending string */
    ourmb_send(mailboxID,procID,endStr,MAX_SIZE,&bytes_read);
    fclose(fp);
    ourmb_clos(mailboxID, procID);
    
    return NULL;
}


int main(int argc, char** argv)
{
    pthread_t client, server;
	
	printf("Start Analyzing...\n");
	
	pthread_create(&server, NULL, &queue_server, NULL);
	pthread_create(&client, NULL, &queue_client, NULL);
	
	pthread_join(server, NULL);
	pthread_join(client, NULL);
	
	printf("Done...\n");
	
	return (EXIT_SUCCESS);
    
}
    
