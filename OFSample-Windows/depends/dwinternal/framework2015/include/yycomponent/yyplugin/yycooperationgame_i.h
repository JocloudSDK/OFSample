#pragma once
#include "dwbase/dwcomex.h"

struct PCooperationGameInfo;

/**
  * @brief ������Ϸ�ӿ�, use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYCooperationGame): public IUnk
{
	struct CoopGameInfo
	{
		int     id;
		QString name;
		QString runName;	//������Ϸ�����е���Ϸ
		bool    playing;
	};

    struct CoopGameInfo2
    {
        int     id;
        QString name;
        QString runName;	//������Ϸ�����е���Ϸ
        bool    playing;
        QString qufu; // ����Ϸ��������
    };

/**
   * @brief ��ȡ������Ϸ��Ϣ.
   * @param .
   * @return .
   */
	virtual bool getCooperationGameInfo(PCooperationGameInfo& info) = 0;
/**
   * @brief ������ʷ��Ϸ��Ϣ.
   */
	virtual void loadHistoryGameInfo() = 0;
/**
   * @brief ͨ����Ϸ���ֻ�ȡ��Ϣ.
   * @param gameName ��Ϸ����.
   * @param info [output] ��Ϸ��Ϣ.
   * @return .
   */
	virtual bool getGameInfoByName(const QString& gameName, PCooperationGameInfo& info) = 0;
/**
   * @brief ����������Ϸ�������˷Ǻ�����Ϸ.
   * @return ��Ϸ����.
   */
	virtual QString getPlayedGameName() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getCooperationGameInfo(IYYCooperationGame::CoopGameInfo& info) = 0;

/**
   * @brief ����������Ϸ�������ڱ���.
   * @param nameΪ��Ϸ�����������硰�����á�.
   * @return .
   */
    virtual void playNewGame(const std::wstring &name) = 0;

    /**
   * @brief 6.11����������.
   * @param name Ϊ������Ϸ, qufuΪ������Ϸ���ڵ���������s1,s2,��k1,k2��kָ���������extDataΪ��չ�ֶΣ���δʹ��.
   * @return .
   */
    virtual void playNewGameInfo(const std::wstring &name, const std::wstring &qufu, const std::wstring &extData) = 0;

    /**
   * @brief 6.11���������ֶ�.
   * @param CoopGameInfo2 ����������Ľṹ�壬ԭ��CoopGameInfo������.
   * @return .
   */
    virtual bool getCooperationGameInfo2(IYYCooperationGame::CoopGameInfo2& info) = 0;

    /**
   * @brief 6.15���ӽӿڣ���ȡ���ܵ���Ϸurl.
   * @param id ��Ϸid.
   * @return �����˵�url.
   */
    virtual QString getGameUrlEncodedById(int id) const = 0;

};