2018-3-6 
套接字地址的结构，用于参数传递，一般使用引用（指向结构的指针）
套接字的通用结构
struct sockaddr{
uint8_t sa_len;
sa_family_t sa_family;
char sa_data[14];
}

ipv4的套接字地址，sockadd_in
struct in_addr{
in_addr_t s_addr;             /* 32 bit ipv4 address ,network byte ordered */
}

struct sockaddr_in{
uint8_t sin_len;             /* length of structure */
sa_family_t sin_family;      /* AF_INET */
in_port_t   sin_port;        /* 16 bit TCP or UDP port number  network byte ordered */
struct in_addr sin_addr;     /* 32 bit ipv4 address,network byte ordered */
char        sin_zero[8];     /* unused */
}
in_addr 是一个结构，用于在ANSI C之前，对于A\B\C类地址的访问。对于现在的无类别地址，一般in_addr都是一个无符号的32为整数，
套接字函数，使用套接字结构作为入参，必须将指向特定协议的套接字结构的指针进行强制类型转换，变成指向通用套接字结构的的指针。

