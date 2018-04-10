#include "stdafx.h"
#include "XButton.h"

extern double g_dDpi;
// 绘制按钮
void CXButton::DrawButton(CDC *pDC, Gdiplus::Graphics&  graphics)
{
	if(m_bEnable==FALSE) return; //隐藏按钮

	// 获取按钮图片信息
	Image* pImage = CImageInfo::Instance()->ImageFromResource(m_nImageContorl);
	UINT iCount = m_nImageCount;
	if(m_bCheck && m_nAltImageContorl != 0)
	{
		pImage = CImageInfo::Instance()->ImageFromResource(m_nAltImageContorl);;
		iCount = m_nAltImageCount;
	}

	// 获取按钮状态信息
	SolidBrush brushT(m_TextColor);
	int	iButtonIndex = 0;
	if(m_bDisabled && iCount >= 4) iButtonIndex = 3;
	else if(m_bPressed && iCount >= 3)iButtonIndex = 2;
	else if(m_bHovering && iCount >= 2) {iButtonIndex = 1;}
	else iButtonIndex = 0;

	if (m_bDisabled)
	{
		if (!m_bCommon)
		{
			iButtonIndex = 0;
		}
	}

	// 在指定位置绘制按钮
	int iWidth = pImage->GetWidth()/iCount;
	int iHeight = pImage->GetHeight();
	RectF grect;
	grect.X = m_rcRect.left, grect.Y = m_rcRect.top; grect.Width = iWidth*g_dDpi/*m_rcRect.Width()*/; grect.Height = iHeight*g_dDpi/*m_rcRect.Height()*/;
	graphics.DrawImage(pImage, grect, iWidth*iButtonIndex,0,iWidth,iHeight, UnitPixel);

	StringFormat *pStrFmt = (StringFormat *)StringFormat::GenericTypographic();
	pStrFmt->SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
	pStrFmt->SetFormatFlags(StringFormatFlagsNoWrap);
	pStrFmt->SetAlignment(m_TextAlign);
	pStrFmt->SetLineAlignment(m_TextLineAlign);

	LOGFONT lfFont;
	memset(&lfFont, 0, sizeof(lfFont));
	lfFont.lfHeight = -m_nTextHeight*g_dDpi;
	lfFont.lfWeight |= FW_NORMAL;
	lfFont.lfUnderline = m_bUnderline;
	StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName), _T("微软雅黑"));
	Gdiplus::Font fontT(pDC->GetSafeHdc(), &lfFont);

	if (m_bDisabled)
	{
		if (m_bCommon)
		{
			brushT.SetColor((ARGB)Color::Gray);
		}
	}
	grect.X=m_rcRect.left, grect.Y=m_rcRect.top; grect.Width = m_rcRect.Width(); grect.Height = m_rcRect.Height();
	graphics.DrawString(m_strBtnTitle, m_strBtnTitle.GetLength(), &fontT, grect, pStrFmt, &brushT);
	if (m_bFocus)
	{
		SolidBrush brushDot((ARGB)Color::Gray);
		Gdiplus::Pen penDot(&brushDot);
		penDot.SetDashStyle(DashStyle::DashStyleDot);
		if (m_bIsSetFocus && m_nStanID != 0)
		{
			m_pParent->GetDlgItem(m_nStanID)->MoveWindow(m_rcRect);
			m_pParent->GetDlgItem(m_nStanID)->SetFocus();
			if (m_strBtnTitle == L"")
			{
				m_pParent->GetDlgItem(m_nStanID)->SetWindowText(m_strBtnTipText);
			}
			else
			{
				m_pParent->GetDlgItem(m_nStanID)->SetWindowText(m_strBtnTitle);
			}
		}
		m_bIsSetFocus = FALSE;
		if (m_bEnlargeDot)
		{
			Point p1(m_rcRect.left-2, m_rcRect.top-2);
			Point p2(m_rcRect.right+1, m_rcRect.top-2);
			Point p3(m_rcRect.right+1, m_rcRect.bottom+1);
			Point p4(m_rcRect.left-2, m_rcRect.bottom+1);

			graphics.DrawLine(&penDot, p1, p2);
			graphics.DrawLine(&penDot, p2, p3);
			graphics.DrawLine(&penDot, p3, p4);
			graphics.DrawLine(&penDot, p4, p1);
		}
		else
		{
			Point p1(m_rcRect.left+2, m_rcRect.top+2);
			Point p2(m_rcRect.right-3, m_rcRect.top+2);
			Point p3(m_rcRect.right-3, m_rcRect.bottom-3);
			Point p4(m_rcRect.left+2, m_rcRect.bottom-3);

			graphics.DrawLine(&penDot, p1, p2);
			graphics.DrawLine(&penDot, p2, p3);
			graphics.DrawLine(&penDot, p3, p4);
			graphics.DrawLine(&penDot, p4, p1);
		}
	}
}

// 创建按钮
void CXButton::CreateButton( CWnd* pParent, LPCTSTR pszTitle, LPRECT lpRect, UINT nImage, UINT nCount /*= 4*/, UINT nAltImage /*= _T("")*/, UINT nAltCount /*= 0*/ , UINT nID/* = 0*/, UINT nTextOffset/* = 0*/, Gdiplus::StringAlignment align/* = StringAlignmentCenter*/, Gdiplus::StringAlignment linealign/* = StringAlignmentCenter*/, Color TextColor/* = Color::Black*/, BOOL bAltColor/* = TRUE*/, int nTextHeight/* = 16*/, BOOL bUnderline/* = FALSE*/, BOOL bHandCursor/* = FALSE*/, BOOL bEnlargeDot/* = FALSE*/)
{
	m_pParent					= pParent;
	m_strBtnTitle					= pszTitle;
	m_rcRect						= lpRect;
	m_nImageContorl			= nImage;
	m_nImageCount			= nCount;
	m_nAltImageContorl		= nAltImage;
	m_nAltImageCount		= nAltCount;
	m_nID							= nID;
	m_nTextOffset				= nTextOffset;
	m_TextAlign					= align;
	m_TextLineAlign			= linealign;
	m_TextColor					= TextColor;
	m_bAltColor					= bAltColor;
	m_nTextHeight				= nTextHeight;
	m_bUnderline				= bUnderline;
	m_bHandCursor			= bHandCursor;
	m_bEnlargeDot				= bEnlargeDot;
	if (m_TextAlign == StringAlignmentFar)
	{
		m_rcRect.right += m_nTextOffset;
	}
}

void CXButton::EnableButton(BOOL bEnable, BOOL bCommon/* = TRUE*/)
{
	m_bDisabled = !bEnable;
	m_bCommon = bCommon;
	m_pParent->InvalidateRect(m_rcRect);
}