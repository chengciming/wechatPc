#pragma once

#ifndef _USER_SHA1_H
#define _USER_SHA1_H

#include <stdio.h>
#include <memory.h>



#define SHA1_SIZE_BYTE 20

/**
* @brief SHA1_String
*
* Detailed description.
* @param[in] inputString 要进行处理的无符号字符串指针
* @param[in] len 要处理的信息长度，n<= strlen(p);
* @param[out] pOutSHA1Buf 输出摘要信息，长度为20的 unsigned char ，共160 bits
* @return int
*/
int SHA1_String(const unsigned char* inputString, unsigned long len, unsigned char* pOutSHA1Buf);

/**
* @brief SHA1_String_Compare
*
* Detailed description.
* @param[in] inputString
* @param[in] len
* @param[in] pOutSHA1Buf
* @return int
*/
int SHA1_String_Compare(const unsigned char* inputString, unsigned long len, const unsigned char* pOutSHA1Buf);

#endif /**< _USER_SHA1_H */
