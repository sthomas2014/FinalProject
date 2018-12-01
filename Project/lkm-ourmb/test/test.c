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



int main(void)
{
    FILE *input  = fopen("UnsortedData.txt", "r"); // input fd
    FILE *output = fopen("output.txt", "w");   // output fd
    if (input == NULL || output == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    
    const char * mailboxID = "MAILBOX_1";
    pid_t procID = getpid();
    
    clock_t start, end;
    
    int lineLen = 32;   // Max number of characters to be read per line.
                        // Excess of this amount gets truncated.
    
    int min = 2000;
    int max = 0;
    int data;
    
    char sendBuff[lineLen]; //Max size of line to read
    char recvBuff[4096];    //Size of one kernel page
    
    int * bytesCopiedFrom = 0;
    int * bytesCopiedTo = 0;

    // ** Subscribe readers. Verify using 'dmesg' to look at data dump
    // 1st sender process
    // create mailbox in kernel space & register sender on access list    
    if (ourmb_open(mailboxID, procID, lineLen, __FLAG_OPEN_SEND) != 0)
    {
        printf("Error opening mailbox for reader1 (sender) \n");
    }
   
    // ** Verify using 'dmesg' to look at data dump
    // 1st receiver process
    // register receiver on access list for specified mailbox
    if (ourmb_open(mailboxID, procID + 1, lineLen, __FLAG_RECV) != 0)
    {
        printf("Error opening mailbox for writer (receiver) \n");
    }
    //Start timer after construction of queue
    start = clock();
    while (fgets(sendBuff, lineLen, input) != NULL)
    {
        ourmb_send(mailboxID,procID,sendBuff,lineLen,bytesCopiedTo);
        
        ourmb_recv(mailboxID,procID,recvBuff,bytesCopiedFrom);
        
        printf("Server Process: Received Data: %s", sendBuff);
		
        data = atoi(sendBuff);
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
    end = clock();
    double totalTime = ((double) (end - start))/CLOCKS_PER_SEC;
    printf("The total execution time is: %f s \n", totalTime);
    
    // Deallocate, close MAILBOX_1
    ourmb_clos(mailboxID, procID);
    ourmb_clos(mailboxID, procID+1);
    
    // close open files
    fclose(input);
    fclose(output);
        
	return 0;
}
