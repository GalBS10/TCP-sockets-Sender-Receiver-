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

int write_file1(int socket){// func that allows us to write in files
    FILE *f;
    char *filename= "Receiver_massege.txt";
    char buffer[SIZE/2];

    f = fopen(filename,"w");
    if(f==NULL){
        perror("-Creating file error");
        exit(1);
    }
    while(1){
        if(recv(socket, buffer,SIZE/2,0)<=0){
            break;
        }
        fprintf(f,"%s",buffer);
        bzero(buffer, SIZE/2);//like memset- delete the first n characters in thr String.
    }

    return 0;
}

int write_file2(int socket,FILE *fp){// func that allows us to write in files
    char *filename= "Receiver_massege.txt";
    char buffer[SIZE/2];

    fp = fopen(filename,"w");
    if(fp==NULL){
        perror("-Creating file error");
        exit(1);
    }
    while(1){
        if(recv(socket, buffer,SIZE/2,0)<=0){
            break;
        }
        fprintf(fp,"%s",buffer);
        bzero(buffer, SIZE/2);//like memset- delete the first n characters in thr String.
    }

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

    printf("amount of bit sended by send is %ld.\n",send(client_socket, Receiver_massege,sizeof(Receiver_massege),0));
    write_file1(client_socket);
    printf("-writing data in the txt file (first).\n");


    FILE *f;
    f = fopen("Receiver_massege.txt","r");
    fseek(f,0,SEEK_END);

    write_file2(client_socket,f);
    printf("-writing data in the txt file (second).\n");

    close(receiver_socket);
    printf("-closing..\n");




















   // int yes=1; 
    //if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1){ 
	  //  perror("setsockopt"); 
	    //exit(1);
          //      }
   // char Sender_massege[256];
    //recv(receiver_socket,&Sender_massege,sizeof(Sender_massege),0);
    

}