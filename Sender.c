#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#define SIZE 1048576

int send_file(FILE *fp, int sender_socket){

    char data [SIZE]={0};//We have has been asked for 1MB file size. Therefore, we need 1024KB and 1KB=1024B. Furtheremore each char is 8B. 
    size_t position = ftell(fp);//current position
    fseek(fp,0,SEEK_END);//Go to end
    size_t length = ftell(fp);//the position is the size
    fseek(fp,position,SEEK_SET);//restore original position


    while (fgets(data, length ,fp))//can be change to SIZE.
    {
        if(send(sender_socket,data,sizeof(data),0)==-1){
            perror("error in sending data.\n");
            exit(1);
        }
        if(send(sender_socket,&data[(length/2)+1],sizeof(data),0)==-1){
            perror("error in sending data.\n");
            exit(1);
        }
       bzero(data,SIZE);
       
    }
    //printf("-%d \n",count);
   // bzero(data,length/2);
   // while (fgets(data, length ,fp))
    //{
      //  bzero(data,SIZE);
   // }
    
    return 0;
}

int main(){
    //creating a socket
    int sender_socket;
    sender_socket=socket(AF_INET, SOCK_STREAM, 0);
    if(sender_socket==-1){
        printf("there is a problem with initializing sender.\n");
    }
    else{
        printf("-initialize successfully.\n");
    }

struct sockaddr_in Receiver_address;
FILE *fp;
char *filename ="Sender_massege.txt";

Receiver_address.sin_family = AF_INET;
Receiver_address.sin_port = htons(9999);
Receiver_address.sin_addr.s_addr = INADDR_ANY;

int connection_status = connect(sender_socket,(struct sockaddr *) &Receiver_address,sizeof(Receiver_address));
if(connection_status==-1){
    printf("there is an error with the connection.\n");
}
else{
printf("-connected.\n");
}
//char server_response[256];
//recv(sender_socket,&server_response, sizeof(server_response),0);

//printf("The server sent the data: %s .\n", server_response);

fp = fopen(filename, "r");
if(fp==NULL){
    perror("Error in reading file.");
    exit(1);
}


if(send_file(fp,sender_socket)==0){
printf("-File data has been send successfully.\n");
}

close(sender_socket);
printf("-closing...\n");

return 0;
}





























/*int main(){

struct in_addr{
    size_t big_endian;
};

Receiver_address.sin_family=AF_INET;
Receiver_address.sin_port=htons(AF_INET);



int socket(int AF,int type , int protocol);//kind of constructor for the socket

int sender_socket;
sender_socket= socket(AF_INET, SOCK_STREAM, 0);//generate new socket named sockfd
#ifdef sockfd<=0 
printf("somthing went wrong with initializing...");
#endif

int connect (int sender_socket, const struct sockaddr *serv_addr, socklen_t addrlen);//a func that connect between the sender and the reciever
int connection_status = 
#ifdef connection_status<=0
printf("there is a problem with the connection");
#endif
void memset(void *str,int c, size_t n);//delete the n first charactars and replace them in c

struct sockaddr_in sa;
inet_pton(int af, , *int);//this func cast the addr into binary represent. bad={-1,0} good={k>0}.
return 0;
}*/