#pragma once

namespace protocol
{
	namespace session
	{
		enum CodecRate;
	}
}

DWDEFINE_INTERFACE(IAudioEngineEncode) : IUnk
{
	virtual bool init(protocol::session::CodecRate codecRate) = 0;
	virtual void flush() = 0;
	virtual bool encode(const std::string &inPCM, std::string &outData, std::string &lbrr) = 0;
	virtual bool encodeLoss(std::string &outData) = 0;
};

DWDEFINE_INTERFACE(IAudioEngineDecode) : IUnk
{
	virtual bool init(protocol::session::CodecRate codecRate) = 0;
	virtual void flush() = 0;
	virtual bool decode(const std::string &inData, std::string &outPCM) = 0;
	virtual bool decodeLoss(std::string &outPCM) = 0;
};
