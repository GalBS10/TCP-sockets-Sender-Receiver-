#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#define SIZE 1048576*2 //for 2MB
#define xor "1100010100100100"       //   5    2    3    0
                                     // 0101 0010 0011 0000
                                     //xor
                                     //   9    7    1    4
                                     // 1001 0111 0001 0100
                                     //----------------------
                                     // 1100 0101 0010 0100

    int send_file(char *data, int sender_socket){//a func that we send the file to the sender.
        size_t a;
        if((a=send(sender_socket,data,SIZE/2,0))==-1){//if send returned -1 there is an error.
            perror("error in sending data.\n");
            exit(1);
        }
        bzero(data,SIZE/2);//like memset, it deletes the first SIZE/2 bits
        printf("The amount of bit we send (first half) = %ld\n",a);
        return 0;
    }

    int send_file2(char *data, int sender_socket){//same as the send_file1 function
        size_t b;
        if((b=send(sender_socket,&data[SIZE/2],SIZE/2,0))==-1){
            perror("error in sending data.\n");
            exit(1);
        }
        bzero(data,SIZE/2);
        printf("The amount of bit we send (second half) = %ld\n",b);
        return 0;
    }


int main(){
    //creating a socket
    int sender_socket;
    sender_socket=socket(AF_INET, SOCK_STREAM, 0);//because we are in linux the default cc is cubic.
    if(sender_socket==-1){
        printf("there is a problem with initializing sender.\n");
    }
    else{
        printf("-initialize successfully.\n");
    }
//--------------------------------------------------------------------------------
//initialize where to send
    struct sockaddr_in Receiver_address;//initialize where to send
    FILE *fp;
    char *filename ="Sender_massege.txt";//the file we want to send

    Receiver_address.sin_family = AF_INET;// setting for IPV4
    Receiver_address.sin_port = htons(9999);//port is 9999
    Receiver_address.sin_addr.s_addr = INADDR_ANY;//listening to all (like 0.0.0.0)
//---------------------------------------------------------------------------------
//connecting the Sender and Receiver
    int connection_status = connect(sender_socket,(struct sockaddr *) &Receiver_address,sizeof(Receiver_address));
    if(connection_status==-1){
        printf("there is an error with the connection.\n");
    }
    else{
        printf("-connected.\n");
    }
//---------------------------------------------------------------------------------
    fp = fopen(filename, "r");//fopen with those pramaters allows us to accese the file we want with permition to read only.
    if(fp==NULL){//if we didn't read the file right.
        perror("Error in reading file.");//then error.
        exit(1);
    }

    char data [SIZE]={0};//The array we want to copy the file into and then send to the receiver.
    printf("fread = %ld\n",fread(data,sizeof(char),SIZE,fp));//copying the file into the array.

    int decision=1;//the condition for the loop.
    while(decision!=0){//start of loop.

        if(send_file(data,sender_socket)==0){//sendind the first half to the Receiver.
            printf("-File data has been send successfully1.\n");
        }
        char server_response[33];
        recv(sender_socket,&server_response, sizeof(server_response),0);//getting the authoratative from the Receiver.
        printf("The server sent the data: %s .\n", server_response);
        if(!strcmp(xor,server_response))//if the Receiver send the right authoratative.
        {
            char *Reno = "reno";//type of CC.
            socklen_t Reno_len = strlen(Reno);
            if (setsockopt(sender_socket, IPPROTO_TCP,TCP_CONGESTION,Reno,Reno_len) != 0)//the change in CC from Cubic to Reno.
            {
                perror("setsockopt");
                exit(1);
            }
            else{
                printf("-CC has changed Cubic -> Reno.\n");
            }
            if(send_file2(data,sender_socket)==0){//sendind the second half to the Receiver.
             printf("-File data has been send successfully2.\n");
            }
        }
        else{//if the xor we got doesnt match, it means that somthing is wrong with the firdt half.
            perror("-The xor didn't make it.\n");
            exit(1);
        }
        bzero(server_response,33);//like memset it deletes the first 33 bits from server_response.
        scanf("%d",&decision);
        if(decision==0){//we don't want to keep sending the file again. 
            close(sender_socket);
            printf("-closing...\n");
            break;
        }

        char *Cubic = "cubic";//type of CC.
        socklen_t Cubic_len = strlen(Cubic);
        if (setsockopt(sender_socket, IPPROTO_TCP,TCP_CONGESTION,Cubic,Cubic_len) != 0)//the change in CC from Reno to Cubic
        {
            perror("setsockopt");
            exit(1);
        }
        else{
            printf("-CC has changed Reno -> Cubic.\n");
        }

    }

    return 0;
}