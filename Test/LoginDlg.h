
// TestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

// CLoginDlg 对话框
class CLoginDlg : public CDialogEx
{
// 构造
public:
	CLoginDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CLoginDlg();
// 对话框数据
    enum { IDD = IDD_DLG_LOGIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    Image*m_pMainBG;
    CEdit m_editUserName;//用户名输入框
    CEdit m_editPsw;//用户密码输入框
    vector<CXButton> m_xButtonList;//按钮列表
    vector<CRect> m_xBtnRect;//按钮矩形列表
    int m_nBtnHoving;//鼠标所在按钮编号
    COLORREF m_colorText;//编辑框文本颜色
    BOOL m_bRemPswCheck;//记住密码按钮勾选
    CString m_strLoginOrReg;//登录按钮显示“登录”或者“注册”
public:
    void GetSystemDpi();//获取系统DPI
    void InitWindow();//初始化窗口
    void InitButtons();//初始化按钮
    void InitEdits();//初始化编辑框

    void OnBtnClose();//关闭按钮功能
    void OnBtnLogin();//登录按钮功能
    void OnBtnRemPsw();//记住密码复选框
    void OnBtnForgetPsw();//忘记密码链接按钮

    int IsButtonFocus();//焦点是否在按钮上
    int XButtonHitTest(CPoint point, CRect&rtButton);//测试是否点击在按钮上
    int XButtonMeasureString(const WCHAR *string, INT length, RectF *boundingBox, int nFontSize/*负数*/);//测量按钮文本
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnUserClickPswEdit();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
