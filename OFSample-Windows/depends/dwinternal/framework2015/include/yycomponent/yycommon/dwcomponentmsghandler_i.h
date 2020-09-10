#pragma once
#include <vector>
#include <string>

struct DWComponentOutParam
{
	struct OneParam
	{
		OneParam():param(0) {}
		template<typename T>
		OneParam &operator = (const T& t);

		std::string type;
		void *param;
	};

	DWComponentOutParam()
	{
		m_paramList.resize(1);	//第一个有特殊用途
	}
	unsigned int count() const{return m_paramList.size()-1;}
	OneParam& operator [](unsigned int index) 
	{
		if (index<count())
		{
			return m_paramList[index+1];
		}
		
		return m_paramList[0];
	}
protected:
	std::vector<OneParam> m_paramList;
};

struct DWComponentInParam: public DWComponentOutParam
{
	void clear()
	{
		m_paramList.clear();
	}
	template <typename T>
	void push(const T& t);

	template<typename T>
	bool get(unsigned int pos, T& t) const;

	template<typename T>
	DWComponentInParam& operator << (const T& t);
};
typedef DWComponentInParam DWComponentParam;

class DWComponentParamWrapper
{
public:
	DWComponentParamWrapper(DWComponentInParam *p):m_p(p), m_pos(0) {};

	void rewind(){m_pos = 0;}

	template<typename T>
	DWComponentParamWrapper& operator >> (T& t);

private:
	DWComponentInParam *m_p;
	unsigned int m_pos;
};

#include "detail/dwcomponentparamimpl.h"

enum MsgHandleReturnType
{
	MsgHandled,
	MsgNotHandled,
	MsgFailed,
};

DWDEFINE_INTERFACE(IDWComponentMsgReceiver): public IUnk	//组件实现，组件通过此接口，接收平台发送的消息
{
	virtual MsgHandleReturnType onMessage(unsigned int id, const DWComponentInParam &input, DWComponentOutParam &output) = 0;
};

DWDEFINE_INTERFACE(IDWComponentMsgSender): public IUnk	//平台实现，组件通过此接口发送消息给平台
{
	virtual MsgHandleReturnType sendMessage(unsigned int id, const DWComponentInParam &input, DWComponentOutParam &output) = 0;
};

