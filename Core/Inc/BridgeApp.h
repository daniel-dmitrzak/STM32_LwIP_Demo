#ifndef INC_BRIDGE_APP_H_
#define INC_BRIDGE_APP_H_

#include "main.h"
#include "tcp_wrapper.h"
#include "CAN_db.h"


class BridgeApp
{
public:
	BridgeApp(uint16_t port, FDCAN_HandleTypeDef* canHandle);
	void appFunct();
private:
	bool getCommand(std::string& key, double& value);
	bool getTokens(std::string&, std::string&, double&);
	void getCAN();
	bool sendCANSignal(std::string signal, double value);
	std::string mCommandString;
	TcpServer* mTcpServer;
	CANDb mCanDB;
	FDCAN_HandleTypeDef* mFDCANHandle;
};

#endif
