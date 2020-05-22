#include "local.h"
#include <wifilocal.h>

sem_t sem;

#define SIZE 512
void sock_close(int socket)
{

	shutdown(socket, SHUT_RDWR);

	close(socket);
	return;
}

int create_socket()
{
	int lisfd;
	lisfd = socket(AF_INET, SOCK_STREAM, 0); //IPV4 数据流 TCP类型
	if (lisfd == -1)
	{
		INFO("create_socket failed! error:%s", strerror(errno));
		return -1;
	}

	return lisfd;
}

int sock_bind(int lisfd, int port)
{
	char ip[20];
	char buf[20];
	int reuse = 1;

	struct sockaddr_in myaddr;
	memset((char *)&myaddr, 0, sizeof(struct sockaddr_in)); //清零
	myaddr.sin_family = AF_INET;							//IPV4
	myaddr.sin_port = htons(port);							//端口
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");		//允许连接到所有本地地址上

	if (setsockopt(lisfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		INFO("setsockopet error");
		return -1;
	}

	if (bind(lisfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) < 0)
	{
		INFO("sock_bind failed! error:%s , port:%d", strerror(errno), port);
	}
}

int sock_listen(int lisfd, int max_con)
{
	if (listen(lisfd, max_con) == -1)
	{
		INFO("sock_listen failed! error:%s", strerror(errno));
	}
	return 0;
}

int sock_accept(int lisfd)
{
	struct sockaddr remoaddr;
	int clifd;
	socklen_t size = sizeof(struct sockaddr);

	memset((char *)&remoaddr, 0, sizeof(struct sockaddr));
	clifd = accept(lisfd, &remoaddr, &size);
	if (clifd == -1)
	{
		INFO("sock_accept failed! error:%s", strerror(errno));
	}
	return clifd;
}

int sock_connect(int clifd, char *ipaddr, int port)
{
	struct sockaddr_in remoaddr;
	memset((char *)&remoaddr, 0, sizeof(struct sockaddr_in));
	remoaddr.sin_family = AF_INET;				  //IPV4地址
	remoaddr.sin_port = htons(port);			  //端口
	remoaddr.sin_addr.s_addr = inet_addr(ipaddr); //服务器端IP地址
	if (connect(clifd, (struct sockaddr *)&remoaddr, sizeof(struct sockaddr)) == -1)
	{
		INFO("sock_connect failed! error:%s", strerror(errno));
	}

	return 0;
}

int sock_send(int sockfd, void *buf, size_t len, int flags)
{
	int nbytes;
	nbytes = send(sockfd, buf, len, flags);
	if (nbytes == -1)
	{
		INFO("sock_send failed! error:%s", strerror(errno));
	}
	return nbytes;
}

int sock_recv(int sockfd, void *buf, size_t len, int flags)
{
	int nbytes;
	nbytes = recv(sockfd, buf, len, flags);
	if (nbytes == -1)
	{

		INFO("sock_recv failed %d,error:%s ", sockfd, strerror(errno));
	}
	return nbytes;
}

int accept_error = 0;
char *mybuf;
int clifd;
extern void handle_dispatch_pack(char *buf, int len);
void socket_recv(int *sock)
{
	int socket;
	int length;
	// char *mybuf = NULL;
	int rv;

	socket = *sock;

	mybuf = (char *)malloc(100);
	if (mybuf == NULL)
	{
		INFO("buf alloc failed");
	}
	else
	{
		INFO("buf alloc ok ");
	}
	while (1)
	{
		/* code */
		// socket = *sock;
		memset(mybuf, 0, 100);
		length = sock_recv(socket, mybuf, 100, 0);
		if (length < 0)
		{
			if (length == EAGAIN)
				continue;
			INFO("socket_recv error:%s", strerror(errno));
			break;
		}
		else if (length == 0)
		{
			// char *ip = "127.0.0.1";
			// int port = 9002;
			INFO(" socket_recv socket close :%s", strerror(errno));
			// socket = socket_client_init(ip,port);
			break;
		}
		if (length > 0)
		{
			INFO("recv data(%d): %s 123", length, mybuf);
			handle_dispatch_pack(mybuf, length);
		}
	}
	free(mybuf);
	sock_close(0);
	pthread_exit(&rv);
	return;
}
void socket_send()
{
	while (1)
	{
		/* code */
		// sem_wait(&sem);
		if (mybuf)
		{
			INFO("TO CLIET");
			sock_send(clifd, mybuf, strlen(mybuf) + 1, 0);
		}
	}
	return;
}
extern int local_port;
int num;
void socket_server_init(int *por)
{
	pthread_t tid, rectid;
	int lisfd;
	int num = 0, length = 0;
	int port;
	int rv;

	port = *por;

	// sem_init(&sem,0,0);//初始化信号量

	INFO(" port : %d ", port);

	lisfd = create_socket();
	sock_bind(lisfd, port);
	sock_listen(lisfd, 1000);

	while (1)
	{
		/* code */
		INFO(" bbbbbbbbbbbbbbbb  ");

		pthread_testcancel();
		clifd = sock_accept(lisfd);
		pthread_testcancel();

		INFO(" 1234  ");

		INFO("scoket clent connect my socket server listenfd(%d) port(%d) fd:%d", lisfd, port, clifd);

		if (clifd < 0)
		{
			continue;
		}
		num++;
		// if (num < 2)
		{
			if (pthread_create(&rectid, NULL, (void *)socket_recv, &clifd))
			{
				INFO("\r\n pthread_create error\r\n");
			}
		}
	}

	close(clifd);
	close(lisfd);
	pthread_exit(&rv);
}

int socket_client_init(char *ipadd, int port)
{
	int clifd;

	clifd = create_socket();
	sock_connect(clifd, ipadd, port);

	return clifd;
}
