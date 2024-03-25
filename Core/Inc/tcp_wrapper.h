
#ifndef INC_TCP_WRAPPER_H_
#define INC_TCP_WRAPPER_H_

#include "tcp_server.h"
#include "main.h"
#include <cstring>
#include <string>

class TcpServer
{
public:
	TcpServer(int port);
	struct tcp_pcb* getPcb() { return mTcpPcb; }
	bool sendString(std::string inputString);
	std::string getString();
private:
	struct tcp_pcb* mTcpPcb;
	struct tcp_server_struct mEs;
};

#endif
