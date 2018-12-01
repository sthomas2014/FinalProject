//Receiver testing
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

    FILE *output = fopen("output.txt", "w");   // output fd
    
    if (output == NULL || output == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    
    const char * mailboxID = "MAILBOX_1";
    pid_t procID = getpid();
    
    int lineLen = 32;  // Number of characters to be read per line.
                        // Excess of this amount gets truncated.
                        
    size_t sizeOfRecvBuff = __KERNBUFF_MAX_BYTES; //4096 bytes
    char  recvBuff[sizeOfRecvBuff];    
    int * bytesCopiedTo = 0;
    
    
    // ** Subscribe writer
    // ** Verify using 'dmesg' to look at data dump
    // 1st receiver process
    // register receiver on access list for specified mailbox
    if (ourmb_open(mailboxID, procID, lineLen, __FLAG_RECV) != 0)
    {
        printf("Error opening mailbox for writer (receiver) \n");
        return -1;
    }
    
    
    while (fgets(sendBuff, lineLen, input) != NULL)
    {
        linesRead++;
        
        printf("To kernelspace: %s\n", sendBuff);
        ourmb_send(mailboxID, procID, sendBuff, lineLen, bytesCopiedTo);
        
        ourmb_recv(mailboxID, procID+4, recvBuff, bytesCopiedFrom);
        printf("From kernelspace: %s\n", recvBuff);
        
        fprintf(output, "%s\n", recvBuff); 
    }
    
    
    printf("Number of lines read %i\n", linesRead);
    
    // Deallocate, close MAILBOX_1
    ourmb_clos(mailboxID, procID);
    
    // close open files
    fclose(output);
        
	return 0;
}

