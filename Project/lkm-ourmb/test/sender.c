//sender testing
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
    FILE *input  = fopen("input.txt", "r"); // input fd

    if (input == NULL || output == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    
    const char * mailboxID = "MAILBOX_1";
    pid_t procID = getpid();
    
    int lineLen = 32;  // Number of characters to be read per line.
                        // Excess of this amount gets truncated.
                        
    size_t sizeOfSendBuff = __KERNBUFF_MAX_BYTES; //4096 bytes
    char * sendBuff[sizeOfSendBuff];
    int * bytesCopiedFrom = 0;
    
    
    // ** Subscribe reader(senders). Verify using 'dmesg' to look at data dump
    // 1st sender process
    // Try to create mailbox in kernel space & register as sender on access list    
    if ( ourmb_open(mailboxID, procID, lineLen, __FLAG_OPEN_SEND) != 0)
    {
        printf("Error opening mailbox for reader1 (sender) \n");
        return -1;
    }

    
    while (fgets(sendBuff, lineLen, input) != NULL)
    {
        
        ourmb_send(mailboxID, procID, sendBuff, lineLen, bytesCopiedTo);
        printf("Process: %i sent message: %s\n", sendBuff);
        
    }
    
    
    printf("Number of lines read %i\n", linesRead);
    
    // Unsubscribe from MAILBOX_1
    ourmb_clos(mailboxID, procID);
    
    // close open files
    fclose(input);
        
	return 0;
}

