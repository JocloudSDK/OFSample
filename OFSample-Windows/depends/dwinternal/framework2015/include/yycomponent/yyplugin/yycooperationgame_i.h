#pragma once
#include "dwbase/dwcomex.h"

struct PCooperationGameInfo;

/**
  * @brief 合作游戏接口, use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYCooperationGame): public IUnk
{
	struct CoopGameInfo
	{
		int     id;
		QString name;
		QString runName;	//启动游戏大厅中的游戏
		bool    playing;
	};

    struct CoopGameInfo2
    {
        int     id;
        QString name;
        QString runName;	//启动游戏大厅中的游戏
        bool    playing;
        QString qufu; // 在游戏所在区服
    };

/**
   * @brief 获取合作游戏信息.
   * @param .
   * @return .
   */
	virtual bool getCooperationGameInfo(PCooperationGameInfo& info) = 0;
/**
   * @brief 加载历史游戏信息.
   */
	virtual void loadHistoryGameInfo() = 0;
/**
   * @brief 通过游戏名字获取信息.
   * @param gameName 游戏名字.
   * @param info [output] 游戏信息.
   * @return .
   */
	virtual bool getGameInfoByName(const QString& gameName, PCooperationGameInfo& info) = 0;
/**
   * @brief 最近在玩的游戏，包括了非合作游戏.
   * @return 游戏名字.
   */
	virtual QString getPlayedGameName() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getCooperationGameInfo(IYYCooperationGame::CoopGameInfo& info) = 0;

/**
   * @brief 更新所玩游戏，保存在本地.
   * @param name为游戏的中文名，如“弹弹堂”.
   * @return .
   */
    virtual void playNewGame(const std::wstring &name) = 0;

    /**
   * @brief 6.11新增加内容.
   * @param name 为所玩游戏, qufu为所玩游戏所在的区服，如s1,s2,或k1,k2，k指快快区服，extData为扩展字段，暂未使用.
   * @return .
   */
    virtual void playNewGameInfo(const std::wstring &name, const std::wstring &qufu, const std::wstring &extData) = 0;

    /**
   * @brief 6.11增加区服字段.
   * @param CoopGameInfo2 增加区服后的结构体，原来CoopGameInfo升级版.
   * @return .
   */
    virtual bool getCooperationGameInfo2(IYYCooperationGame::CoopGameInfo2& info) = 0;

    /**
   * @brief 6.15增加接口，获取加密的游戏url.
   * @param id 游戏id.
   * @return 加密了的url.
   */
    virtual QString getGameUrlEncodedById(int id) const = 0;

};