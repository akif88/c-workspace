#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"

#define PORT 7890 

int main(int argc, char const *argv[])
{
        int sockfd, new_sockfd; // Listen on sock_fd, new connection on new_fd
        struct sockaddr_in host_addr, client_addr;
        socklen_t sin_size;
        int recv_length=0, yes=1;
        char buffer[1024];


        if((sockfd = socket(PF_INET, SOCK_STREAM, 0))==-1)
            fatal("error create new socket");
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
            fatal("error reuse");

        host_addr.sin_family = AF_INET;
        host_addr.sin_port = htons(PORT);
        host_addr.sin_addr.s_addr = 0;
        memset(&(host_addr.sin_zero), '\0', 8); 

        if(bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))==-1)
            fatal("error bind");
        if(listen(sockfd, 5) == -1)
            fatal("error listen");

        while(1){
            sin_size = sizeof(struct sockaddr_in);
            if((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size))==-1)
                fatal("error accept");

            printf("server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            
            send(new_sockfd, "Hello, world!", 13, 0);
            
            while ((recv_length = recv(new_sockfd, &buffer, 1024, 0)) > 0)
            {
                printf("RECV: %d bytes\n", recv_length);
                dump(buffer, recv_length);
            }
            close(new_sockfd);
        }    

    return 0;
}
