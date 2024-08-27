#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#define  LOG_TAG    "NDK_SERVER"
#define  LOGUNK(...)  __android_log_print(ANDROID_LOG_UNKNOWN,LOG_TAG,__VA_ARGS__)
#define  LOGDEF(...)  __android_log_print(ANDROID_LOG_DEFAULT,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_FATAL_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_SILENT_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_crazinglab_androidnetwork_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

static pthread_t app_thread;
static void *server_thread(void *userdata);
extern "C" JNIEXPORT void JNICALL
Java_com_crazinglab_androidnetwork_MainActivity_initThread(
        JNIEnv* env,
        jobject /* this */) {

    LOGI("initThread(...)\n");
    pthread_create(&app_thread, nullptr, server_thread, nullptr);
}

/*
 * references
 *  - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *
 *
 */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <chrono>
#include <thread>

#define MAX 512
#define PORT 8080

// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        if (read(connfd, buff, sizeof(buff)) <= 0) {
            LOGI("Connection Cloased");
            break;
        }
        // print buffer which contains the client contents
        LOGI("From client: %s\t To client : ", buff);
        //bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        //while ((buff[n++] = getchar()) != '\n')
        //    ;
        //sprintf(buff, "test message from server\n");

        // and send that buffer to client
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        write(connfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            LOGI("Server Exit...\n");
            break;
        }
    }
}

// Driver function
static void *server_thread(void *userdata)
{
    int sockfd = -1, connfd = -1;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    //get host name
    char hostname[16];
    memset(hostname, 0x0, sizeof(hostname));
    gethostname(hostname, sizeof(hostname));
    LOGI("Device Name (=hostname): %s\n", hostname);

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        LOGE("socket creation failed...\n");
        return NULL;
    }
    else
        LOGI("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        LOGE("socket bind failed...\n");
        return nullptr;
    }
    else
        LOGI("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        LOGE("Listen failed...\n");
        return nullptr;
    }
    else
        LOGI("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (connfd < 0) {
        LOGE("server accept failed...\n");
        return nullptr;
    }
    else
        LOGI("server accept the client...\n");

    // reference
    //   * https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
    //   * http://man7.org/linux/man-pages/man3/inet_ntop.3.html
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&cli;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    char str[INET_ADDRSTRLEN];
    const char *p = inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    if (p != nullptr) {
        LOGI("  accepted from (addr=%s, port=%d)\n", str, ntohs(pV4Addr->sin_port)) ;
    } else {
        LOGE( "Error number: %s(%d)\n", strerror(errno), errno);
    }

    // reference
    //  * https://stackoverflow.com/questions/46484240/getpeername-from-listeningserver-socket
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = clnt_addr_size=sizeof(clnt_addr);

    struct sockaddr_in addr1;
    struct sockaddr_in addr2;

    struct sockaddr_in addr3;
    struct sockaddr_in addr4;

    socklen_t serv_len = sizeof(serv_addr);
    int serv_peer_err = getpeername(sockfd, (struct sockaddr *)&addr1, &serv_len);
    int serv_sock_err = getsockname(sockfd, (struct sockaddr *)&addr3, &serv_len);

    LOGI("Server socket's peer ip : %s\n", inet_ntoa(addr1.sin_addr));
    LOGI("Server socket's peer port : %d\n", ntohs(addr1.sin_port));
    LOGI("Server socket's ip : %s\n", inet_ntoa(addr3.sin_addr));
    LOGI("Server socket's port : %d\n", ntohs(addr3.sin_port));
    LOGI("\n\n\n\n\n");

    int clnt_peer_err = getpeername(connfd, (struct sockaddr *)&addr2, &clnt_addr_size);
    int clnt_sock_err = getsockname(connfd, (struct sockaddr *)&addr4, &clnt_addr_size);

    LOGI("Client socket's peer ip : %s\n", inet_ntoa(addr2.sin_addr));
    LOGI("client socket's peer port %d\n", ntohs(addr2.sin_port));
    LOGI("Client socket's ip : %s\n", inet_ntoa(addr4.sin_addr));
    LOGI("Client socket's port : %d\n", ntohs(addr4.sin_port));


    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    if (connfd > 0) close(connfd);
    if (sockfd > 0) close(sockfd);
    return nullptr;
}
