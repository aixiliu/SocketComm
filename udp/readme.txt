2018-3-6 
�׽��ֵ�ַ�Ľṹ�����ڲ������ݣ�һ��ʹ�����ã�ָ��ṹ��ָ�룩
�׽��ֵ�ͨ�ýṹ
struct sockaddr{
uint8_t sa_len;
sa_family_t sa_family;
char sa_data[14];
}

ipv4���׽��ֵ�ַ��sockadd_in
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
in_addr ��һ���ṹ��������ANSI C֮ǰ������A\B\C���ַ�ķ��ʡ��������ڵ�������ַ��һ��in_addr����һ���޷��ŵ�32Ϊ������
�׽��ֺ�����ʹ���׽��ֽṹ��Ϊ��Σ����뽫ָ���ض�Э����׽��ֽṹ��ָ�����ǿ������ת�������ָ��ͨ���׽��ֽṹ�ĵ�ָ�롣

