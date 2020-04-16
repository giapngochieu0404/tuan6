
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	fd_set fdread;
	int ret;
	char buf[256];
	SOCKET clients[64];
	int numclients = 0;

	while (1)
	{
		FD_ZERO(&fdread);

		FD_SET(listener, &fdread);

		for (int i = 0; i < numclients; i++)
		{
			FD_SET(clients[i], &fdread);

	    ret = select(0, &fdread, 0, 0, 0);

		if (ret < 0)
		{
			break;
		}

		// kiem tra xem socket nao do co xay ra su kien hay k???
		if(FD_ISSET(listener, &fdread))
		{
			// co ket noi den server, chap nhan ket noi
			SOCKET client = accept(listener, NULL, NULL);
			const char* msg = "Hello client\n";
			send(client, msg, strlen(msg), 0);

			clients[numclients] = client;
			numclients++;

		}

		char cmd[16], id[32], tmp[32];
		const char* errorMsg = "Sai cu phap. Hay dang nhap lai di nao";
		for (int i = 0; i < numclients; i++)
		{
			if (FD_ISSET(clients[i], &fdread));
				//nhan du lieu
				ret = recv(clients[i], buf, sizeof(buf), 0);
				if (ret <= 0)
				{
					closesocket(clients[i]);
					// nen xoa socket khoi mang
					continue;/

				}
				buf[ret] = 0;
				printf("Received: %s\n", buf);
			}

			
			ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
			if (ret == 2)
			{
				// kiem tra ki tu dau tien co phai la client_id hay khong
				if (strcmp(cmd, "client_id:") == 0)
				{
					const char* okMsg = "Dang nhap thanh cong. Hay gui thong diep de chuyen tiep";
					send(clients[i], okMsg, strlen(errorMsg), 0);
					
					clients[numclients] = clients[i];
					numclients++;

					return;
				}
				else
				{
					send(clients[i], errorMsg, strlen(errorMsg), 0);
				}
			}
			else
			{
				send(clients[i], errorMsg, strlen(errorMsg), 0);
			}
			


			
			char sendBuf[256];
			//2.thuc hien chuyen tiep tn
			for(int i= 0;i<numclients;i++)
			{

				ret = recv(clients[i], buf, sizeof(buf), 0);
				if (ret <= 0)
				{
					closesocket(clients[i]);
					return 0;// neu dung break , no se thoat vong lapnay
					//va nhay den vong lap while phia sau
				}
				buf[ret] = 0;
				printf("Received: %s\n", buf);

				strcpy(sendBuf, id);// copy id vao sendBuf
				strcat(sendBuf, ":");
				strcat(sendBuf, buf);
				// hoac su dung sprintf(sendBuf,"%s: %s",id,buf);
				for (int i = 0; i < numclients; i++)
				{
					send(clients[i], sendBuf, strlen(sendBuf), 0);
				}
			}

		}
	}
}
