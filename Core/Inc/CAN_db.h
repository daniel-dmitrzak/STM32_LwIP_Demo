#pragma once
/*
 * can_db.h
 *
 *  Created on: Mar 24, 2024
 *      Author: Daniel Dmitrzak
 */

#ifndef INC_CAN_DB_H_
#define INC_CAN_DB_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <sstream>

class CANSignal
{
public:
	CANSignal(std::string name, unsigned int start, unsigned int length, double factor, int offset, bool sign);
	std::string getSignalName() { return mSignalName; }
	double getValue() { return mDecodedValue;  }
	double signalDecode(double value);
	uint8_t getStart() { return mStart; }
	uint8_t getLength() { return mLength; }
	bool getIsUnsigned() { return mIsUnsigned;  }
	double signalEncode(double value);
	double getRawValue() { return mRawValue; }
private:
	std::string mSignalName;
	unsigned int mStart;
	unsigned int mLength;
	double mFactor;
	int mOffset;
	bool mIsUnsigned;
	double mDecodedValue;
	bool mOrder;
	double mRawValue;
};

class CANMessage
{
public:
	CANMessage(std::string name, uint16_t ID);

	void addSignal(CANSignal);
	void messageDecode(uint8_t* buffer, uint8_t len);
	void messageEncode(uint8_t* buffer, uint8_t len);
	uint16_t getID() { return mID;  }
	bool checkID(uint16_t value);
	std::vector<CANSignal> getSignals() { return mSignals; }
	std::string getName() { return mName;  }
	std::string buildCString();
	CANSignal* findSignalByName(std::string name);

private:
	std::string mName;
	uint16_t mID;
	std::vector<CANSignal> mSignals;
};

class CANDb
{
public:
	CANMessage* findMessageByID(uint16_t id);
	std::vector<CANMessage> mCANMessages;
	void buildTestDb();
};


#endif /* INC_CAN_DB_H_ */
