#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__
#include <sys/types.h>  
#include <sys/socket.h>    
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

class UDPSocket
{
public:
	UDPSocket():m_udpsocket(-1)
    {};
    virtual ~UDPSocket()
    {
        Close();
    }
	bool Create(void);
	bool Create(int port);
	bool Create(const char * const szIP, int port);

	/** 
	 * @brief 
       @param (in) buf
       @param (in) size
       @param (out) ip
       @param (out) port
       @return 
       @note 
       \n<b>ALL</b>:  recvfrom()
	 */	
	int RecvFrom(void* buf, int size, string & ip, int& port);

	/** 
	 * @brief 
       @param (in) buf 
       @param (in) size 
       @param (out) addr 
       @return 
       @note 
       \n<b>ALL</b>:  recvfrom()
	 */	
	int RecvFrom(void* buf, int size, sockaddr_in& addr);

	/** 
	 * @brief 
       @param (in) buf 
       @param (in) size 
       @param (in) ip 
       @param (in) port 
       @return 
       @note 
       \n<b>ALL</b>:  sendto()
	 */	
	int SendTo(void* buf, int size, const char* ip, int port);
    /** 
	 * @brief 
       @param (in) buf 
       @param (in) size 
       @param (in) addr 
       @return 
       @note 
       \n<b>ALL</b>:  sendto()
	 */	
	int SendTo(void* buf, int size, const sockaddr_in& addr);
	void Close();
    int  GetFd()
    {
        return m_udpsocket;
    }

private:
	int m_udpsocket;
};
#endif

