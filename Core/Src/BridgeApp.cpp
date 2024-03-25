#include "BridgeApp.h"

BridgeApp::BridgeApp(uint16_t port, FDCAN_HandleTypeDef* canHandle)
{
	LOG("Starting Bridge Appe\r\n");
	mTcpServer = new TcpServer(port);
	mCanDB.buildTestDb();
	mFDCANHandle = canHandle;
}

void BridgeApp::appFunct()
{
	std::string signalName;
	double signalValue;

	getCAN();
	if(getCommand(signalName, signalValue))
	{
		sendCANSignal(signalName, signalValue);
	}
}

bool BridgeApp::getCommand(std::string& key, double& value)
{
	std::string tempString = mTcpServer->getString();
	mCommandString.append(tempString);
	if(!tempString.empty()) {
		std::string tempKey;
		double tempValue;
		if(getTokens(mCommandString, tempKey, tempValue))
		{
			printf("Command string size: %d key: %s value: %f CommandString:%s\r\n", mCommandString.length(), tempKey.c_str(), tempValue, mCommandString.c_str());
			key = tempKey;
			value = tempValue;
			return true;
		}
	}
	return false;
}

bool BridgeApp::sendCANSignal(std::string name, double value)
{
	// Init CAN header
	FDCAN_TxHeaderTypeDef        txHeader;
	txHeader.Identifier          = 0x600;
	txHeader.IdType              = FDCAN_STANDARD_ID;
	txHeader.TxFrameType         = FDCAN_DATA_FRAME;
	txHeader.DataLength          = FDCAN_DLC_BYTES_8;
	txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	txHeader.BitRateSwitch       = FDCAN_BRS_ON;
	txHeader.FDFormat            = FDCAN_FD_CAN;
	txHeader.TxEventFifoControl  = FDCAN_STORE_TX_EVENTS;
	txHeader.MessageMarker       = 0x52U;

	// Init CANdb object pointers
	CANMessage* pTempMessage = NULL;
	CANSignal*  pTempSignal  = NULL;

	pTempMessage = mCanDB.findMessageByID(0x600);
	if(pTempMessage != NULL)
	{
		pTempSignal = pTempMessage->findSignalByName(name);
		if(pTempSignal != NULL)
		{
			pTempSignal->signalEncode(value);
			uint8_t buffer[8];
			pTempMessage->messageEncode(buffer, 8);

			if (HAL_FDCAN_AddMessageToTxFifoQ(mFDCANHandle, &txHeader, buffer) == HAL_OK)
			{
				printf("Signal %s sent OK!\r\n", name.c_str());
				return true;
			}
			else
			{
				printf("Signal %s failed to send!\r\n", name.c_str());
				return false;
			}
		}
	}
	return false;

}

void BridgeApp::getCAN()
{
	// Handle CAN Here
	if(HAL_FDCAN_GetRxFifoFillLevel(mFDCANHandle, FDCAN_RX_FIFO0) > 0)
	{
		printf("RxFifoLevel: %ld\r\n", HAL_FDCAN_GetRxFifoFillLevel(mFDCANHandle, FDCAN_RX_FIFO0));
		FDCAN_RxHeaderTypeDef rxHeader;
		uint8_t rxDataBuffer[12];
		if(HAL_OK == HAL_FDCAN_GetRxMessage(mFDCANHandle, FDCAN_RX_FIFO0, &rxHeader, &rxDataBuffer[0]))
		{
			CANMessage* pCANMessage = NULL;
			pCANMessage = mCanDB.findMessageByID(rxHeader.Identifier & 0xFFF);
			if(pCANMessage != NULL)
			{
				printf("We received msg %s\r\n", pCANMessage->getName().c_str());
				pCANMessage->messageDecode(rxDataBuffer, rxHeader.DataLength);
				mTcpServer->sendString(pCANMessage->buildCString());
			}
		}
	}
}

bool BridgeApp::getTokens(std::string& input, std::string& key, double& value)
{
	size_t pos = input.find('&');
	if(pos != std::string::npos)
	{
		input.erase(0, pos); // Remove all of the garbage before &
		pos = input.find('\n');
		if(pos != std::string::npos) // Check if command finished receiving
		{
			std::string line;
			line = input.substr(0, pos);
			input.erase(0, pos+1);
			printf("Got full command line: %s \r\n",line.c_str());
			std::string sValue;
			pos = line.find(':');
			if(pos != std::string::npos)
			{
				key = line.substr(1, pos-1);
				sValue = line.substr(pos+1, line.length()-pos-2);
				printf("Key:'%s' Value:'%s' \r\n", key.c_str(),sValue.c_str());
				value = atof(sValue.c_str());
				return true;
			}
		}
	}
	else
	{
		// Just some garbage. Clear the buffer
		input.clear();
	}
	return false;
}
