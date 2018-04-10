
// LoginDlg.cpp : ʵ���ļ�
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

// CLoginDlg �Ի���
double g_dDpi;


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nBtnHoving = -1;
    m_bRemPswCheck = TRUE;
    m_strLoginOrReg = _T("��¼");
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


// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    m_colorText = RGB(0, 0, 0);

    GetSystemDpi();
    InitWindow();
    InitButtons();
    InitEdits();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoginDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
    //
	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
    //
	//	// ʹͼ���ڹ����������о���
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;
    //
	//	// ����ͼ��
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialogEx::OnPaint();
	//}

    // ʹ��UNICODE�ַ�ת��
    USES_CONVERSION;

    CPaintDC dc(this);
    CMemoryDC dcMem(&dc);

    //��ȡ���ھ���λ��
    CRect rtClient;
    GetClientRect(rtClient);

    //��ȡ�ػ����λ��
    CRect rcPaint;
    dcMem.GetClipBox(&rcPaint);

    Gdiplus::Graphics graphics(dcMem.m_hDC);

    m_pMainBG = CImageInfo::Instance()->ImageFromResource(IDB_PNG_LOGINBG);
    graphics.DrawImage(m_pMainBG, 0, 0, m_pMainBG->GetWidth()*g_dDpi, m_pMainBG->GetHeight()*g_dDpi);

    //���Ʊ༭������ı�
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
    StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName), _T("΢���ź�"));
    Gdiplus::Font ftLeft(dcMem.GetSafeHdc(), &lfFont);

    CString strLeft;
    RectF rcString;

    strLeft = _T("�û���:");
    graphics.MeasureString(strLeft, strLeft.GetLength(), &ftLeft, PointF((float)0, (float)0), &stringFormat, &rcString);
    rectString.X = 120 * g_dDpi;
    rectString.Y = 132 * g_dDpi;
    rectString.Width = rcString.Width*g_dDpi;
    rectString.Height = rcString.Height*g_dDpi;
    graphics.DrawString(strLeft, strLeft.GetLength(), &ftLeft, rectString, &stringFormat, &brushLeft);

    strLeft = _T("����:");
    rectString.Y = 162 * g_dDpi;
    graphics.DrawString(strLeft, strLeft.GetLength(), &ftLeft, rectString, &stringFormat, &brushLeft);

    //���ư�ť
    for (size_t i = 0; i< m_xButtonList.size(); i++)
    {
        CXButton& dcControl = m_xButtonList[i];
        CRect rcControl;
        dcControl.GetRect(&rcControl);
        dcControl.DrawButton(dcMem, graphics);
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ȡϵͳDPI
void CLoginDlg::GetSystemDpi()
{
    CDC *pDC = GetDC();
    float sysDpi = (float)pDC->GetDeviceCaps(LOGPIXELSX) / 96;
    ReleaseDC(pDC);
    g_dDpi = sysDpi;
}

//��ʼ������
void CLoginDlg::InitWindow()
{
    m_pMainBG = CImageInfo::Instance()->ImageFromResource(IDB_PNG_LOGINBG);

    //�޸������ڴ�С
    CRect cClientRect;
    GetClientRect(&cClientRect);
    cClientRect.right = m_pMainBG->GetWidth()*g_dDpi;
    cClientRect.bottom = m_pMainBG->GetHeight()*g_dDpi;
    MoveWindow(&cClientRect);
    CenterWindow(GetParent());

    //�ڽ�������ӱ༭��
    CWnd * pWnd;
    pWnd = GetDlgItem(IDC_EDIT_USER);
    pWnd->SetWindowPos(NULL, 170 * g_dDpi, 133 * g_dDpi, 200 * g_dDpi, 20 * g_dDpi, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_EDIT_PSW);
    pWnd->SetWindowPos(NULL, 170 * g_dDpi, 163 * g_dDpi, 200 * g_dDpi, 20 * g_dDpi, SWP_NOZORDER);
}

//��ʼ����ť
void CLoginDlg::InitButtons()
{
    m_xButtonList.clear();
    m_xBtnRect.clear();

    CRect cClientRect;
    GetClientRect(&cClientRect);

    CRect rtButton;
    CXButton dcControl;

    //�رհ�ť
    rtButton.top = 12 * g_dDpi;
    rtButton.bottom = rtButton.top + 20 * g_dDpi;
    rtButton.left = 467 * g_dDpi;
    rtButton.right = rtButton.left + 20 * g_dDpi;
    dcControl.SetBtnTipText(_T("�ر�"));
    dcControl.CreateButton(this, _T(""), rtButton, IDB_PNG_BTNCLOSE, 2, NULL, 0, IDC_BTN_CLOSE);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //��ס���븴ѡ��
    RectF rtBtn;
    CString strCheckBtn = _T("��ס����");
    XButtonMeasureString(strCheckBtn, strCheckBtn.GetLength(), &rtBtn, -12);

    rtButton.left = 170 *g_dDpi;
    rtButton.right = rtButton.left + (rtBtn.Width + 25)*g_dDpi;
    rtButton.top = 193 * g_dDpi;
    rtButton.bottom = rtButton.top + 18 * g_dDpi;
    dcControl.CreateButton(this, strCheckBtn, rtButton, IDB_PNG_CHECK, 2, IDB_PNG_UNCHECK, 2, IDC_BTN_REMPSW, 0, StringAlignmentFar, StringAlignmentCenter, Color::Black, FALSE, 12, 0, 0, 1);
    dcControl.SetStyle(1);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //��¼��ť
    rtButton.top = 233 * g_dDpi;
    rtButton.bottom = rtButton.top + 30 * g_dDpi;
    rtButton.left = 170 * g_dDpi;
    rtButton.right = rtButton.left + 118 * g_dDpi;
    dcControl.SetBtnTipText(m_strLoginOrReg);
    dcControl.CreateButton(this, m_strLoginOrReg, rtButton, IDB_PNG_NORMALBTN, 4, NULL, 0, IDC_BTN_LOGIN, 0, StringAlignmentCenter, StringAlignmentCenter, Color::Black, 1, 14);
    m_xButtonList.push_back(dcControl);
    m_xBtnRect.push_back(rtButton);

    //�������밴ť
    CString strLinkBtn = _T("��������");
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

//��ʼ����ť
void CLoginDlg::InitEdits()
{
    m_editUserName.SetLimitText(20);
    GetDlgItem(IDC_EDIT_USER)->SetWindowText(_T("�������û���"));
    m_editPsw.SetLimitText(20);
    GetDlgItem(IDC_EDIT_PSW)->SetWindowText(_T("����������"));
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
        MessageBox(_T("�������û���"), _T("��¼"), MB_OK);
    }
    if (strPsw.IsEmpty() == TRUE)
    {
        MessageBox(_T("����������"), _T("��¼"), MB_OK);
    }

    //У���û�������û��������룬����¼����У������δ��¼���򴴽��ļ�
    int nLoginRet = IsLoginSucceed(strUserName, strPsw, m_bRemPswCheck);

    switch (nLoginRet)
    {
    case LOGIN_OLD_RIGHT:
    {
        MessageBox(_T("��ӭ������"));
    }
    break;
    case LOGIN_OLD_WRONG:
    {
        MessageBox(_T("�û����������"));
    }
    break;
    case LOGIN_NEW_SAVERIGHT:
    {
        m_strLoginOrReg = _T("��¼");
        InitButtons();
        Invalidate(0);
        MessageBox(_T("��ӭʹ�ã�"));
    }
    break;
    case LOGIN_NEW_SAVEERROR:
    {
        MessageBox(_T("�����쳣�������ԣ�"));
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
    AfxMessageBox(L"�����ӣ�");
}

void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CRect rt;
    int nButton = XButtonHitTest(point, rt);

    if (nButton != -1)//����ڰ�ť��
    {
        m_xButtonList[nButton].LButtonDown();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CLoginDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CRect rt;
    int nButton = XButtonHitTest(point, rt);
    if (nButton != -1)
    {
        m_xButtonList[nButton].LButtonUp();
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

//�����Ƿ����ڰ�ť��
int CLoginDlg::XButtonHitTest(CPoint point, CRect&rtButton)
{
    for (size_t i = 0; i< m_xButtonList.size(); i++)
    {
        CXButton& dcControl = m_xButtonList[i];
        if (dcControl.PtInButton(point))
        {
            if (m_xButtonList[i].GetShow() == TRUE)
            {//���Կɼ��İ�ť
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

    // TODO:  �ڴ˸��� DC ���κ�����
    if (nCtlColor == CTLCOLOR_EDIT)
    {
        pDC->SetTextColor(m_colorText);
    }
    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}

int CLoginDlg::XButtonMeasureString(const WCHAR *string, INT length, RectF *boundingBox, int nFontSize/*����*/)
{
    CDC *pDC = GetDC();

    Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    StringFormat *pStrFmt = (StringFormat *)StringFormat::GenericTypographic();
    pStrFmt->SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);

    LOGFONT lfFont;
    memset(&lfFont, 0, sizeof(lfFont));
    lfFont.lfHeight = nFontSize;
    lfFont.lfWeight |= FW_NORMAL;
    StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName), _T("΢���ź�"));

    Gdiplus::Font font(pDC->GetSafeHdc(), &lfFont);

    int nRet = graphics.MeasureString(string, length, &font, PointF((float)0, (float)0), pStrFmt, boundingBox);

    graphics.ReleaseHDC(pDC->GetSafeHdc());
    ReleaseDC(pDC);
    return nRet;
}


void CLoginDlg::OnUserClickPswEdit()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //��ȡ����Ŀ¼
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);

    CString strUserName = _T("");
    GetDlgItem(IDC_EDIT_USER)->GetWindowText(strUserName);
    //�ڳ���Ŀ¼�²����û������ļ�
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s.dat"), szPath, strUserName);

    if (IsFileExist(strFilePath) == FALSE)
    {
        m_strLoginOrReg = _T("ע��");
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
    // TODO:  �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {// ��ֹ���»س�����esc�رս���
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
