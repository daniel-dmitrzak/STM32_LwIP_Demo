#include "tcp_wrapper.h"

TcpServer::TcpServer(int port)
{
	LOG("Starting TCP Server\r\n");
	mTcpPcb = tcp_new();
	tcp_server_init(mTcpPcb, port);
}

bool TcpServer::sendString(std::string inputString)
{
	if(mTcpPcb != NULL)
	{
		struct tcp_server_struct* es = NULL;
		es = (struct tcp_server_struct*)getPcb()->callback_arg;
		if(es != NULL)
		{
			if(es->state == 1 || es->state == 2)
			{
				struct pbuf* newData;
				newData = pbuf_alloc(PBUF_IP, inputString.length(), PBUF_RAM);
				std::memcpy(newData->payload, inputString.c_str(), inputString.length());

				if(es->p == NULL)
				{
					es->p = newData;
				}
				else
				{
					pbuf_chain(es->p, newData);
				}
				return true;
			}
		}
	}
	return false;
}

std::string TcpServer::getString()
{
	std::string inputString;
	struct tcp_server_struct* es = NULL;
	es = (struct tcp_server_struct*)getPcb()->callback_arg;

	if(es != NULL)
	{
		if(es->pr != NULL)
		{
			inputString.append((const char*)es->pr->payload, es->pr->len);
			struct pbuf* ptr = es->pr;
			es->pr = es->pr->next;
			int freed = 0;
			do
			{
			  freed = pbuf_free(ptr);
			}
			while(freed == 0);
		}
	}

	return inputString;
}

