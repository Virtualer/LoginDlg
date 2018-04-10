
// TestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CLoginDlg �Ի���
class CLoginDlg : public CDialogEx
{
// ����
public:
	CLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CLoginDlg();
// �Ի�������
    enum { IDD = IDD_DLG_LOGIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    Image*m_pMainBG;
    CEdit m_editUserName;//�û��������
    CEdit m_editPsw;//�û����������
    vector<CXButton> m_xButtonList;//��ť�б�
    vector<CRect> m_xBtnRect;//��ť�����б�
    int m_nBtnHoving;//������ڰ�ť���
    COLORREF m_colorText;//�༭���ı���ɫ
    BOOL m_bRemPswCheck;//��ס���밴ť��ѡ
    CString m_strLoginOrReg;//��¼��ť��ʾ����¼�����ߡ�ע�ᡱ
public:
    void GetSystemDpi();//��ȡϵͳDPI
    void InitWindow();//��ʼ������
    void InitButtons();//��ʼ����ť
    void InitEdits();//��ʼ���༭��

    void OnBtnClose();//�رհ�ť����
    void OnBtnLogin();//��¼��ť����
    void OnBtnRemPsw();//��ס���븴ѡ��
    void OnBtnForgetPsw();//�����������Ӱ�ť

    int IsButtonFocus();//�����Ƿ��ڰ�ť��
    int XButtonHitTest(CPoint point, CRect&rtButton);//�����Ƿ����ڰ�ť��
    int XButtonMeasureString(const WCHAR *string, INT length, RectF *boundingBox, int nFontSize/*����*/);//������ť�ı�
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnUserClickPswEdit();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
