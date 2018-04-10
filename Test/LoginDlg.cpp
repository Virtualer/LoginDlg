
// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_BTN_CLOSE     0xEFF0
#define IDC_BTN_REMPSW    0xEFF1
#define IDC_BTN_LOGIN     0xEFF2
#define IDC_BTN_FORGETPSW 0xEFF3

// CLoginDlg 对话框
double g_dDpi;


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nBtnHoving = -1;
    m_bRemPswCheck = TRUE;
    m_strLoginOrReg = _T("登录");
}

CLoginDlg::~CLoginDlg()
{
    if (m_pMainBG)
    {
        delete m_pMainBG;
        m_pMainBG = NULL;
    }
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_USER, m_editUserName);
    DDX_Control(pDX, IDC_EDIT_PSW, m_editPsw);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_COMMAND(IDC_BTN_CLOSE,&CLoginDlg::OnBtnClose)
    ON_COMMAND(IDC_BTN_LOGIN, &CLoginDlg::OnBtnLogin)
    ON_COMMAND(IDC_BTN_REMPSW, &CLoginDlg::OnBtnRemPsw)
    ON_COMMAND(IDC_BTN_FORGETPSW, &CLoginDlg::OnBtnForgetPsw)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_PSW, &CLoginDlg::OnUserClickPswEdit)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    m_colorText = RGB(0, 0, 0);

    GetSystemDpi();
    InitWindow();
    InitButtons();
    InitEdits();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoginDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // 用于绘制的设备上下文
    //
	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
    //
	//	// 使图标在工作区矩形中居中
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;
    //
	//	// 绘制图标
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialogEx::OnPaint();
	//}

    // 使用UNICODE字符转换
    USES_CONVERSION;

    CPaintDC dc(this);
    CMemoryDC dcMem(&dc);

    //获取窗口矩形位置
    CRect rtClient;
    GetClientRect(rtClient);

    //获取重绘矩形位置
    CRect rcPaint;
    dcMem.GetClipBox(&rcPaint);

    Gdiplus::Graphics graphics(dcMem.m_hDC);

    m_pMainBG = CImageInfo::Instance()->ImageFromResource(IDB_PNG_LOGINBG);
    graphics.DrawImage(m_pMainBG, 0, 0, m_pMainBG->GetWidth()*g_dDpi, m_pMainBG->GetHeight()*g_dDpi);

    //绘制编辑框左侧文本
    StringFormat stringFormat;
    stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
    stringFormat.SetAlignment(StringAlignmentNear);
    stringFormat.SetLineAlignment(StringAlignmentNear);

    RectF rectString;
    SolidBrush brushLeft(Color(255, 105, 105, 105));
    LOGFONT lfFont;
    memset(&lfFont, 0, sizeof(lfFont));
    lfFont.lfHeight = -11 * g_dDpi;
    lfFont.lfWeight |= FW_NORMAL;
    StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName), _T("微软雅黑"));
    Gdiplus::Font ftLeft(dcMem.GetSafeHdc(), &lfFont);

    CString strLeft;
    RectF rcString;

    strLeft = _T("用户名:");
    graphics.MeasureString(strLeft, strLeft.GetLength(), &ftLeft, PointF((float)0, (float)0), &stringFormat, &rcString);
    rectString.X = 120 * g_dDpi;
    rectString.Y = 132 * g_dDpi;
    rectString.Width = rcString.Width*g_dDpi;
    rectString.Height = rcString.Height*g_dDpi;
    graphics.DrawString(strLeft, strLeft.GetLength(), &ftLeft, rectString, &stringFormat, &brushLeft);

    strLeft = _T("密码:");
    rectString.Y = 162 * g_dDpi;
    graphics.DrawString(strLeft, strLeft.GetLength(), &ftLeft, rectString, &stringFormat, &brushLeft);

    //绘制按钮
    for (size_t i = 0; i< m_xButtonList.size(); i++)
    {
        CXButton& dcControl = m_xButtonList[i];
        CRect rcControl;
        dcControl.GetRect(&rcControl);
        dcControl.DrawButton(dcMem, graphics);
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//获取系统DPI
void CLoginDlg::GetSystemDpi()
{
    CDC *pDC = GetDC();
    float sysDpi = (float)pDC->GetDeviceCaps(LOGPIXELSX) / 96;
    ReleaseDC(pDC);
    g_dDpi = sysDpi;
}

//初始化窗口
void CLoginDlg::InitWindow()
{
    m_pMainBG = CImageInfo::Instance()->ImageFromResource(IDB_PNG_LOGINBG);

    //修改主窗口大小
    CRect cClientRect;
    GetClientRect(&cClientRect);
    cClientRect.right = m_pMainBG->GetWidth()*g_dDpi;
    cClientRect.bottom = m_pMainBG->GetHeight()*g_dDpi;
    MoveWindow(&cClientRect);
    CenterWindow(GetParent());

    //在界面上添加编辑框
    CWnd * pWnd;
    pWnd = GetDlgItem(IDC_EDIT_USER);
    pWnd->SetWindowPos(NULL, 170 * g_dDpi, 133 * g_dDpi, 200 * g_dDpi, 20 * g_dDpi, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_EDIT_PSW);
    pWnd->SetWindowPos(NULL, 170 * g_dDpi, 163 * g_dDpi, 200 * g_dDpi, 20 * g_dDpi, SWP_NOZORDER);
}

//初始化按钮
void CLoginDlg::InitButtons()
{
    m_xButtonList.clear();
    m_xBtnRect.clear();

    CRect cClientRect;
    GetClientRect(&cClientRect);

    CRect rtButton;
    CXButton dcControl;

    //关闭按钮
    rtButton.top = 12 * g_dDpi;
    rtButton.bottom = rtButton.top + 20 * g_dDpi;
    rtButton.left = 467 * g_dDpi;
    rtButton.right = rtButton.left + 20 * g_dDpi;
    dcControl.SetBtnTipText(_T("关闭"));
    dcControl.CreateButton(this, _T(""), rtButton, IDB_PNG_BTNCLOSE, 2, NULL, 0, IDC_BTN_CLOSE);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //记住密码复选框
    RectF rtBtn;
    CString strCheckBtn = _T("记住密码");
    XButtonMeasureString(strCheckBtn, strCheckBtn.GetLength(), &rtBtn, -12);

    rtButton.left = 170 *g_dDpi;
    rtButton.right = rtButton.left + (rtBtn.Width + 25)*g_dDpi;
    rtButton.top = 193 * g_dDpi;
    rtButton.bottom = rtButton.top + 18 * g_dDpi;
    dcControl.CreateButton(this, strCheckBtn, rtButton, IDB_PNG_CHECK, 2, IDB_PNG_UNCHECK, 2, IDC_BTN_REMPSW, 0, StringAlignmentFar, StringAlignmentCenter, Color::Black, FALSE, 12, 0, 0, 1);
    dcControl.SetStyle(1);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //登录按钮
    rtButton.top = 233 * g_dDpi;
    rtButton.bottom = rtButton.top + 30 * g_dDpi;
    rtButton.left = 170 * g_dDpi;
    rtButton.right = rtButton.left + 118 * g_dDpi;
    dcControl.SetBtnTipText(m_strLoginOrReg);
    dcControl.CreateButton(this, m_strLoginOrReg, rtButton, IDB_PNG_NORMALBTN, 4, NULL, 0, IDC_BTN_LOGIN, 0, StringAlignmentCenter, StringAlignmentCenter, Color::Black, 1, 14);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //忘记密码按钮
    CString strLinkBtn = _T("忘记密码");
    XButtonMeasureString(strLinkBtn, strLinkBtn.GetLength(), &rtBtn, -12);
    rtButton.top = 163 * g_dDpi;
    rtButton.bottom = rtButton.top + 20 * g_dDpi;
    rtButton.left = 370 * g_dDpi;
    rtButton.right = rtButton.left + rtBtn.Width*g_dDpi + 20*g_dDpi;
    dcControl.SetBtnTipText(strLinkBtn);
    dcControl.CreateButton(this, strLinkBtn, rtButton, IDB_PNG_LINK, 1, NULL, 0, IDC_BTN_FORGETPSW, 0, StringAlignmentCenter, StringAlignmentCenter, Color::SkyBlue, 1, 12);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);
}

//初始化按钮
void CLoginDlg::InitEdits()
{
    m_editUserName.SetLimitText(20);
    GetDlgItem(IDC_EDIT_USER)->SetWindowText(_T("请输入用户名"));
    m_editPsw.SetLimitText(20);
    GetDlgItem(IDC_EDIT_PSW)->SetWindowText(_T("请输入密码"));
}


void CLoginDlg::OnBtnClose()
{
    EndDialog(IDCANCEL);
}


void CLoginDlg::OnBtnLogin()
{
    CString strUserName = _T("");
    CString strPsw = _T("");

    m_editUserName.GetWindowText(strUserName);
    m_editPsw.GetWindowText(strPsw);

    if (strUserName.IsEmpty() == TRUE)
    {
        MessageBox(_T("请输入用户名"), _T("登录"), MB_OK);
    }
    if (strPsw.IsEmpty() == TRUE)
    {
        MessageBox(_T("请输入密码"), _T("登录"), MB_OK);
    }

    //校验用户输入的用户名和密码，若登录过则校验正误，未登录过则创建文件
    int nLoginRet = IsLoginSucceed(strUserName, strPsw, m_bRemPswCheck);

    switch (nLoginRet)
    {
    case LOGIN_OLD_RIGHT:
    {
        MessageBox(_T("欢迎回来！"));
    }
    break;
    case LOGIN_OLD_WRONG:
    {
        MessageBox(_T("用户名密码错误！"));
    }
    break;
    case LOGIN_NEW_SAVERIGHT:
    {
        m_strLoginOrReg = _T("登录");
        InitButtons();
        Invalidate(0);
        MessageBox(_T("欢迎使用！"));
    }
    break;
    case LOGIN_NEW_SAVEERROR:
    {
        MessageBox(_T("操作异常，请重试！"));
    }
    break;
    default:
        break;
    }
}


void CLoginDlg::OnBtnRemPsw()
{
    if (m_bRemPswCheck == TRUE)
    {
        m_bRemPswCheck = FALSE;
    }
    else
    {
        m_bRemPswCheck = TRUE;
    }
}

void CLoginDlg::OnBtnForgetPsw()
{
    AfxMessageBox(L"猪脑子？");
}

void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    CRect rt;
    int nButton = XButtonHitTest(point, rt);

    if (nButton != -1)//点击在按钮上
    {
        m_xButtonList[nButton].LButtonDown();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CLoginDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    CRect rt;
    int nButton = XButtonHitTest(point, rt);
    if (nButton != -1)
    {
        m_xButtonList[nButton].LButtonUp();
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

//测试是否点击在按钮上
int CLoginDlg::XButtonHitTest(CPoint point, CRect&rtButton)
{
    for (size_t i = 0; i< m_xButtonList.size(); i++)
    {
        CXButton& dcControl = m_xButtonList[i];
        if (dcControl.PtInButton(point))
        {
            if (m_xButtonList[i].GetShow() == TRUE)
            {//仅对可见的按钮
                m_xButtonList[i].GetRect(&rtButton);
                return (int)i;
            }
        }
    }
    return -1;
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    if (nCtlColor == CTLCOLOR_EDIT)
    {
        pDC->SetTextColor(m_colorText);
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

int CLoginDlg::XButtonMeasureString(const WCHAR *string, INT length, RectF *boundingBox, int nFontSize/*负数*/)
{
    CDC *pDC = GetDC();

    Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    StringFormat *pStrFmt = (StringFormat *)StringFormat::GenericTypographic();
    pStrFmt->SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);

    LOGFONT lfFont;
    memset(&lfFont, 0, sizeof(lfFont));
    lfFont.lfHeight = nFontSize;
    lfFont.lfWeight |= FW_NORMAL;
    StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName), _T("微软雅黑"));

    Gdiplus::Font font(pDC->GetSafeHdc(), &lfFont);

    int nRet = graphics.MeasureString(string, length, &font, PointF((float)0, (float)0), pStrFmt, boundingBox);

    graphics.ReleaseHDC(pDC->GetSafeHdc());
    ReleaseDC(pDC);
    return nRet;
}


void CLoginDlg::OnUserClickPswEdit()
{
    // TODO:  在此添加控件通知处理程序代码
    //获取程序目录
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);

    CString strUserName = _T("");
    GetDlgItem(IDC_EDIT_USER)->GetWindowText(strUserName);
    //在程序目录下查找用户名的文件
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s.dat"), szPath, strUserName);

    if (IsFileExist(strFilePath) == FALSE)
    {
        m_strLoginOrReg = _T("注册");
        InitButtons();
        Invalidate(0);
    }
    else
    {
        return;
    }
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO:  在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {// 防止按下回车或者esc关闭界面
            return TRUE;
        }
        //if (pMsg->wParam == VK_TAB)
        //{
        //    int nFocusBtn = IsButtonFocus();
        //    if (nFocusBtn != -1)
        //    {
        //        m_xButtonList[nFocusBtn++].SetFocus(TRUE);
        //        if (nFocusBtn == m_xButtonList.size())
        //        {
        //            nFocusBtn = 0;
        //            m_xButtonList[nFocusBtn++].SetFocus(TRUE);
        //        }
        //    }
        //}
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

int CLoginDlg::IsButtonFocus()
{
    int nRet = -1;
    for (int i = 0; i < m_xButtonList.size(); i++)
    {
        if (m_xButtonList[i].GetFocus() == TRUE)
        {
            nRet = i;
        }
    }

    return nRet;
}
