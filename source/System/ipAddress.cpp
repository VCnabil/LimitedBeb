//------------------------------------------------------------------------------
//  TITLE :          ipAddress.c
//------------------------------------------------------------------------------

#include "syslib.h"
#include "ipAddress.h"
#include <string.h>

#ifdef __LINUX__
 #include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#include <asm/types.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/types.h>

#endif

#ifdef __LINUX__
#define PACKETSIZE  64
#define BUFSIZE		8192
struct packet
{
	struct icmphdr hdr;
	char msg[PACKETSIZE-sizeof(struct icmphdr)];
};

int pid=-1;
struct protoent *proto=NULL;
int cnt=1;

struct route_info
{
	u_int dstAddr;
	u_int srcAddr;
	u_int gateWay;
	char ifName[IF_NAMESIZE];
};

#endif

char* IpAddressGet(char* pBuffer, uint32_t bufLen)
{
#ifdef __LINUX__
	int fd;

	// Open socket
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd != -1)
	{
		int ret;
		struct ifreq ifr;

		// IPv4 IP address
		ifr.ifr_addr.sa_family = AF_INET;

		// Check eth0 first (internal)
		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
		ret = ioctl(fd, SIOCGIFADDR, &ifr);
		if (ret != 0)
		{
			// Try eth2 second (USB)
			strncpy(ifr.ifr_name, "eth2", IFNAMSIZ-1);
			ret = ioctl(fd, SIOCGIFADDR, &ifr);
		}
		if (ret == 0)
		{
			// Got a result, use this IP address
			strncpy(pBuffer, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), bufLen);
		}
		else
		{
			// Can't get the IP
			strncpy(pBuffer, "N/A", bufLen);
		}

		// Close socket
		int res = close(fd);
		if (res < 0)
		{
			int a = errno;
			errno++;
		}
	}
	else
	{
		int a = errno;
		perror("socket");
		strncpy(pBuffer, "N/A", bufLen);
	}
#else
	strncpy(pBuffer, "N/A", bufLen);
#endif

	return pBuffer;
}