#pragma once
#include "stable.h"

enum EnumEqualWay
{
	enumOperatorAssign,
	enumOperatorAND,
	enumOperatorOR,
};

/**@brief ö�ٹ���
*	��϶��ö��״̬������ѡ���Ĺ��� EqualWay ���в���
*	ö��ֵ�����������Ƚϣ��������������㣬��������������
*	���ϲ���������
*			MakeCombineEnum& operator = (TEnum valEnum)
*			isAllEnum(TEnum valEnum) 
*			isAnyEnum(TEnum valEnum)
*
*/
template<typename TEnum, TEnum invalidEnum, qint32 EnumNum, EnumEqualWay EqualWay = enumOperatorAssign>
class MakeCombineEnum
{
public:
	MakeCombineEnum()
	{
		for (qint32 curIndex=0; curIndex < EnumNum; ++curIndex)
		{
			enums[curIndex] = invalidEnum;
		}
	}

	MakeCombineEnum& operator = (TEnum valEnum)
	{
		for (qint32 curIndex=0; curIndex < EnumNum; ++curIndex)
		{
			setEnumOf<EqualWay>(index, valEnum);
		}
		return *this;
	}

	bool isAllEnum(TEnum valEnum)
	{
		for (qint32 curIndex=0; curIndex < EnumNum; ++curIndex)
		{
			if( ! equalOf<EqualWay>(curIndex, valEnum) ) { return false; }
		}

		return true;
	}

	bool isAnyEnum(TEnum valEnum)
	{
		for (qint32 curIndex=0; curIndex < EnumNum; ++curIndex)
		{
			if(equalOf<EqualWay>(curIndex, valEnum) ) { return true; }
		}

		return false;
	}

	TEnum	enumOf(qint32 index)
	{
		return enums[index];
	}

	void setEnum(qint32 index, TEnum valEnum)
	{
		setEnumOf<EqualWay>(index, valEnum);
	}

protected:
	template<EnumEqualWay equalWay>
	void setEnumOf(qint32 index, TEnum valEnum);

	template<>
	void setEnumOf<enumOperatorAssign>(qint32 index, TEnum valEnum)
	{
		enums[index] = valEnum;
	}

	template<>
	void setEnumOf<enumOperatorAND>(qint32 index, TEnum valEnum)
	{
		enums[index] &= valEnum;
	}

	template<>
	void setEnumOf<enumOperatorOR>(qint32 index, TEnum valEnum)
	{
		enums[index] |= valEnum;
	}

	template<EnumEqualWay equalWay>
	bool equalOf(qint32 index, TEnum valEnum);

	template<>
	bool equalOf<enumOperatorAssign>(qint32 index, TEnum valEnum)
	{
		return (enumOf(index) == valEnum);
	}

	template<>
	bool equalOf<enumOperatorAND>(qint32 index, TEnum valEnum)
	{
		return (enumOf(index) & valEnum);
	}

	template<>
	bool equalOf<enumOperatorOR>(qint32 index, TEnum valEnum)
	{
		return (enumOf(index) | valEnum);
	}
private:
	TEnum enums[EnumNum];
};