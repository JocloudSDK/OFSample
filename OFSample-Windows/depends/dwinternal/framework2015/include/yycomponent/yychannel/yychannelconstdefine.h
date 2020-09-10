#pragma once


//����β����ض���
const wchar_t KTagRoot[]	= L"msg";
const wchar_t KTagText[]	= L"txt";
const wchar_t KTagImage[]	= L"img";
const wchar_t KTagExtra[]	= L"extra";
const wchar_t KTagProg[]	= L"prop";
const wchar_t KTagSender[]	= L"sender";

const wchar_t KAttrID[]      = L"id";
const wchar_t KAttrUrl[]     = L"url";
const wchar_t KAttrData[]    = L"data";
const wchar_t KAttrPos[]     = L"pos";
const wchar_t KAttrIsCache[] = L"isCache";
const wchar_t KAttrTooltip[] = L"tooltip";
const wchar_t KAttrPriority[] = L"priority";
const wchar_t KAttrNickColor[] = L"nickColor";


/** the xml element

<msg>
<txt data="abc" />
<img id="sendImage" url="xxxx" data="xxx"/> //id �����ڲ�ר�ã���ӦYYMessageItem::ItemType������Ϊ�������
<txt data="123" />
...
<extra id="gameteam"> ////Ϊ�������
<txt pos=1 data="xxx" url="xxx"/> // 0Ϊ������Ϣ�ǳ���ǰ�棬1Ϊ���棬2Ϊ������ǰ�棬3Ϊ����
<img pos=1 data="xxx" />
</extra>

<extra id="gameteam2">
<txt pos=1 data="xxx"/>
<img pos=1 data=zhao"xxx"/>
</extra>
</msg>
*/