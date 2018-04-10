#pragma once

#define LOGIN_OLD_RIGHT     1 //1-���û��û���������ȷ
#define LOGIN_OLD_WRONG     2 //2-���û��û����������
#define LOGIN_NEW_SAVERIGHT 3 //3-���û������ļ��ɹ�
#define LOGIN_NEW_SAVEERROR 4 //4-���û������ļ�ʧ��

typedef struct USERINFO
{
    unsigned char chSalt[512];
    unsigned char chUserName[30];
    unsigned char chUserPsw[30];
};

//խ�ֽ�ת���ֽ�
BOOL C2W(wchar_t* wstr, const char* str);
//���ֽ�תխ�ֽ�
BOOL W2C(char* str, const wchar_t* wstr);

//�û��Ƿ��Ѿ�����
BOOL IsUserSaved(CString strUserName);

//�洢�û��������� ��δ��¼��洢 ��¼����У�� 
int IsLoginSucceed(CString strUserName, CString strUserPsw, BOOL bRememberPsw);

//У���û���������
BOOL CheckUserNamePsw(CString strUserName, CString strUserPsw);
//�ж��ļ��Ƿ����
BOOL IsFileExist(CString strFilePath);
