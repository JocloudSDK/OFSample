#include <string.h>

DWBIND_INTERFACE(char, DWMAKE_INTERFACE_UUID_NAME(INT8))
DWBIND_INTERFACE(short, DWMAKE_INTERFACE_UUID_NAME(INT16))
DWBIND_INTERFACE(int, DWMAKE_INTERFACE_UUID_NAME(INT32))
DWBIND_INTERFACE(__int64, DWMAKE_INTERFACE_UUID_NAME(INT64))
DWBIND_INTERFACE(unsigned char, DWMAKE_INTERFACE_UUID_NAME(UINT8))
DWBIND_INTERFACE(unsigned short, DWMAKE_INTERFACE_UUID_NAME(UINT16))
DWBIND_INTERFACE(unsigned int, DWMAKE_INTERFACE_UUID_NAME(UINT32))
DWBIND_INTERFACE(unsigned __int64, DWMAKE_INTERFACE_UUID_NAME(UINT64))
DWBIND_INTERFACE(float, DWMAKE_INTERFACE_UUID_NAME(float))
DWBIND_INTERFACE(double, DWMAKE_INTERFACE_UUID_NAME(double))
DWBIND_INTERFACE(bool, DWMAKE_INTERFACE_UUID_NAME(bool))
DWBIND_INTERFACE(QString, DWMAKE_INTERFACE_UUID_NAME(QString))
DWBIND_INTERFACE(std::string, DWMAKE_INTERFACE_UUID_NAME(std::string))
DWBIND_INTERFACE(std::wstring, DWMAKE_INTERFACE_UUID_NAME(std::wstring))
class QObject;
DWBIND_INTERFACE(QObject, DWMAKE_INTERFACE_UUID_NAME(QObject))
class DuiWidget;
DWBIND_INTERFACE(DuiWidget, DWMAKE_INTERFACE_UUID_NAME(DuiWidget))
class DuiFrameWindow;
DWBIND_INTERFACE(DuiFrameWindow, DWMAKE_INTERFACE_UUID_NAME(DuiFrameWindow))
class QVariant;
DWBIND_INTERFACE(QVariant, DWMAKE_INTERFACE_UUID_NAME(QVariant))
DWBIND_INTERFACE(QList<quint32>, DWMAKE_INTERFACE_UUID_NAME(QList<quint32>))
class QMenu;
DWBIND_INTERFACE(QMenu, DWMAKE_INTERFACE_UUID_NAME(QMenu))
class DuiMenu;
DWBIND_INTERFACE(DuiMenu, DWMAKE_INTERFACE_UUID_NAME(DuiMenu))


template<typename T>
struct DWTypeInformation
{
	typedef typename T type;
	enum {pointerLevevl = 0};
};

template<typename T>
struct DWTypeInformation<T*>
{
	typedef typename DWTypeInformation<T>::type type;
	enum {pointerLevevl = DWTypeInformation<T>::pointerLevevl + 1};
};

template<typename T>
struct DWTypeInformation<const T>
{
	typedef typename DWTypeInformation<T>::type type;
	enum {pointerLevevl = DWTypeInformation<T>::pointerLevevl};
};

template<typename T>
struct DWTypeInformation<const T*>
{
	typedef typename DWTypeInformation<T>::type type;
	enum {pointerLevevl = DWTypeInformation<T>::pointerLevevl + 1};
};

template<typename T>
std::string typeName()
{
	std::string typeN = __dwuuidof(DWTypeInformation<T>::type).toString();
	if (DWTypeInformation<T>::pointerLevevl > 0)
	{
		char buf[10] = {0};
		_itoa_s(DWTypeInformation<T>::pointerLevevl, buf, 10);
		typeN += "_";
		typeN += buf;
		typeN += "ptr";
	}

	return typeN;
}

template<typename T>
DWComponentOutParam::OneParam& DWComponentOutParam::OneParam::operator =(const T &t)
{
	if (typeName<T>() == type)
	{
		*(T*)param = t;
	}
	else
	{
		__asm {int 3}
	}

	return *this;
}

template <typename T>
void DWComponentInParam::push(const T& t)
{
	OneParam one;
	one.type = typeName<T>();
	one.param = reinterpret_cast<void*>(const_cast<T*>(&t));
	m_paramList.push_back(one);
}

template<typename T>
bool DWComponentInParam::get(unsigned int pos, T& t) const
{
	pos++;
	if (pos >= m_paramList.size())
	{
		__asm {int 3}
		return false;
	}

	const OneParam& one = m_paramList[pos];	
	if (typeName<T>() != one.type)
	{
		__asm {int 3}
		return false;	//type mismatch
	}

	t = *reinterpret_cast<T*>(one.param);
	return true;
}

template<typename T>
DWComponentInParam& DWComponentInParam::operator <<(const T &t)
{
	push(t);
	return *this;
}

template<typename T>
DWComponentParamWrapper& DWComponentParamWrapper::operator >>(T &t)
{
	m_p->get(m_pos++, t);
	return *this;
}