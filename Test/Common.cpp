#include "stdafx.h"
#include "Common.h"
#include "md5.h"
#pragma comment(lib,"md5_dll.lib")

BOOL C2W(wchar_t* wstr, const char* str)
{
    int len = MultiByteToWideChar(CP_OEMCP, 0, str, -1, wstr, 0);
    return len == MultiByteToWideChar(CP_OEMCP, 0, str, -1, wstr, len);
}

BOOL W2C(char* str, const wchar_t* wstr)
{
    int len = WideCharToMultiByte(CP_OEMCP, 0, wstr, -1, str, 0, 0, 0);
    return len == WideCharToMultiByte(CP_OEMCP, 0, wstr, -1, str, len, 0, 0);
}

int IsLoginSucceed(CString strUserName, CString strUserPsw, BOOL bRememberPsw)
{
    int nRet = 0;

    //��ȡ����Ŀ¼
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);

    //�ڳ���Ŀ¼�²����û������ļ�
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s.dat"), szPath, strUserName);

    if (IsFileExist(strFilePath) == TRUE)
    {//�û���¼�����Ѵ��ڴ��û��ļ���У��
        if (CheckUserNamePsw(strUserName, strUserPsw) == TRUE)
        {
            nRet = LOGIN_OLD_RIGHT;
        }
        else
        {
            nRet = LOGIN_OLD_WRONG;
        }
    }
    else
    {//�û���δ��¼������û��ļ�
        if (bRememberPsw == TRUE)
        {//�û���ѡ��ס����
            //��ȡ�û�������û���������
            TCHAR * tchUserName;
            TCHAR * tchUserPsw;
            tchUserName = strUserName.GetBuffer(strUserName.GetLength());
            tchUserPsw = strUserPsw.GetBuffer(strUserPsw.GetLength());

            strUserName.ReleaseBuffer();
            strUserPsw.ReleaseBuffer();

            //������
            unsigned char chSalt[512] = "";
            Calc_GenRandom(chSalt, sizeof(chSalt));

            //�����û�����MD5
            unsigned char chUserNameMd5[30] = { 0 };
            size_t stLength = 0;
            StringCchLength(tchUserName, MAX_PATH, &stLength);
            Calc_PBKDF2(tchUserName, stLength*sizeof(TCHAR), chUserNameMd5, chSalt);

            //�����û������MD5
            unsigned char chUserPswMd5[30] = { 0 };
            stLength = 0;
            StringCchLength(tchUserPsw, MAX_PATH, &stLength);
            Calc_PBKDF2(tchUserPsw, stLength*sizeof(TCHAR), chUserPswMd5, chSalt);

            //�����û��ļ�
            CFile cf;
            BOOL bRet = cf.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
            if (bRet == FALSE)
            {
                nRet = LOGIN_NEW_SAVEERROR;
            }

            USERINFO userinfo;
            memcpy_s(userinfo.chSalt, sizeof(userinfo.chSalt), chSalt, sizeof(chSalt));
            memcpy_s(userinfo.chUserName, sizeof(userinfo.chUserName), chUserNameMd5, sizeof(chUserNameMd5));
            memcpy_s(userinfo.chUserPsw, sizeof(userinfo.chUserPsw), chUserPswMd5, sizeof(chUserPswMd5));

            cf.Write(&userinfo, sizeof(USERINFO));
            cf.Flush();
            cf.Close();

            //�����û��ļ�
            CFileStatus  fs;
            CFile::GetStatus(strFilePath, fs);
            fs.m_attribute = 0x22;
            CFile::SetStatus(strFilePath, fs);

            nRet = LOGIN_NEW_SAVERIGHT;
        }
        else
        {
            //û���ҵ��û����ļ����û�Ҳû��ѡ���ס���룬��Ҫ��һ��ʽУ��
        }
    }
    return nRet;
}

BOOL CheckUserNamePsw(CString strUserName, CString strUserPsw)
{
    //��ȡ�û�������û���������
    TCHAR * tchUserName;
    TCHAR * tchUserPsw;
    tchUserName = strUserName.GetBuffer(strUserName.GetLength());
    tchUserPsw = strUserPsw.GetBuffer(strUserPsw.GetLength());

    strUserName.ReleaseBuffer();
    strUserPsw.ReleaseBuffer();

    //��ȡ�ļ��е���Ϣ
    TCHAR tchPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, tchPath, MAX_PATH);
    PathRemoveFileSpec(tchPath);

    CString strFilePath;
    strFilePath.Format(_T("%s\\%s.dat"), tchPath, strUserName);

    CFile cf;
    BOOL bRet = cf.Open(strFilePath, CFile::modeReadWrite);

    USERINFO saveinfo;
    memset(&saveinfo, 0, sizeof(USERINFO));
    if (bRet == TRUE)
    {
        if (cf.GetLength() < sizeof(USERINFO))
        {
            cf.Close();
            return FALSE;
        }
        cf.Read(&saveinfo, sizeof(USERINFO));
        cf.Close();
    }
    else
    {
        return FALSE;
    }

    //�����û�����MD5
    unsigned char chUserNameMd5[30] = { 0 };
    size_t stLength = 0;
    StringCchLength(tchUserName, MAX_PATH, &stLength);
    Calc_PBKDF2(tchUserName, stLength*sizeof(TCHAR), chUserNameMd5, saveinfo.chSalt);

    //�����û������MD5
    unsigned char chUserPswMd5[30] = { 0 };
    stLength = 0;
    StringCchLength(tchUserPsw, MAX_PATH, &stLength);
    Calc_PBKDF2(tchUserPsw, stLength*sizeof(TCHAR), chUserPswMd5, saveinfo.chSalt);

    if (memcmp(chUserNameMd5,saveinfo.chUserName,sizeof(chUserNameMd5)) == 0)
    {
        if (memcmp(chUserPswMd5,saveinfo.chUserPsw,sizeof(chUserPswMd5)) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL IsFileExist(CString strFilePath)
{
    WIN32_FIND_DATA findfile;
    HANDLE hFind;
    hFind = FindFirstFile(strFilePath, &findfile);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    FindClose(hFind);
    return TRUE;
}
