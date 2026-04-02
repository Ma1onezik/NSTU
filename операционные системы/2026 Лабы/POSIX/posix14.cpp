//-------------- posix14 - родитель, 2 потомка, 2 сокета - ping родитель-потомок-родитель
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int ServerSocketBase=6000;	// База номера порта

//----------------------- Сокет потомка ---- передача и прием размерность/массив
void childPing(){
	int ppid = getppid();
	int pid = getpid();
	printf("Потомок=%d, родитель pid=%d\n",pid,ppid);
	const char *server_ip = "127.0.0.1";
	int client;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ServerSocketBase+ppid); 
	addr.sin_addr.s_addr = inet_addr(server_ip);
	// файловый дескриптор (число)
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client <0){
		printf("Потомок=%d, родитель pid=%d Ошибка подключения_1=%d\n",pid,ppid,client);
		return;
		} 
	int vv = connect(client, (struct sockaddr *) &addr, sizeof(addr));
	if (vv < 0) {
		printf("Потомок=%d, родитель pid=%d Ошибка подключения_2=%d\n",pid,ppid,vv);
		return;
		} 
	int size=0;
	int bytes_read; 	// Размер сообщения в байтах
	bytes_read = recv(client, &size, sizeof(int), 0);
	if (bytes_read!=sizeof(int)){
		printf("Потомок=%d, родитель pid=%d, родитель отправил байт(1)=%d\n",pid,ppid,bytes_read);		
		close(client);
		return;
		}
	int *a = new int[size];
	bytes_read = recv(client, a, size*sizeof(int), 0);
	if (bytes_read!=size*sizeof(int)){
		printf("Потомок=%d, родитель pid=%d, родитель отправил байт(2)=%d\n",pid,ppid,bytes_read);		
		close(client);
		return;
		}
	printf("Потомок=%d, родитель pid=%d, потомок получил ",pid,ppid);		
	for(int i=0;i<size;i++)	
		printf("%d ",a[i]);
	printf("\n");
    send(client, &size, sizeof(int), 0);
    send(client, a, size*sizeof(int), 0);
	delete []a;
	close(client);
	}

//----------------------- Соект родителя ---- передача и прием размерность/массив
void oneParentPing(int socket,int a[], int size){
	int pid = getpid();
    send(socket, &size, sizeof(int), 0);
    send(socket, a, size*sizeof(int), 0);
	int size2=0;
	int bytes_read; 	// Размер сообщения в байтах
	bytes_read = recv(socket, &size2, sizeof(int), 0);
	if (bytes_read!=sizeof(int)){
		printf("Pодитель pid=%d, родитель принял байт(1)=%d\n",pid,bytes_read);		
		close(socket);
		return;
		}
	int *a2 = new int[size2];
	bytes_read = recv(socket, a2, size2*sizeof(int), 0);
	if (bytes_read!=size2*sizeof(int)){
		printf("Родитель pid=%d, родитель принял байт(2)=%d\n",pid,bytes_read);		
		close(socket);
		return;
		}
	printf("Родитель pid=%d, родитель получил ",pid);		
	for(int i=0;i<size2;i++)	
		printf("%d ",a2[i]);
	printf("\n");
	delete []a2;
    close(socket);
    }
//----------------------- Серверный сокет - прием соединений родителем
void oneConnection(){		
    int A[]={1,2,3,4,5,6,7,8,9,10};
	int pid = getpid();
	printf("Родитель pid=%d\n",pid);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ServerSocketBase+pid); 	// преобразование порта в сетевой порядок байт
    addr.sin_addr.s_addr = htons(INADDR_ANY); 		// Принимать любой запрос
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("pid= %d Не удалось связать сокет сервера с адресом\n",pid);
        return;
		}
    if (listen(server, 1) != 0) {
        close(server);
		printf("Не удалось прослушать сокет\n");
		}
    int client1 = accept(server, nullptr, nullptr); 
	if (client1 < 0) {
        printf("pid= %d Не удалось получить клиента 1=%d\n",pid,client1);
        close(server);
        return;
        }
	oneParentPing(client1,A,10);	
    close(server);
    }

int main (void) {
    switch (fork ()) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			childPing();
            exit (0);
		default:		
			// Родитель
			oneConnection();
			}
		wait (NULL);
		return (0);
		}
