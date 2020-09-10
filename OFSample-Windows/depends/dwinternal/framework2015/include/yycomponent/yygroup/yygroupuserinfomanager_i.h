
#pragma once

#include "dwbase/dwcomex.h"

class DuiWidget;
class DuiSettingPageBase;
DWDEFINE_INTERFACE(IYYGroupUserinfoManager) : public IDWQComponent
{
    virtual void showUserInfoFrame(UINT32 gid, UINT32 uid) = 0;

    //以后需要的话，再把单个userinfoFrame接口开出来
    enum UserInfoPos
    {
        POS_USERINFO_BEHIND_IMID = 0
    };
    virtual bool insertWidget(UINT32 uid, DuiWidget* widget, UserInfoPos pos) = 0;

    virtual bool insertPage(UINT32 uid, const QString& strTabText, const QString& strIconPath, const QString& strPageName, DuiSettingPageBase* page) = 0;

};
