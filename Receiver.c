#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#define SIZE 1048576*2
#define xor "1100010100100100" 

int write_file1(int socket,char *data){// func that allows us to write in files
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
        if((size=recv(socket,data,SIZE/2,0))<=0)
        {
            break;
        }
        sum=sum+size;
        fprintf(f,"%s",data);
    }
        bzero(data, SIZE/2);//like memset- delete the first n characters in thr String.
        printf("sum=%ld\n",sum);

    return 0;
}

int write_file2(int socket,char *data){// func that allows us to write in files
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
   char Receiver_massege[33] = xor;
    
    int receiver_socket;
    receiver_socket = socket(AF_INET,SOCK_STREAM,0);
    if(receiver_socket==-1){
        printf("-there is a problem with initializing receiver\n");
    }else{
    printf("-initialize successfully.\n");
    }
    struct sockaddr_in Sender_address,new_addr;
    Sender_address.sin_family=AF_INET;
    Sender_address.sin_port=htons(9999);
    Sender_address.sin_addr.s_addr=INADDR_ANY;

    int bindd =bind(receiver_socket,(struct sockaddr *) &Sender_address,sizeof(Sender_address));
    if(bindd==-1){
        printf("-there is a problem with bindding.\n");
    }
    else{
    printf("-bindding successfully.\n");
    }
    int sock_queue =listen(receiver_socket,2);
    if(sock_queue==-1){
        printf("-queue is full, can't listen.\n");
    }
    else{
        printf("-listening...\n");
    }
    int client_socket;
    socklen_t addr_size=sizeof(new_addr);
    client_socket= accept(receiver_socket,(struct sockaddr*)&new_addr, &addr_size);
    while(1){
    char data[SIZE];

    write_file1(client_socket,data);
    printf("-writing data in the txt file (first).\n");
    
    //if(we got all the bytes)
    printf("amount of bit sended by send is %ld.\n",send(client_socket, Receiver_massege,sizeof(Receiver_massege),0));

    write_file2(client_socket,data);
    printf("-writing data in the txt file (second).\n");
    }
    close(receiver_socket);
    printf("-closing..\n");

return 0;
}