/*
 * references
 *  - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *
 *
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <csignal>
#include <exception>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <chrono>
#include <thread>

#define MAX 512
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX] = {'H', 'E', 'L', 'L', 'O', '\n'};
    int n;
    char count = 0;
    for (;;) {
        auto b = std::chrono::high_resolution_clock::now();
        //bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
//        while ((buff[n++] = getchar()) != '\n') {
//            printf("<%c>\n", buff[n-1]);
//        }
        buff[MAX-1] = count;
        count++;
        write(sockfd, buff, sizeof(buff));
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        bzero(buff, sizeof(buff));
        if (read(sockfd, buff, sizeof(buff)) <= 0) {
            printf("Server closed\n");
            break;
        }
        printf("From Server : %s(%d)", buff, buff[MAX-1]);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        auto e = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(e - b).count();
        if (elapsed > 30.0) printf(" - %f mmilliseconds\n", elapsed);
    }
}

class InterruptException : public std::exception
{
public:
    InterruptException(int s) : S(s) {}
    int S;
};

void signal_handler(int s)
{
    throw InterruptException(s);
}

int main(int argc, char *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // init signal handler
    std::signal(SIGINT, signal_handler);

    // reference
    //  - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    const char *addr_default = "127.0.0.1";
    char *addr_value = (char *)addr_default;
    int c;
    opterr = 0;

    while ((c = getopt (argc, argv, "i:")) != -1)
        switch (c)
        {
            case 'i':
                addr_value = optarg;
                break;
            case '?':
                if (optopt == 'i')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort ();
        }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr_value);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    //get host name
    char hostname[16];
    memset(hostname, 0x0, sizeof(hostname));
    gethostname(hostname, sizeof(hostname));
    printf("Device Name (=hostname): %s\n", hostname);

    // reference
    //  - https://stackoverflow.com/questions/49335001/get-local-ip-address-in-c
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    int err = getsockname(sockfd, (struct sockaddr*)&name, &namelen);

    char buffer[INET_ADDRSTRLEN];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, INET_ADDRSTRLEN);
    if(p != nullptr) {
        printf("Local IP address is: %s\n", buffer);
    } else {
        fprintf(stderr, "Error number: %s(%d)\n", strerror(errno), errno);
    }

    // reference
    //  * https://stackoverflow.com/questions/46484240/getpeername-from-listeningserver-socket
    struct sockaddr_in addr1;
    struct sockaddr_in addr2;

    int clnt_sock_err, clnt_peer_err;
    socklen_t serv_len = sizeof(addr1);
    clnt_peer_err = getpeername(sockfd, (struct sockaddr *)&addr1, &serv_len);
    clnt_sock_err = getsockname(sockfd, (struct sockaddr *)&addr2, &serv_len);

    printf("Client socket's ip : %s\n", inet_ntoa(addr2.sin_addr));
    printf("client socket's port %d\n", ntohs(addr2.sin_port));
    printf("Client socket's peer ip : %s\n", inet_ntoa(addr1.sin_addr));
    printf("Client socket's peer port : %d\n", ntohs(addr1.sin_port));

    printf("%d %d\n", clnt_sock_err, clnt_peer_err);

    // function for chat
    try {
        func(sockfd);
    } catch (InterruptException &e) {
        fprintf(stderr, "Terminated by Interrupt %s\n", e.what());
    }

    // close the socket
    close(sockfd);
}

