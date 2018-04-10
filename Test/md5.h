
#ifndef _MD5_H_
#define _MD5_H_

typedef  unsigned char  u8;

#pragma pack(1)

#pragma pack()

/************************************************************************/
/**
Function: md5 encrypt
Input:
		  in  ---- The plaintext data buffer
		  in_len --- The plaintext data buffer length(B)
		  out  ---- The encrypted data buffer, must larger than 17B, the valid data size is 16B
output:
		  none
**/
/************************************************************************/
//void Calc_MD5( const void * in, size_t in_len, unsigned char * out );

//extern "C" _declspec(dllexport) void Calc_MD5( const void * in, size_t in_len, unsigned char * out );
extern "C" _declspec(dllexport) void  Calc_PBKDF2(const void * in, size_t in_len, unsigned char * out, unsigned char *SaltValue);//加盐哈希
extern "C" _declspec(dllexport) void  Calc_GenRandom(unsigned char * out, unsigned int in_len);//强随机函数

#endif




