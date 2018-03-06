#include <sys/types.h>  
#include <sys/socket.h>  
#include<pthread.h>  
#include <netinet/in.h>  
#include <stdio.h>  
#include <string.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
using namespace std;
using std::string;
using namespace std;

const int BUF_LEN = 512;
int main(int argc, char **argv)  
{
    char* serverIp;
    unsigned short serverPort = 0;
    if(argc > 2)
    {
        unsigned char buf[sizeof(struct in6_addr)] = {0};
        if(1 == inet_pton(AF_INET, argv[1], buf))
        {
            serverIp = argv[1];
            serverPort = atoi(argv[2]);
        }
        else{
            printf("TCPClient inpurt serverip or serverport error,exit");
            return  -1;
        }
    }
    string  sendMsg = "tcpclient send message";
    printf("TCPClient start, serverIp is %s  serverPort is %d \n",serverIp,serverPort);

    /*
　　TCP客户端一般步骤是： 
　　1、创建一个socket，用函数socket()； 
　　2、设置socket属性，用函数setsockopt();* 可选 
　　3、绑定IP地址、端口等信息到socket上，用函数bind();* 可选 
　　4、设置要连接的对方的IP地址和端口等属性； 
　　5、连接服务器，用函数connect()； 
　　6、收发数据，用函数send()和recv()，或者read()和write(); 
　　7、关闭网络连接；
    */
    int cTcpSock;  
    if ((cTcpSock=socket(AF_INET, SOCK_STREAM, 0)) < 0)  
    {  
        printf("TCPClient socket() fail\n");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);  
    serverAddr.sin_addr.s_addr = ::inet_addr(serverIp);  

    /* set socket option */
    int keepAlive = 1;
    if (::setsockopt(cTcpSock, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive)) != 0)
    {
        printf("TCPClient setsockopt fail\n");
        //return -1;
    }

    if(connect(cTcpSock,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) != 0)
    {
        close(cTcpSock);
        printf("TCPClient connect fail\n");
        return -1;
    }

    while(1)
    {
        write(cTcpSock,"hello client send",strlen("hello client send"));
#if 1
        char rcvbuff[BUF_LEN] = {0};
        int n = 0;
        int recvlen = 0;
        while((n = read(cTcpSock, &rcvbuff[recvlen], (BUF_LEN - recvlen) ) >= 0 )) 
        {
            recvlen += n;
            if( n >= 0 )
            {
                printf("client recv(%s:%d) %s\n",inet_ntoa(serverAddr.sin_addr),ntohs(serverAddr.sin_port),rcvbuff);
                break;
            }
        }
#endif
        sleep(1);
    }
    close(cTcpSock);
    return 0;  
}  
