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
    socklen_t addr_size;

    struct sockaddr_in server_address;
    int server_socket;

    struct sockaddr_in client_address;
    int client_socket;

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(server_socket == -1) {
        printf("Socket Creation Failed!!\n");
        exit(0);
    } else {
        printf("Socket Creation Successful!!\n");
    }

    addr_size = sizeof(server_address);

    bzero(&server_address, addr_size);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&server_address, addr_size) != 0) {
        printf("Socket Binding Failed!!\n");
        exit(0);
    } else {
        printf("Socket Binding was Successful!!\n");
    }

    while(1>0) {
        char *msg;
        bzero(buffer, 1024);
        int count = 0;

        while(1) {
            char frame[129];
            char ACK[4] = {'A', 'C', 'K', '\n'};

            printf("Recieving a frame from Client......\n");
            bzero(frame, 129);

            int n = recvfrom(server_socket, frame, sizeof(frame), 0, (struct sockaddr*)&client_address, &addr_size);

            if(n>0) {
                printf("Frame %c has been recieved!!\n", frame[128]);

                ACK[3] = frame[128];
                sendto(server_socket, ACK, 4, 0, (struct sockaddr*)&client_address, addr_size);
                count = (int)(frame[128]) - 48;

                for(int j=0; j<128; j++) {
                    buffer[count*128 + j] = frame[j];
                }

                if(count == 7) {
                    printf("All frames have been recieved!!\n");
                    break;
                }

            } else {
                ACK[0] = 'N';
                ACK[1] = 'A';
                ACK[2] = 'C';
                ACK[3] = 'K';
                sendto(server_socket, ACK, 4, 0, (struct sockaddr*)&client_address, addr_size);

            }
        }
        
        if(strncmp("exit",buffer,4) == 0) {
            printf("The client has Closed the Communication Channel!!\n\n");
            break;
            
        }

        printf("Message From client: %s\n", buffer);

    }

    printf("Done!!\n");
    return 0;
}