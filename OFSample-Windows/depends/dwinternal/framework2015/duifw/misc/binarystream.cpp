#include "stable.h"
#include "binarystream.h"

namespace
{
	const quint32 KDefaultSize = 3000;
}

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
# define READ_FROM_BIG_ENDIAN readNoSwapped
# define WRITE_TO_BIG_ENDIAN writeNoSwapped
# define READ_FROM_LITTLE_ENDIAN readSwapped
# define WRITE_TO_LITTLE_ENDIAN writeSwapped
#else
# define READ_FROM_BIG_ENDIAN readSwapped
# define WRITE_TO_BIG_ENDIAN writeSwapped
# define READ_FROM_LITTLE_ENDIAN readNoSwapped
# define WRITE_TO_LITTLE_ENDIAN writeNoSwapped
#endif

void DwBinaryStream::chop(int s)
{
	m_offset += s;
	if(m_offset >= m_data.size()) {
		m_data.clear();
		m_offset = 0;
	}
}

void DwBinaryStream::reserve(int s)
{
	if(m_offset != 0 && m_data.size() + s > m_data.capacity()) {
		m_data.remove(0, m_offset);
		m_offset = 0;
	}

	if(m_data.size() + s > m_data.capacity()) {
		m_data.reserve(m_data.size() + s);
	}
}

int DwBinaryStream::remains() const
{
	return m_data.size() - m_offset;
}

const char* DwBinaryStream::constData() const
{
	return m_data.constData() + m_offset;
}

void DwBinaryStream::readNoSwapped(void *dst, int s)
{
	memcpy_s(dst, s, constData(), s);
	chop(s);
}

void DwBinaryStream::writeNoSwapped(void *src, int s)
{
	reserve(s);
	m_data.append(static_cast<const char*>(src), s);
}

void DwBinaryStream::readSwapped(void *raw, int s)
{
	const char *src = constData();
	char *dst = (char*)raw;
	for(int i = 0; i < s; ++i) {
		dst[i] = src[s - i - 1];
	}
	chop(s);
}

void DwBinaryStream::writeSwapped(void *raw, int s)
{
	const char* src = (const char*)raw;
	reserve(s);
	while(s) {
		m_data.append(src[--s]);
	}
}

//virtual
void DwBinaryStream::readWithEndianAdjustment(void *dst, int s)
{
	READ_FROM_LITTLE_ENDIAN(dst, s);
}

//virtual
void DwBinaryStream::writeWithEndianAdjustment(void *src, int s)
{
	WRITE_TO_LITTLE_ENDIAN(src, s);
}

bool DwBinaryStream::read(void *dst, int s)
{
	if (remains() < s)
	{
		DW_ASSERT_X(false, __FUNCTION__, "read error!");
		//engine()->currentContext()->throwError("read error!");
		return false;
	}

	readWithEndianAdjustment(dst, s);

	return true;
};

bool DwBinaryStream::write(void *src, int s)
{
	writeWithEndianAdjustment(src, s);
	return true;
};

DwBinaryStream::DwBinaryStream(QObject *parent /* = NULL */)
	:QObject(parent), m_data(), m_offset(0)
{
	m_data.reserve(KDefaultSize);
}

DwBinaryStream::DwBinaryStream(const QByteArray &rhs, QObject *parent)
	:QObject(parent), m_data(rhs), m_offset(0)
{
}

DwBinaryStream::~DwBinaryStream()
{
}

//virtual
int DwBinaryStream::putBlobLength(int length) {
	writeInt16(length);
	return length;
}

//virtual
int DwBinaryStream::getBlobLength() {
	return readInt16();
}

//virtual
int DwBinaryStream::toStringLength(int len) {
	return len;
}

bool DwBinaryStream::writeByte(unsigned char b)
{
	return write(&b, sizeof(unsigned char));
}

bool DwBinaryStream::writeUInt16(unsigned int b)
{
	UINT16 t = static_cast<UINT16>(b);
	return write(&t, sizeof(UINT16));
}

bool DwBinaryStream::writeInt16(int b)
{
	INT16 t = static_cast<INT16>(b);
	return write(&t, sizeof(INT16));
}

bool DwBinaryStream::writeInt(int n)
{
	return write(&n, sizeof(int));
}

bool DwBinaryStream::writeUInt(unsigned int n)
{
	return write(&n, sizeof(unsigned int));
}

bool DwBinaryStream::writeInt64(qlonglong n)
{
	return write(&n, sizeof(qlonglong));
}

bool DwBinaryStream::writeUInt64(qulonglong n)
{
	return write(&n, sizeof(qulonglong));
}
bool DwBinaryStream::writeBool(bool b)
{
	return write(&b, sizeof(bool));
}

bool DwBinaryStream::writeReal(double d)
{
	return write(&d, sizeof(double));
}

bool DwBinaryStream::writeBinary(const DwBinaryStream *binary)
{
	return this->writeByteArray(binary->getByteArray());
}

//Q_INVOKABLE
bool DwBinaryStream::writeRaw(const DwBinaryStream *binary)
{
	reserve(binary->remains());
	m_data.append(binary->constData(), binary->remains());
	return true;
}

bool DwBinaryStream::writeByteArray(const QByteArray &bytes)
{
	int len = putBlobLength(bytes.size());
	reserve(len);
	//be careful, DwBinaryStream32::putBlobLength return bytes.size() + 1
	//this will append the extra '\0' to result blob, as openplatform wire format required
	m_data.append(bytes.constData(), len);
	return true;
}

bool DwBinaryStream::writeString(const QString &str)
{
	return this->writeByteArray(str.toUtf8());
}

unsigned char DwBinaryStream::readByte()
{
	unsigned char c = 0;
	read(&c, sizeof(unsigned char));
	return c;
}

unsigned int DwBinaryStream::readUInt16()
{
	UINT16 c = 0;
	read(&c, sizeof(UINT16));
	return c;
}

int DwBinaryStream::readInt16()
{
	INT16 c = 0;
	read(&c, sizeof(INT16));
	return c;
}

int DwBinaryStream::readInt()
{
	int c = 0;
	read(&c, sizeof(int));
	return c;
}

unsigned int DwBinaryStream::readUInt()
{
	unsigned int c = 0;
	read(&c, sizeof(unsigned int));
	return c;
}

qlonglong DwBinaryStream::readInt64()
{
	qlonglong c = 0;
	read(&c, sizeof(qlonglong));
	return c;
}

qulonglong DwBinaryStream::readUInt64()
{
	qulonglong c = 0;
	read(&c, sizeof(qulonglong));
	return c;
}

bool DwBinaryStream::readBool()
{
	bool c = 0;
	read(&c, sizeof(bool));
	return c;
}

double DwBinaryStream::readReal()
{
	double c = 0;
	read(&c, sizeof(double));
	return c;
}

bool DwBinaryStream::readByteArray(QByteArray *bytes)
{
	int len = getBlobLength();
	if (len == 0)
	{
		return true;
	}
	else if (len > 0 && remains() >= len)
	{
		const char *data = constData();
		bytes->clear();
		bytes->append(data, len);
		chop(len);
		return true;
	}

	DW_ASSERT_X(false, __FUNCTION__, "readBinary error.");
	//engine()->currentContext()->throwError("readBinary error.");
	return false;
}

bool DwBinaryStream::readBinary(DwBinaryStream *binary)
{
	QByteArray bytes;
	if(this->readByteArray(&bytes)) {
		binary->fromByteArray(bytes);
		return true;
	}
	return false;
}

QString DwBinaryStream::readString()
{
	int len = getBlobLength();
	if (len == 0)
	{
		return "";
	}
	else if (len > 0 && remains() >= len)
	{
		QString res = QString::fromUtf8(constData(), this->toStringLength(len));
		chop(len);
		return res;
	}

	DW_ASSERT_X(false, __FUNCTION__, "readString error.");
	//engine()->currentContext()->throwError("readString error.");
	return ""; //we've got nothing

}

QString DwBinaryStream::readFullUTF8String()
{
	QString res = QString::fromUtf8(constData(), remains());
	chop(remains());
	return res;
}

bool DwBinaryStream::writeFullUTF8String(const QString &str)
{
	QByteArray bytes = str.toUtf8();
	reserve(bytes.size());
	m_data.append(bytes.constData(), bytes.size());
	return true;
}


void DwBinaryStream::clear()
{
	m_offset = 0;
	m_data.clear();
}

QByteArray DwBinaryStream::getByteArray() const
{
	if(m_offset == 0)
		return m_data;
	return QByteArray(constData(), remains());
}

bool DwBinaryStream::fromByteArray(const QByteArray& bytes)
{
	m_data = bytes;
	m_offset = 0;
	return true;
}

QString DwBinaryStream::toString() const
{
	return QString(getByteArray());
}

int DwBinaryStream::length() const
{
	return remains();
}

bool DwBinaryStream::copy( DwBinaryStream *binary )
{
	const char *data = constData();
	binary->m_data.append(data, remains());
	return true;
}


DwBinaryStream32::DwBinaryStream32(QObject *parent)
	: DwBinaryStream(parent) {
}

DwBinaryStream32::DwBinaryStream32(const QByteArray &rhs, QObject *parent)
	: DwBinaryStream(rhs, parent) {
}

//virtual
int DwBinaryStream32::putBlobLength(int length) {
	writeUInt(length + 1);
	return length + 1;
}

//virtual
int DwBinaryStream32::getBlobLength() {
	return readUInt();
}

//virtual
int DwBinaryStream32::toStringLength(int len) {
	//openplatform suckkkkkkkkk null-term wire format
	return len - 1;
}


DwBinaryStream32BigEndian::DwBinaryStream32BigEndian(QObject *parent)
	: DwBinaryStream(parent) {
}

DwBinaryStream32BigEndian::DwBinaryStream32BigEndian(const QByteArray &rhs, QObject *parent)
	: DwBinaryStream(rhs, parent) {
}

//virtual
int DwBinaryStream32BigEndian::putBlobLength(int length) {
	writeUInt(length);
	return length;
}

//virtual
int DwBinaryStream32BigEndian::getBlobLength() {
	return readUInt();
}

//virtual
int DwBinaryStream32BigEndian::toStringLength(int len) {
	//openplatform suckkkkkkkkk null-term wire format
	return len;
}

//virtual
void DwBinaryStream32BigEndian::readWithEndianAdjustment(void *dst, int s)
{
	READ_FROM_BIG_ENDIAN(dst, s);
};

//virtual
void DwBinaryStream32BigEndian::writeWithEndianAdjustment(void *src, int s)
{
	WRITE_TO_BIG_ENDIAN(src, s);
};
