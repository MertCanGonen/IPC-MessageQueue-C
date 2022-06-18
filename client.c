//Mert Can Gönen
//181101039

#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

//message structure
struct msg{
    long type_msg;
    int id;
    int array[1000];
} message;

int main(){

    key_t key;
    int msgid;

    //connection with the main thread
    key = 1111;
    msgid = msgget(key, 0666 | IPC_CREAT); 

    //sending pid in the message struct to get thread and create message queue with the using pid as queue key
    int pid = getpid();
    message.type_msg = 1;
    message.id = pid;
    msgsnd(msgid, &message, sizeof(message), 0);

    //connection with the worker thread and start to send arrays
    key = pid;
    msgid = msgget(key, 0666 | IPC_CREAT);
    while (1){

        //create random array
        int arr[1000];
        int i;
        for (i=0;i<1000;i++){
            message.array[i] = rand() % 1000;
        }

        //print unsorted array
        printf("\nUnsorted Array: ");
        for(i=0;i<1000;i++){
            printf("%d ",message.array[i]);
        }
        printf("\n\n");

        //send the array with message to worker thread
        msgsnd(msgid, &message, sizeof(message), 0);

        //get that array is sorted message from worker thread
        if (msgrcv(msgid, &message, sizeof(message), 1, 0)) {
            
            //produce file name to read
            char name[50];
            char ext[] = ".txt";
            sprintf(name, "%d", pid);
            strcat(name, ext);
          
            //read and print sorted array from file that worker thread wrote
            int arr[1000];
            int i;
            FILE *f ;
            printf("Sorted Array: ");
            if ((f = fopen(name, "r")) != NULL){
                for (i=0;i<1000;i++){
                    fscanf(f, "%d", &arr[i]);
                    printf("%d ", arr[i]);
                }
                printf("\n");
            }
        }
        sleep(10);
    }
}