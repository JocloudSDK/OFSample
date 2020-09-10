#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

DWDEFINE_INTERFACE(IYYAccountInfo) : public IUnk
{
	virtual QString accountName() = 0;
	virtual QString savedHashPassword() = 0;
	virtual LOGINTYPE loginType() = 0;
	virtual QString logoUrl() = 0;
	virtual int logoIndex() = 0;
	virtual IM_STATUS imStatus() = 0;
	virtual bool isAutoLogin() = 0;
	virtual bool isRememberHashPassword() = 0;

	virtual QString token() const = 0;
	virtual QString passport() const = 0;
	virtual QString accountInfo() const = 0;
	virtual bool isRememberFacebookAccount() const = 0;

	virtual QString credit(void) = 0;
	virtual quint32 uid(void) = 0;
	virtual bool isRememberHashPassword2() = 0;
};

DWDEFINE_INTERFACE(IYYAccountManager) : public IDWQComponent
{
	virtual void initAccountList(LOGINTYPE type) = 0;
	virtual void setCurrentUid(quint32 uid) = 0;
	virtual IYYAccountInfoPtr currentAccountInfo() = 0;
	virtual void setCurrentAutologin(bool isAutoLogin) = 0;
	virtual void saveCurrentAccount() = 0;
	virtual void setCurrentAccountName(const QString &accountName, LOGINTYPE type) = 0;
	virtual void setCurrentImStatus( IM_STATUS imStatus ) = 0;
	virtual void initLastAccount(LOGINTYPE type) = 0;
	virtual void setCurrentAccountIndex( int index ) = 0;
	virtual void setCurrentRememberPassword( bool isRememberPassword ) = 0;
	virtual void setCurrentHashPassword( const QString &hashPassword ) = 0;
	virtual QList<IYYAccountInfoPtr> accountList() = 0;
	virtual void removeAccount( const QString &accountName, bool deleteInfo) = 0;
	virtual void finishLoadAccountList(LOGINTYPE type) = 0;
	virtual void updateAccountLogo( const QString &account, int logoIndex, const QString &logoUrl ) = 0;
	virtual bool isEasyPassword(const QString& passwordPlainText) = 0;

	//login by 3rd
	virtual void setLoginType(LOGINTYPE type) = 0;
	virtual void setCurrentPassport(const QString & passport) = 0;
	virtual void setCurrentToken(const QString &token) = 0;
	virtual void setCurrentAccountInfo(const QString &accountInfo) = 0;

	virtual void setCurrentLoginType(LOGINTYPE loginType) = 0;
	virtual LOGINTYPE currentLoginType() const = 0;
	virtual LOGINTYPE latestLoginType() = 0;

	virtual bool currentRememberFacebookAccount() = 0;
	virtual void setCurrentRememberFacebookAccount( bool isRememberFacebookAccount ) = 0;

	virtual void cleanAccountList() = 0;
	virtual void cleanCurrentAccount() = 0;

	virtual bool currentIsAutoLogin() = 0;

	virtual void setCurrentCredit(const QString& credit) = 0;
	virtual int getAccountInfoIndex(const QString& accountName) = 0;

	virtual void setCurrentRememberPassword2( bool isRememberPassword ) = 0;

    // 
    virtual IYYAccountInfoPtr account(const QString& account_name, const QString& logined_passport, quint32 uid) = 0;
    virtual IYYAccountInfoPtr setCurrentAccount(const QString& account_name, const QString& logined_passport, quint32 uid) = 0;

//signals:
	//void accountListChanged();
	//void currentAccountInfoChanged();
};