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
#include <getopt.h>

#include <queue>
#include <vector>

#include "config.hpp"



static struct option long_options[] = {
    {"help",                no_argument,        nullptr,  'h' },
    {"server-hostname",     required_argument,  nullptr,  's' },
    {"port",                required_argument,  nullptr,  'p' },
    {nullptr,               0,                  nullptr,  0 },
};

void print_help(void) {
    printf("usage: tcp_client [-h] [-H HOST] [-p PORT]\n\n");
    printf("This is toy tcp client to test comm latency\n\n");
    printf("options:\n");

    printf("  -h,      --help                  show this help message and exit\n");
    printf("  -s HOST, --server-hostname HOST  set server hostname\n");
    printf("  -p PORT, --port            PORT  set server port number\n");
}

void func(int sockfd)
{
    std::priority_queue<double, std::vector<double>, std::greater<double>> min_heap;

    char buff[MAX_BUF] = {'H', 'E', 'L', 'L', 'O', '\n'};
    int n;
    uint64_t count = 0;
    auto b = std::chrono::high_resolution_clock::now();
    for (;;) {
        auto b_frame = std::chrono::high_resolution_clock::now();
        //bzero(buff, sizeof(buff));
        //printf("Enter the string : ");
        n = 0;
//        while ((buff[n++] = getchar()) != '\n') {
//            printf("<%c>\n", buff[n-1]);
//        }
        // buff[MAX-1] = count;
        count++;
        write(sockfd, buff, sizeof(buff));
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        bzero(buff, sizeof(buff));
        if (read(sockfd, buff, sizeof(buff)) <= 0) {
            printf("Server closed\n");
            break;
        }
        // printf("From Server : %s(%d)", buff, buff[MAX-1]);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        auto e_frame = std::chrono::high_resolution_clock::now();
        double elapsed_frame = std::chrono::duration<double, std::milli>(e_frame - b_frame).count();

        if (min_heap.size() < 10) {
            min_heap.push(elapsed_frame);
        } else {
            if (elapsed_frame > min_heap.top()) {
                min_heap.pop();
                min_heap.push(elapsed_frame);
            }
        }

        if (elapsed_frame > 30.0) printf(" - %f mmilliseconds\n", elapsed_frame);

        auto e = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(e - b).count();
        if (elapsed > PERF_MONITOR_INTERVAL) {
            float fps = 1000.0*count/elapsed;
            printf("\t>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            printf("\t    fps: %6f hz\telapsed= %9f milliseconds/ %ld\n", fps, elapsed, count);
            printf("\t    average elapsed time per frame: %3.3f millisecs\n", elapsed/count);
            printf("\t    [");
            while (!min_heap.empty()) {
                printf("%.3f, ", min_heap.top());
                min_heap.pop();
            }
            printf("]\n");
            printf("\t>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            //reset
            b = std::chrono::high_resolution_clock::now();
            count = 0;
        }
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

    //
    // reference
    //  - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    //  - https://man7.org/linux/man-pages/man3/getopt.3.html
    //
    const char *server_hostname_default = "127.0.0.1";
    char *server_hostname = (char *)server_hostname_default;
    int port = PORT;
    int c;
    opterr = 0;

    int option_index = 0;
    while ((c = getopt_long (argc, argv, "hs:p:", long_options, &option_index)) != -1)
        switch (c)
        {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                       printf(" with arg %s", optarg);
                printf("\n");
                break;
            case 'h':
                print_help();
                return 0;
            case 's':
                server_hostname = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 'r')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option or no argument `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                printf("return=%c(%x)\n", c, c);
                abort ();
        }

    // printf("optind=%d\n", optind);

    printf("\tserver_hostname: %s\n", server_hostname);
    printf("\tport           : %d\n", port);

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
    servaddr.sin_addr.s_addr = inet_addr(server_hostname);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
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

    // printf("client socket error=%d peer_error=%d\n", clnt_sock_err, clnt_peer_err);

    // function for chat
    try {
        func(sockfd);
    } catch (InterruptException &e) {
        fprintf(stderr, "Terminated by Interrupt %s\n", e.what());
    }

    // close the socket
    close(sockfd);
}

