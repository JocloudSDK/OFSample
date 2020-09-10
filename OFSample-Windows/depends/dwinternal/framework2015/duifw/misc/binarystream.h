
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

class DW_DUIFW_EXPORT DwBinaryStream : public QObject
{
	Q_OBJECT
		Q_DISABLE_COPY(DwBinaryStream);

public:
	DwBinaryStream(QObject *parent = NULL);
	DwBinaryStream(const QByteArray &rhs, QObject *parent = NULL);
	~DwBinaryStream();

	Q_PROPERTY(int length READ length)

public:
	QByteArray getByteArray() const;
	bool fromByteArray(const QByteArray& bytes);

	int length() const;

public:
	Q_INVOKABLE bool writeByte(unsigned char b);
	Q_INVOKABLE bool writeInt16(int b);
	Q_INVOKABLE bool writeUInt16(unsigned int b);
	Q_INVOKABLE bool writeInt(int n);
	Q_INVOKABLE bool writeUInt(unsigned int n);
	Q_INVOKABLE bool writeInt64(qlonglong n);
	Q_INVOKABLE bool writeUInt64(qulonglong n);
	Q_INVOKABLE bool writeBool(bool b);
	Q_INVOKABLE bool writeReal(double d);
	Q_INVOKABLE bool writeBinary(const DwBinaryStream *binary);
	Q_INVOKABLE bool writeString(const QString &str);
	Q_INVOKABLE bool writeRaw(const DwBinaryStream *binary);

	Q_INVOKABLE unsigned char readByte();
	Q_INVOKABLE int readInt16();
	Q_INVOKABLE unsigned int readUInt16();
	Q_INVOKABLE int readInt();
	Q_INVOKABLE unsigned int readUInt();
	Q_INVOKABLE qlonglong readInt64();
	Q_INVOKABLE qulonglong readUInt64();
	Q_INVOKABLE bool readBool();
	Q_INVOKABLE double readReal();
	Q_INVOKABLE bool readBinary(DwBinaryStream *binary);
	Q_INVOKABLE QString readString();
	Q_INVOKABLE bool readByteArray(QByteArray *bytes);

	Q_INVOKABLE bool copy(DwBinaryStream *binary);

	Q_INVOKABLE void clear();
	Q_INVOKABLE QString toString() const;

	//这两个函数专门为陆铭那边提供的，避免他那边的插件在移植到4.0的时候做过多修改
	//这两个函数将内容全部当做一个utf8字符串处理，且最开始没有长度字段。
	Q_INVOKABLE bool writeFullUTF8String(const QString &str);
	Q_INVOKABLE QString readFullUTF8String();

protected:
	virtual int putBlobLength(int length);
	virtual int getBlobLength();
	virtual int toStringLength(int len);
	virtual void readWithEndianAdjustment(void *des, int s);
	virtual void writeWithEndianAdjustment(void *src, int s);

	inline void readNoSwapped(void *des, int s);
	inline void writeNoSwapped(void *src, int s);

	inline void readSwapped(void *des, int s);
	inline void writeSwapped(void *src, int s);

private:
	bool writeByteArray(const QByteArray &binary);
	bool read(void *des, int s);
	bool write(void *src, int s);

	inline void chop(int s);
	inline void reserve(int s);
	inline int remains() const;
	inline const char* constData() const;

private:
	QByteArray m_data;
	int m_offset;
};

//with 32bit blob header
class DW_DUIFW_EXPORT DwBinaryStream32 : public DwBinaryStream
{
	Q_OBJECT
		Q_DISABLE_COPY(DwBinaryStream32);

public:
	DwBinaryStream32(QObject *parent = NULL);
	DwBinaryStream32(const QByteArray &rhs, QObject *parent = NULL);

	virtual int putBlobLength(int length);
	virtual int getBlobLength();
	virtual int toStringLength(int len);
};

//with 32bit big-endian blob header
class DW_DUIFW_EXPORT DwBinaryStream32BigEndian : public DwBinaryStream
{
	Q_DISABLE_COPY(DwBinaryStream32BigEndian );

public:
	DwBinaryStream32BigEndian (QObject *parent = NULL);
	DwBinaryStream32BigEndian (const QByteArray &rhs, QObject *parent = NULL);

	virtual int putBlobLength(int length);
	virtual int getBlobLength();
	virtual int toStringLength(int len);
	virtual void readWithEndianAdjustment(void *des, int s);
	virtual void writeWithEndianAdjustment(void *src, int s);
};
