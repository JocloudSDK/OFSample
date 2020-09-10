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

	// �ӿ�������״̬������ʾ
	// isMouseLeaveTrigger - ����չ��������뿪��������Ƿ�ᴥ������
	// Ŀǰ�ÿ�ݼ���˫��������YYͼ���������ʱ��Ҫ����false
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

	// ��������������غ󣬻ָ�����뿪��������
	void on_doneHide();

private:
	enum HideMode
	{
		HM_NONE,		//û�����ز��Ҳ�ͣ���ڴ��ڱ�Ե
		HM_TOP,			//�����ڴ�������
		HM_LEFT,		//�����ڴ������
		HM_RIGHT,		//�����ڴ����ұ�
		HM_TOP_EDGE,	//ͣ���ڴ����ϱ�Ե
		HM_LEFT_EDGE,	//ͣ���ڴ������Ե
		HM_RIGHT_EDGE,	//ͣ���ڴ����ұ�Ե
	};

	// ��ʾճ������Ļ���ܱ�
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