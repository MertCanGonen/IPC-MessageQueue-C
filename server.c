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

//worker thread's function
void* thread_function(void * arg){

    //get contact with process which it wants to get sorted array
    key_t key = message.id;
    int msgid = msgget(key, 0666 | IPC_CREAT);

    //keep working as the messages arrives
    while (1){
        if (msgrcv(msgid, &message, sizeof(message), 1, 0)){

            //produce file name to write
            char name[50];
            char ext[] = ".txt";
            sprintf(name, "%d", message.id);
            strcat(name, ext);

            //get the array from message
            int arr[1000];
            int i, j;
            for(i=0;i<1000;i++){
                arr[i] = message.array[i];
            }

            //sort the array
            for(i=0;i<1000;i++){
                for (j=i+1;j<1000;j++){
                    if (arr[i] > arr[j]){
                        int tmp = arr[i];
                        arr[i] = arr[j];
                        arr[j] = tmp;
                    }
                }
            }

            //open to file and write the array
            FILE *f = fopen(name, "w");
            for(i=0;i<1000;i++){
                fprintf(f, "%d ",arr[i]);
            }
            fclose(f);

            //send the message that array is sorted and written on file
            msgsnd(msgid, &message, sizeof(message), 0);
        }
    }
}

int main(){

    key_t key;
    int msgid;

    key = 1111;
    msgid = msgget(key, 0666 | IPC_CREAT);
    pthread_t thread;
    
    while (1){
        printf("Server started waiting...\n");
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("Server is connected with Process %d\n", message.id);
        pthread_create(&thread, NULL, thread_function, NULL);
    }
    
}