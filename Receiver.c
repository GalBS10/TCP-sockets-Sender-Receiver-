#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#include <time.h>
#define SIZE 1048576*2
#define xor "1100010100100100" 

int write_file1(int socket,char *data){// func that allows us to write in files
    FILE *f;//pointer type FILE
    char *filename= "Receiver_massege.txt";//the of the file that we want.
    f = fopen(filename,"w");//fopen with those pramaters allows us to accese the file we want with permition to write.
    if(f==NULL){//if we didn't read the file right.
        perror("-Creating file error");//then error.
        exit(1);
    }
    size_t size=0;//a paramater that tell us how many bits we got from the Sender (by recv()).
    size_t sum=0;//a parameter that allows us to know if all the bits of the first half make it.
                                            //(by adding size to sum in every run in the loop)
    while(sum<SIZE/2){//while amount of bits we got < the amount of bits there are in half of the file.
        size=recv(socket,data,SIZE/2,0);
        if(size == -1)
        {
            perror("error in write file 1");
            exit(1);
        }
        else if(size == 0){
            return 1;
        }
        else{}

        sum=sum+size;
        fprintf(f,"%s",data);//copy what we got from the file into data
    }
    bzero(data, SIZE/2);//like memset- delete the first n characters in thr String.
    printf("sum=%ld\n",sum);

    return 0;
}

int write_file2(int socket,char *data){// same as write_file1.
    FILE *f;
    char *filename= "Receiver_massege.txt";

    f = fopen(filename,"w");
    if(f==NULL){
        perror("-Creating file error");
        exit(1);
    }
    size_t size=0;
    size_t sum=0;
    while(sum<SIZE/2){
        if((size=recv(socket,&data[SIZE/2],SIZE/2,0))<=0)
        {
            break;
        }
        sum=sum+size;
        fprintf(f,"%s",&data[SIZE/2]);
    }
    bzero(data, SIZE);//like memset- delete the first n characters in thr String.
    printf("sum=%ld\n",sum);

    return 0;
}

int main(){
    char Receiver_massege[33] = xor;//the authoratative we want to send.
    clock_t start,end;//intialzie time parameters.
    double cpu_time_used;
    double Time1[50];//the array we use to keep the times for receiving the first half ot the file.
    int time_counter1 = 0;//this counter rins on the Time1 - array.
    double Time2[50];//the array we use to keep the times for receiving the second half ot the file.
    int time_counter2 = 0;//this counter rins on the Time2 - array.

//creating a socket
    int receiver_socket;
    receiver_socket = socket(AF_INET,SOCK_STREAM,0);
    if(receiver_socket==-1){
        printf("-there is a problem with initializing receiver\n");
    }
    else{
        printf("-initialize successfully.\n");
    }
//--------------------------------------------------------------------------------
//initialize where to send
    struct sockaddr_in Sender_address,new_addr;
    Sender_address.sin_family=AF_INET;
    Sender_address.sin_port=htons(9999);
    Sender_address.sin_addr.s_addr=INADDR_ANY;
//---------------------------------------------------------------------------------
//connecting the Receiver and Sender
    int bindd =bind(receiver_socket,(struct sockaddr *) &Sender_address,sizeof(Sender_address));
    if(bindd==-1){
        printf("-there is a problem with bindding.\n");
    }
    else{
        printf("-bindding successfully.\n");
    }
//---------------------------------------------------------------------------------

    int sock_queue =listen(receiver_socket,2);//now it can listen to two senders in pareral.
    if(sock_queue==-1){//if there are already 2 senders.
        printf("-queue is full, can't listen.\n");
    }
    else{
        printf("-listening...\n");
    }
//initialize the socket for comunicating with the Sender.
    int client_socket;//the socket
    socklen_t addr_size=sizeof(new_addr);
    client_socket= accept(receiver_socket,(struct sockaddr*)&new_addr, &addr_size);//the func return socket discriptor of a new 
    //socket and information of the Sender like IP and Port into new_addr.
//---------------------------------------------------------------------------------
    char data[SIZE];//the arraywe use in order to copy to the file we want to save
    while(1){
        start = clock();//start counting the time.
        if(write_file1(client_socket,data) == 1){//receiving the first half of the file. if we have entered the loop
            double avg1 = 0, avg2 = 0;           //it means that the sender got 0 and closed the socket, and now we
            printf("printing the Time array - 1.\n");// are calculating and printing the avg time of each array.
            for(int i = 0; i < time_counter1; i++){
                printf("Time1[%f].\n",Time1[i]);
                avg1 = avg1 + Time1[i];
            }
            printf("the average time for receiving the first half is: %f\n\n", avg1/time_counter1);
            printf("printing the Time array - 2.\n");
            for(int i = 0; i < time_counter2; i++){
                printf("Time2[%f].\n",Time2[i]);
                avg2 = avg2 + Time2[i];
            }
            printf("the average time for receiving the second half is: %f\n", avg2/time_counter2);
            close(client_socket);
            printf("-closing..\n");
            return 0;
        }
        end = clock();
        cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
        Time1[time_counter1] = cpu_time_used;//calculating the time it took to receive the first half of the file
        time_counter1++;
        printf("-writing data in the txt file (first).\n");
    
    //if(we got all the bytes)
        printf("amount of bit sended by send is %ld.\n",send(client_socket, Receiver_massege,sizeof(Receiver_massege),0));//send authentication
        char *Reno = "reno";
        socklen_t Reno_len = strlen(Reno);
            if (setsockopt(client_socket, IPPROTO_TCP,TCP_CONGESTION,Reno,Reno_len) != 0)//the change in CC from Cubic to Reno
            {
                perror("setsockopt");
                exit(1);
            }
            else{
                printf("-CC has changed.\n");
            }
        start = clock();
        write_file2(client_socket,data);//receiving the second half of the file
        end = clock();
        cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
        Time2[time_counter2] = cpu_time_used;//calculating the time it took to receive the second half of the file
        time_counter2++;
        printf("-writing data in the txt file (second).\n");

    }

return 0;
}