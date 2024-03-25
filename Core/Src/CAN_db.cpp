#include "CAN_db.h"

CANSignal::CANSignal(std::string name, unsigned int start, unsigned int length, double factor, int offset, bool unsig) :
	mSignalName		(name),
	mStart			(start),
	mLength			(length),
	mFactor			(factor),
	mOffset			(offset),
	mIsUnsigned		(unsig),
	mDecodedValue	(0.0),
	mOrder			(false),
	mRawValue		(0.0)
{
}
	
double CANSignal::signalDecode(double input)
{
	mDecodedValue = (input + mOffset) * mFactor; 
	return mDecodedValue;
}

double CANSignal::signalEncode(double input)
{
	mDecodedValue = input;
	mRawValue = (input / mFactor) - mOffset;
	return mRawValue;
}

CANMessage::CANMessage(std::string name, uint16_t ID):
	mName	(name),
	mID		(ID)
{
}

CANSignal* CANMessage::findSignalByName(std::string name)
{
	std::vector<CANSignal>::iterator it;
	for (it = mSignals.begin(); it != mSignals.end(); it++)
	{
		if ((*it).getSignalName() == name)
			return &(*it);
	}
	return NULL;
}

void CANMessage::messageDecode(uint8_t* buffer, uint8_t len)
{
	std::vector<CANSignal>::iterator it; 
	for (it = mSignals.begin(); it != mSignals.end(); it++)
	{
		uint16_t valueRaw = 0;
		uint8_t sigStart = (*it).getStart() / 8;
		uint8_t sigLen = (*it).getLength() / 8;
		uint8_t sigEnd = sigStart + sigLen; 

		if (sigEnd > len) return;

		valueRaw |= buffer[sigStart] & 0xFF; 
		if (sigLen == 2)
		{
			valueRaw |= (buffer[sigStart + 1] << 8) & 0xFF00; 
		}

		double intermediate = 0.0;
		if (!(*it).getIsUnsigned())
		{
			if (sigLen == 1) intermediate = (int8_t)valueRaw; 
			else if (sigLen == 2) intermediate = (int16_t)valueRaw;
		}
		else
		{
			intermediate = valueRaw;
		}
		(*it).signalDecode(intermediate);
	}
}

void CANMessage::messageEncode(uint8_t* buffer, uint8_t len)
{
	std::vector<CANSignal>::iterator it;
	for (it = mSignals.begin(); it != mSignals.end(); it++)
	{
		uint16_t valueRaw = 0;
		uint8_t sigStart = (*it).getStart() / 8;
		uint8_t sigLen = (*it).getLength() / 8;
		uint8_t sigEnd = sigStart + sigLen;
		(*it).signalEncode((*it).getValue());
		if (sigEnd > len) continue;

		if ((*it).getIsUnsigned())
		{
			valueRaw = (*it).getRawValue();
		}
		else
		{
			if (sigLen == 1) valueRaw = static_cast<int8_t>((*it).getRawValue());
			else if (sigLen == 2) valueRaw = static_cast<int16_t>((*it).getRawValue());
		}
		buffer[sigStart] = valueRaw & 0xFF;
		if (sigLen == 2) buffer[sigStart + 1] = (valueRaw & 0xFF00) >> 8;
	}
}

std::string CANMessage::buildCString()
{
	std::stringbuf tmpString;
	std::ostream oTmpString (&tmpString);
	oTmpString << getName() << " ";
	std::vector<CANSignal>::iterator it;
	for (it = mSignals.begin(); it != mSignals.end(); it++)
	{
		oTmpString << (*it).getSignalName() << ":" << (*it).getValue() << " ";
	}
	oTmpString << std::endl;
	return tmpString.str();
}

void CANMessage::addSignal(CANSignal signal)
{
	mSignals.push_back(signal);
}

bool CANMessage::checkID(uint16_t id)
{
	return (id == mID);
}

CANMessage* CANDb::findMessageByID(uint16_t id)
{
	std::vector<CANMessage>::iterator it;
	for (it = mCANMessages.begin(); it != mCANMessages.end(); it++)
	{
		if ((*it).checkID(id))
		{
			return &(*it);
		}
	}
	return NULL;
}

void CANDb::buildTestDb()

{
	CANMessage mBase0Message = CANMessage("Base0", 0x600);
	mBase0Message.addSignal(CANSignal("RPM", 0, 16, 1, 0, true));
	mBase0Message.addSignal(CANSignal("TPS", 16, 8, 0.5, 0, true));
	mBase0Message.addSignal(CANSignal("IAT", 24, 8, 1, 0, false));
	mBase0Message.addSignal(CANSignal("MAP", 32, 16, 1, 0, true));
	mBase0Message.addSignal(CANSignal("INJPW", 48, 16, 0.016129, 0, true));

	CANMessage mBase1Message = CANMessage("Base2", 0x602);
	mBase1Message.addSignal(CANSignal("VSPD", 0, 16, 1, 0, true));
	mBase1Message.addSignal(CANSignal("BARO", 16, 8, 1, 0, true));
	mBase1Message.addSignal(CANSignal("OILT", 24, 8, 1, 0, true));
	mBase1Message.addSignal(CANSignal("OILP", 32, 8, 0.0625, 0, true));
	mBase1Message.addSignal(CANSignal("FUELP", 40, 8, 0.0625, 0, true));
	mBase1Message.addSignal(CANSignal("FUELP", 48, 16, 1, 0, false));

	mCANMessages.push_back(mBase0Message);
	mCANMessages.push_back(mBase1Message);
}
