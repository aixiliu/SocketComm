#include "UdpSocket.h"

bool UDPSocket::Create(void)
{
    return Create(NULL, 0);
}

bool UDPSocket::Create(int port)
{
    return Create(NULL, port);
}
bool UDPSocket::Create(const char * const localip, int port)
{
    struct sockaddr_in listenAddr;
    bzero(&listenAddr, sizeof(listenAddr));
 
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(port);
    if(NULL == localip)
    {
        listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else{
        listenAddr.sin_addr.s_addr =  inet_addr(localip);
    }
    
    if ((m_udpsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)  
    {  
        printf("UDPSocket creat fail\n");
        return false;
    }
    int flags = ::fcntl(m_udpsocket, F_GETFL);
    if (flags == -1)
    {
        return false;
    }
    if ((flags & O_NONBLOCK) == 0 && (::fcntl(m_udpsocket, F_SETFL, flags | O_NONBLOCK) == -1)) 
    {
        return false;
    }

    if (0 != port && bind(m_udpsocket, (struct sockaddr *)&listenAddr, sizeof(listenAddr)) < 0)  
    {  
        printf("bind %d error\n",port);
        return false;
    }
    return true;
}

int UDPSocket::RecvFrom(void * buf, int size, sockaddr_in & addr)
{
    struct sockaddr_in remoteAddr;
    socklen_t addlen = sizeof(remoteAddr);
	memset(buf,0,size);
	int n =  recvfrom(m_udpsocket, buf, size, 0, (struct sockaddr*)&addr,&addlen);
    //printf("%s %u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        return n;
}

int UDPSocket::RecvFrom(void * buf, int size, string & ip, int & port)
{
    struct sockaddr_in addr;
	memset(buf,0,size);
	int n = RecvFrom((void *)buf, size,addr);
    ip = inet_ntoa(addr.sin_addr);
    port  = ntohs(addr.sin_port);
    return n;
}

int UDPSocket::SendTo(void * buf, int size, const sockaddr_in & addr)
{
    int n = sendto(m_udpsocket,(char *)buf,size,0,(struct sockaddr *)&addr, sizeof(addr));  
    if (size > 0 && n < 0)  
    {   
        printf("UDPSocket sendto fail\n");  
    }
    return n;
}

int UDPSocket::SendTo(void * buf, int size, const char * ip, int port)
{
    struct sockaddr_in  remoteaddr;  
    bzero(&remoteaddr, sizeof(remoteaddr));  
    remoteaddr.sin_family = AF_INET;
    remoteaddr.sin_addr.s_addr = ::inet_addr(ip);
    remoteaddr.sin_port = htons(port);

    return SendTo(buf,size,remoteaddr);
}
void UDPSocket::Close()
{
    if(m_udpsocket)
    {
        close(m_udpsocket);
    }
}

#if 1
int main(int argc, char **argv)  
{
    char* serverIp;
    int serverPort = 0;
    int bindPort = 0;
    if(argc > 3)
    {
        serverIp = argv[1];
        serverPort = atoi(argv[2]);
        bindPort = atoi(argv[3]);
    }
    else
    {
        printf("please input  serverIp serverPort bindPort\n");
        return 0;
    }
    printf("serverIp is %s  serverPort is %d,bindPort is %d\n",serverIp,serverPort,bindPort);

    // udp client does not need bind port,input port equal 0
    if(0 != bindPort)
    {
        UDPSocket udpserver;
        udpserver.Create(bindPort);
        while(1)
        {
            // server:first recvform then sendto
            char rcvbuff[512] ={0};
            string remoteip;
            int remoteport = 0;
            int n = udpserver.RecvFrom((void *)&rcvbuff, 512, remoteip,remoteport);
            if(n > 0 )
            {
                printf("UDPSocket recv %d(char): %s,from %s(%d)\n",n,rcvbuff,remoteip.c_str(),remoteport);
                string sendmsg = "udpserver send message";
                int sn =  udpserver.SendTo((void *) sendmsg.c_str(), sendmsg.length(), remoteip.c_str(), remoteport);
                if(sn > 0 )
                {
                    //printf("UDPSocket send success\n");
                }
            }
            sleep(1);
        }        
    }
    else
    {
        UDPSocket udpclient;
        udpclient.Create();
        while (1)
        {
            //client:first sendto  then recvform
            char clientrcvbuff[512] ={0};
            string remotesip;
            int remotesport = 0;
            string clientmsg = "udpclient send message";
            int clientsendnum = udpclient.SendTo((void *) clientmsg.c_str(), clientmsg.length(), serverIp, serverPort);
            int clientrecvnum = udpclient.RecvFrom((void *)&clientrcvbuff, 512, remotesip,remotesport);
            if( clientrecvnum > 0 )
            {
                printf("UDPSocket client recv %d(char): %s,from %s(%d)\n",clientrecvnum,clientrcvbuff,remotesip.c_str(),remotesport);
            }
            sleep(1);
        }
    }
}
#endif

