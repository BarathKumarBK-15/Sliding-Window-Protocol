#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 6969

int main() {
    char buffer[1024];

    int client_socket;
    struct sockaddr_in server_address;
    socklen_t addr_size = sizeof(server_address);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(client_socket == -1) {
        printf("Socket Creation Failed!!\n");
        exit(0);
    } else {
        printf("Socket Creation Successfull\n");
    }

    bzero(&server_address, addr_size);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1>0) {
        bzero(buffer, 1024);
        int n=0;

        printf("Enter Message to Server: ");
        while((buffer[n++] = getchar()) != '\n');

        for(int i=0; i<8; i++) {
            int flag = 1;

            char frame[129];
            char req[4] = {'A', 'C', 'K', '\n'};

            bzero(frame, 129);

            for(int j=0; j<128; j++) {
                frame[j] = buffer[i*128 + j];

            }
            frame[128] = (char)(i+48);
            req[3] = (char)(i+48);

            while(flag) {
                sendto(client_socket, frame, sizeof(frame), 0, (struct sockaddr*)&server_address, addr_size);
                    
                char ACK[4];
                    
                printf("Recieving Acknowledgement from Server......\n");
                    
                n = recvfrom(client_socket, ACK, sizeof(ACK), 0, (struct sockaddr*)&server_address, &addr_size);

                if(n>0 && strncmp(req,ACK,4) == 0) {
                    printf("Acknowledgement Recieved for frame %d from Server......\n", i);
                    flag = 0;
                        
                } else {
                    printf("Negative Acknowledgement or Time Out..... Resending Frame %d", i);

                }

            }

        }

        if(strncmp("exit",buffer,4) == 0) {
            printf("You have Closed the Communication Channel!!\n\n");
            break;
            
        }

    }

    close(client_socket);

    printf("Done!!\n");
    return 0;
}