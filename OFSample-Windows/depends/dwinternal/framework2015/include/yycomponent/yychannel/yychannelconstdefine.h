#pragma once


//公屏尾灯相关定义
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
<img id="sendImage" url="xxxx" data="xxx"/> //id 数字内部专用，对应YYMessageItem::ItemType，其他为组件别名
<txt data="123" />
...
<extra id="gameteam"> ////为组件别名
<txt pos=1 data="xxx" url="xxx"/> // 0为公屏消息昵称行前面，1为后面，2为内容行前面，3为后面
<img pos=1 data="xxx" />
</extra>

<extra id="gameteam2">
<txt pos=1 data="xxx"/>
<img pos=1 data=zhao"xxx"/>
</extra>
</msg>
*/