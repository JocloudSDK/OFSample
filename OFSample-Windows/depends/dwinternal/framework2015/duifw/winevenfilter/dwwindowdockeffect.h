#pragma once
#include "dwwinevenfilter_i.h"

//------------------------------------------------------------------------------
class DuiFrameWindow;
class DWWindowDockEffectPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWWindowDockEffect : public DWAbstractWinEvenFilter
{
	Q_OBJECT
	Q_DISABLE_COPY(DWWindowDockEffect)
	Q_DECLARE_PRIVATE(DWWindowDockEffect)

public:
	explicit DWWindowDockEffect(DuiFrameWindow *window);
	virtual ~DWWindowDockEffect();

	virtual bool winEvent(MSG *message, long *result);

	bool IsHideToEdge(void)const;

	// 从靠边隐藏状态弹出显示
	// isMouseLeaveTrigger - 界面展开后，鼠标离开界面访问是否会触发隐藏
	// 目前用快捷键或双击任务栏YY图标呼出界面时需要传递false
	void ToShowFromEdge(bool isMouseLeaveTrigger = true);
	bool IsCloseToEdge(void)const;
	void ToHideToEdge(void);

	void ToShowFromEdgeImmediately();
	void setAlwaysOnTopState(bool topMost);
	void setChildBlock(bool childBlock );

    void setEnabled( bool b );
    bool enabled() const;

private slots:
	void on_timeout();
	void on_showOrHideWindow();

	// 在主界面完成隐藏后，恢复鼠标离开触发动作
	void on_doneHide();

private:
	enum HideMode
	{
		HM_NONE,		//没有隐藏并且不停靠在窗口边缘
		HM_TOP,			//隐藏在窗口上面
		HM_LEFT,		//隐藏在窗口左边
		HM_RIGHT,		//隐藏在窗口右边
		HM_TOP_EDGE,	//停靠在窗口上边缘
		HM_LEFT_EDGE,	//停靠在窗口左边缘
		HM_RIGHT_EDGE,	//停靠在窗口右边缘
	};

	// 表示粘附在屏幕的周边
	enum CloseToBoundary
	{
		CTB_Left = 0,
		CTB_Top,
		CTB_Right,
		CTB_None
	};

	struct AreaInfo
	{
		LONG left;
		LONG top;
		LONG right;
		LONG bottom;

		AreaInfo() : left(0), top(0), right(0), bottom(0){}
		void reset()
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
	};

private:
	void _fixMovingRect(UINT fwSide, LPRECT pRect);
	void _toShowOrHideWindow();
	void _doHide();
	void _doShow();
	void _setTopMost(bool bTopMost);

	bool _isDragFullWindows();
	void _saveWindowRect(LPRECT rect);

	void _updateAreaInfo();
	void _resetAreaInfo();

	bool _handleMoving(UINT fwSide, LPRECT pRect);
	bool _handleMove(int x, int y);
	bool _handleHitClient(QPoint point);
	bool _handleMaxMinInfo();

	CloseToBoundary _checkColseToBoundary();

private:
	DWWindowDockEffectPrivate* d_ptr;
};