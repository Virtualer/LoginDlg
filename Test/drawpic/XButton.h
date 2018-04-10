#pragma once
#include "../Resource.h"
class CXButton
{
public:
	// 标准构造函数
	CXButton()
	{
		m_bCheck = FALSE;
		m_bHovering = FALSE;
		m_bPressed = FALSE;
		m_bDisabled = FALSE;
		m_nImageContorl	= 0;
		m_nAltImageContorl	= 0;
		m_bEnable	= TRUE;
		m_bFocus	= FALSE;
		m_hNextWnd	= NULL;
		m_nTabIndex	= -1;
		m_nFocusControl	= 0;
		m_bEnlargeDot	= FALSE;
		m_nUpIndex	= -1;
		m_nDownIndex = -1;
		m_nLeftIndex = -1;
		m_nRightIndex	= -1;
		m_bCommon = TRUE;
		m_nStyle = 0;
		m_bIsSetFocus = FALSE;
	}

	// 设置按钮矩形位置
	void SetRect(LPCRECT lpRect){m_rcRect = lpRect;};
	// 获取按钮矩形位置
	void GetRect(LPRECT lpRect){*lpRect = *m_rcRect;};
	// 检测指定点是否处于按钮内
	BOOL PtInButton(CPoint& point)
	{
		BOOL bPtInBtn = m_rcRect.PtInRect(point);
		if (m_bHandCursor && bPtInBtn && !m_bDisabled)
		{
			SetCursor(::LoadCursor(NULL, IDC_HAND));
		}

		return bPtInBtn;
	};
	// 绘制按钮
	void DrawButton( CDC *pDC, Gdiplus::Graphics&  graphics);
	// 创建按钮
	void CreateButton(CWnd* pParent, LPCTSTR pszTitle, LPRECT lpRect, UINT nImage, UINT nCount = 4, UINT sAltImage = 0, UINT nAltCount = 0, UINT nID = 0, UINT nTextOffset = 0, Gdiplus::StringAlignment align = StringAlignmentCenter, Gdiplus::StringAlignment linealign = StringAlignmentCenter, Color TextColor = Color::Black, BOOL bAltColor = TRUE, int nTextHeight = 16, BOOL bUnderline = FALSE, BOOL bHandCursor = FALSE, BOOL bEnlargeDot = FALSE);
	// 设置按钮状态
	void EnableButton(BOOL bEnable, BOOL bCommon = TRUE);

	BOOL IsDisabled()
	{
		return m_bDisabled;
	}

	// 设置按钮隐藏或可见
	void ShowButton(BOOL bEnable)
	{
		m_bEnable = bEnable;
		if (m_bFocus == TRUE)
		{
			m_bFocus = FALSE;
		}
		m_pParent->InvalidateRect(m_rcRect);
	}

	// 设置按钮选定状态
	void SetCheck(BOOL bCheck)
	{
		m_bCheck = bCheck;
		m_pParent->InvalidateRect(m_rcRect);
	}

	BOOL GetCheck()
	{
		return m_bCheck;
	}

	void SetStyle(int nStyle)
	{
		m_nStyle = nStyle;
	}

	int GetStyle()
	{
		return m_nStyle;
	}

	BOOL GetShow() {return m_bEnable;}

	void SetText(LPCTSTR pszTitle)
	{
		if(m_strBtnTitle.Compare(pszTitle))
		{
			m_strBtnTitle = pszTitle;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}

	CString GetBtnTitle()
	{
		return m_strBtnTitle;
	}

	void SetFocus(BOOL bFocus)
	{
		if(m_bEnable)
		{
			m_bFocus = bFocus;
			m_pParent->InvalidateRect(m_rcRect);
		}
		m_bIsSetFocus = TRUE;
	}
	void SetBtnTipText(CString strBtnTipText)
	{
		m_strBtnTipText = strBtnTipText;
	}
	CString GetBtnTipText()
	{
		return m_strBtnTipText;
	}
	BOOL GetFocus()
	{
		return m_bFocus;
	}

	void SetNextWnd(HWND hWnd)
	{
		m_hNextWnd = hWnd;
	}

	HWND GetNextWnd()
	{
		return m_hNextWnd;
	}

	void SetTabIndex(int nTabIndex)
	{
		m_nTabIndex = nTabIndex;
	}

	int GetTabIndex()
	{
		return m_nTabIndex;
	}

	void SetUpIndex(int nIndex)
	{
		m_nUpIndex = nIndex;
	}

	int GetUpIndex()
	{
		return m_nUpIndex;
	}

	void SetDownIndex(int nIndex)
	{
		m_nDownIndex = nIndex;
	}

	int GetDownIndex()
	{
		return m_nDownIndex;
	}

	void SetLeftIndex(int nIndex)
	{
		m_nLeftIndex = nIndex;
	}

	int GetLeftIndex()
	{
		return m_nLeftIndex;
	}

	void SetRightIndex(int nIndex)
	{
		m_nRightIndex = nIndex;
	}

	int GetRightIndex()
	{
		return m_nRightIndex;
	}

	void SetFocusControl(int nFocusControl)
	{
		m_nFocusControl = nFocusControl;
	}

	int GetFocusControl()
	{
		return m_nFocusControl;
	}

	// 获取按钮Id
	UINT GetControlId(){return m_nID;}
	// 触发按钮事件
	void DoCommand()
	{
		if(!m_bDisabled && m_bEnable)
		{
			m_bCheck = !m_bCheck;
			m_pParent->InvalidateRect(m_rcRect);
			m_pParent->PostMessage(WM_COMMAND, m_nID);
		}
	}

	// 鼠标按下按钮事件
	void LButtonDown()
	{
		if(!m_bDisabled && m_bEnable)
		{
			m_bPressed = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	};
	// 鼠标放下按钮事件
	void LButtonUp()
	{
		if(!m_bDisabled && m_bEnable)
		{
			if(m_bPressed == TRUE)
			{
				m_bCheck = !m_bCheck;
				m_bPressed = FALSE;
				m_pParent->InvalidateRect(m_rcRect);
				m_pParent->PostMessage(WM_COMMAND, m_nID);
			}
		}
	}
	// 鼠标进入按钮区域事件
	void MouseHover()
	{
		if(!m_bDisabled && m_bEnable)
		{
			m_bHovering = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}
	// 鼠标离开按钮区域事件
	void MouseLeave()
	{
		if(!m_bDisabled && m_bEnable)
		{
			m_bPressed = FALSE;
			m_bHovering = FALSE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}
private:
	// 按钮矩形位置
	CRect	m_rcRect;
	// 标识鼠标进入按钮区域
	BOOL	m_bHovering;
	// 标识已经点击按钮
	BOOL	m_bPressed;
	// 标识按钮已经被禁用
	BOOL m_bDisabled;
	// 标识按钮可见
	BOOL m_bEnable;
	// 标识按钮已经被选中
	BOOL	m_bCheck;
	// 按钮图片编号
	UINT	m_nImageContorl;
	// 按钮按下状态图片编号
	UINT	m_nAltImageContorl;
	// 按钮图片中包含的图片个数
	UINT	m_nImageCount;
	// 按钮选中图片中包含的图片个数
	UINT	m_nAltImageCount;
	// 按钮Id号
	int		m_nID;
	// 按钮父窗口
	CWnd*	m_pParent;
	// 按钮文字
	CString m_strBtnTitle;
	// 按钮文字偏移
	UINT m_nTextOffset;
	// 按钮文字对齐方式
	Gdiplus::StringAlignment m_TextAlign;
	// 按钮文字对齐方式
	Gdiplus::StringAlignment m_TextLineAlign;
	// 按钮文字颜色
	Color m_TextColor;
	// 切换文字颜色
	BOOL m_bAltColor;
	// 按钮文字高度
	int m_nTextHeight;
	// 按钮文字下划线
	BOOL m_bUnderline;
	// 手形鼠标光标
	BOOL m_bHandCursor;
	// 焦点
	BOOL m_bFocus;
	// 下一个控件（目前是标准控件）
	HWND m_hNextWnd;		
	// 按下Tab键跳转，存储下一个编辑框容器下标索引
	int m_nTabIndex;		
	// 参见枚举FocusControl
	int m_nFocusControl;			
	// 使用放大的虚框（tab）
	BOOL m_bEnlargeDot;
	// 上键跳转，存储下一个按钮下标索引
	int m_nUpIndex;
	// 下键跳转，存储下一个按钮下标索引
	int m_nDownIndex;
	// 左键跳转，存储下一个按钮下标索引
	int m_nLeftIndex;
	// 右键跳转，存储下一个按钮下标索引
	int m_nRightIndex;
	// 禁用时按钮及文字颜色是否采用常规，为智能USB保护定制
	BOOL m_bCommon;
	//tip
	CString m_strBtnTipText;
	// 按钮样式
	int m_nStyle;
	//按钮对应的标准控件ID
	int m_nStanID;
	//焦点是否已经设置
	BOOL m_bIsSetFocus;
};
