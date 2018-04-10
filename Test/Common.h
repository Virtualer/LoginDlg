#pragma once

#define LOGIN_OLD_RIGHT     1 //1-老用户用户名密码正确
#define LOGIN_OLD_WRONG     2 //2-老用户用户名密码错误
#define LOGIN_NEW_SAVERIGHT 3 //3-新用户创建文件成功
#define LOGIN_NEW_SAVEERROR 4 //4-新用户创建文件失败

typedef struct USERINFO
{
    unsigned char chSalt[512];
    unsigned char chUserName[30];
    unsigned char chUserPsw[30];
};

//窄字节转宽字节
BOOL C2W(wchar_t* wstr, const char* str);
//宽字节转窄字节
BOOL W2C(char* str, const wchar_t* wstr);

//用户是否已经存在
BOOL IsUserSaved(CString strUserName);

//存储用户名和密码 若未登录则存储 登录过则校验 
int IsLoginSucceed(CString strUserName, CString strUserPsw, BOOL bRememberPsw);

//校验用户名和密码
BOOL CheckUserNamePsw(CString strUserName, CString strUserPsw);
//判断文件是否存在
BOOL IsFileExist(CString strFilePath);
