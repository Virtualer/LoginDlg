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

    //获取程序目录
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);

    //在程序目录下查找用户名的文件
    CString strFilePath;
    strFilePath.Format(_T("%s\\%s.dat"), szPath, strUserName);

    if (IsFileExist(strFilePath) == TRUE)
    {//用户登录过，已存在此用户文件，校验
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
    {//用户从未登录，存此用户文件
        if (bRememberPsw == TRUE)
        {//用户勾选记住密码
            //获取用户输入的用户名和密码
            TCHAR * tchUserName;
            TCHAR * tchUserPsw;
            tchUserName = strUserName.GetBuffer(strUserName.GetLength());
            tchUserPsw = strUserPsw.GetBuffer(strUserPsw.GetLength());

            strUserName.ReleaseBuffer();
            strUserPsw.ReleaseBuffer();

            //生成盐
            unsigned char chSalt[512] = "";
            Calc_GenRandom(chSalt, sizeof(chSalt));

            //生成用户名的MD5
            unsigned char chUserNameMd5[30] = { 0 };
            size_t stLength = 0;
            StringCchLength(tchUserName, MAX_PATH, &stLength);
            Calc_PBKDF2(tchUserName, stLength*sizeof(TCHAR), chUserNameMd5, chSalt);

            //生成用户密码的MD5
            unsigned char chUserPswMd5[30] = { 0 };
            stLength = 0;
            StringCchLength(tchUserPsw, MAX_PATH, &stLength);
            Calc_PBKDF2(tchUserPsw, stLength*sizeof(TCHAR), chUserPswMd5, chSalt);

            //创建用户文件
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

            //隐藏用户文件
            CFileStatus  fs;
            CFile::GetStatus(strFilePath, fs);
            fs.m_attribute = 0x22;
            CFile::SetStatus(strFilePath, fs);

            nRet = LOGIN_NEW_SAVERIGHT;
        }
        else
        {
            //没有找到用户的文件，用户也没有选择记住密码，需要另一方式校验
        }
    }
    return nRet;
}

BOOL CheckUserNamePsw(CString strUserName, CString strUserPsw)
{
    //获取用户输入的用户名和密码
    TCHAR * tchUserName;
    TCHAR * tchUserPsw;
    tchUserName = strUserName.GetBuffer(strUserName.GetLength());
    tchUserPsw = strUserPsw.GetBuffer(strUserPsw.GetLength());

    strUserName.ReleaseBuffer();
    strUserPsw.ReleaseBuffer();

    //读取文件中的信息
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

    //生成用户名的MD5
    unsigned char chUserNameMd5[30] = { 0 };
    size_t stLength = 0;
    StringCchLength(tchUserName, MAX_PATH, &stLength);
    Calc_PBKDF2(tchUserName, stLength*sizeof(TCHAR), chUserNameMd5, saveinfo.chSalt);

    //生成用户密码的MD5
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
