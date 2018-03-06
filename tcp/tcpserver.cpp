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
#include <errno.h>

using namespace std;
using std::string;
using namespace std;
const int BUF_LEN = 512;

int main(int argc, char **argv)  
{
    char* serverIP;
    unsigned short bindPort = 0;
    if(argc > 2)
    {
        serverIP = argv[1];
        bindPort = atoi(argv[2]);
    }

    printf("TcpServer serverIP is %s,bindPort is %d\n",serverIP,bindPort);
    /*
　　TCP编程的服务器端一般步骤是： 
　　1、创建一个socket，用函数socket()； 
　　2、设置socket属性，用函数setsockopt(); * 可选 
　　3、绑定IP地址、端口等信息到socket上，用函数bind(); 
　　4、开启监听，用函数listen()； 
　　5、接收客户端上来的连接，用函数accept()； 
　　6、收发数据，用函数send()和recv()，或者read()和write(); 
　　7、关闭网络连接； 
　　8、关闭监听
    */
    int sock;
    if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("TcpServer creat socket fail %d(errno %d)\n",sock,errno);
        return  -1;
    }

    struct sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(bindPort);
    localAddr.sin_addr.s_addr = ::inet_addr(serverIP);
    //localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (localAddr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("TcpServer Incorrect ip address!\n");
        close(sock);
        return -1;
    }
	int flags = ::fcntl(sock, F_GETFL);
	if((flags & O_NONBLOCK) == 0)
	{
        if (::fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
		{
			printf("TcpServer set notblock fail\n");
	    }
    }
    if(bind(sock,(struct sockaddr *)&localAddr,sizeof(localAddr)) < 0)
    {
        printf("TcpServer bind port(%d) fail \n",bindPort);
        close(sock);
        return -1;
    }
    if((::listen(sock,1024)) < 0)
    {
        printf("TCPSocket listen fail\n");
        close(sock);
        return -1;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
	int clientSock = -1;

    while (1)
    {
        if(clientSock < 0  && (clientSock = accept(sock,(struct sockaddr *)&clientAddr,&clientAddrLen)) == 0)
        {
            printf("TcpServer accept from %s:%d \n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
        }
        char rcvbuff[512] = {0};
        int n = 0;
        int recvlen = 0;
        while(clientSock >=0 &&(n = read(clientSock, &rcvbuff[recvlen], (BUF_LEN - recvlen) ) >= 0 )) 
        {
            recvlen += n;
            if( n >= 0 )
            {
                printf("TcpServer recv(%s:%d) %s\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port),rcvbuff);
                break;
            }
        }

        write(clientSock,"hello tcpserver send",strlen("hello tcpserver send"));
        sleep(1);
    }
    close(clientSock);
    return 0;
}  
