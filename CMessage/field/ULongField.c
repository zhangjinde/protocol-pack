/*
 * @(#) ULongField.c Created on 2014-03-27
 *
 * Copyright (c) 2014-2016 Erisen Xu (@itfriday)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ULongField.h"

#include "comm/BaseFuncDef.h"
#include "comm/ErrorCode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * 字段编码
 * @param baBuf 保存字段编码后的协议信息
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 * @param 成功返回0，失败返回错误码
 */
static int ulong_field_encode(LPFIELD pstField, LPBYTEARRAY pstByteArray, U16 nVer);

/**
 * 字段解码
 * @param szBuf 要解析的协议
 * @param iBufLen 协议的长度
 */
static int ulong_field_decode(LPFIELD pstField, const char* szBuf, int iBufLen);


/**
 * 将字段格式化成可读形式
 * @param pstField 要格式化的字段
 * @param pstByteArray 保存字段信息的缓存区
 * @param szPrefix 格式化字符串的前缀
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 */
static int ulong_field_format(LPFIELD pstField, LPBYTEARRAY pstByteArray, const char* szPrefix, U16 nVer);

/**
 * 将字段格式化成XML形式
 * @param pstField 要格式化的字段
 * @param pstByteArray 保存字段信息的缓存区
 * @param szPrefix 格式化字符串的前缀
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 */
static int ulong_field_to_xml(LPFIELD pstField, LPBYTEARRAY pstByteArray, const char* szPrefix, U16 nVer);

/**
 * 设置字段的值
 * @param szValue 字段的值
 */
static void ulong_field_set_value(LPFIELD pstField, const char* szValue);

/**
 * Int字段信息定义
 */
FIELDINFO g_stULongFieldInfo =
{
    (U8)FIELD_TYPE_ULONG,   /* type */
    NULL,                   /* construct */
    NULL,                   /* init */
    ulong_field_encode,     /* encode */
    ulong_field_decode,     /* decode */
    ulong_field_format,     /* format */
    ulong_field_to_xml,     /* toXml */
    NULL,                   /* getSubField */
    NULL,                   /* getSubFieldByName */
    ulong_field_set_value   /* setValue */
};

/**
 * 字段初始化
 * @param pstULongField 要初始化的字段
 * @param szName 字段名字
 * @param nTag 字段Tag
 * @param nVer 字段引人版本号
 */
void construct_ulong_field(LPULONGFIELD pstULongField, const char* szName, U16 nTag, LPFIELD pstParent, U16 nVer)
{
    if (NULL == pstULongField) return;

    init_field((LPFIELD)pstULongField, &g_stULongFieldInfo, szName, nTag, pstParent, nVer);
}

/**
 * 字段初始化
 * @param pstFieldInfo 字段描述信息
 * @param szName 字段名字
 * @param nTag 字段Tag
 * @param nVer 字段引人版本号
 */
void init_ulong_field(LPULONGFIELD pstULongField, const char* szName, U16 nTag, LPFIELD pstParent, U16 nVer, U64 ullDefault)
{
    if (NULL == pstULongField) return;

    construct_ulong_field(pstULongField, szName, nTag, pstParent, nVer);

    pstULongField->ullValue = ullDefault;
}

/**
 * 字段编码
 * @param pstField 要编码的字段
 * @param pstInitField 已初始化的字段，提供编码函数
 * @param baBuf 保存字段编码后的协议信息
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 * @param 成功返回0，失败返回错误码
 */
int ulong_field_encode(LPFIELD pstField, LPBYTEARRAY pstByteArray, U16 nVer)
{
    int iRet;

    if (NULL == pstField || NULL == pstByteArray) return ERROR_INPUT_PARAM_NULL;

    CHECK_FIELD_VER_RET(pstField, nVer);

    CHECK_FUNC_RET(bytearray_append_u16(pstByteArray, pstField->nTag), iRet);
    CHECK_FUNC_RET(bytearray_append_u8(pstByteArray, pstField->pstFieldInfo->bType), iRet);
    CHECK_FUNC_RET(bytearray_append_u64(pstByteArray, ((LPULONGFIELD)pstField)->ullValue), iRet);

    return 0;
}

/**
 * 字段解码
 * @param pstField 要编码的字段
 * @param pstInitField 已初始化的字段，提供编码函数
 * @param szBuf 要解析的协议
 * @param iBufLen 协议的长度
 */
int ulong_field_decode(LPFIELD pstField, const char* szBuf, int iBufLen)
{
    U64 ullValue;
    const char* pszBuf;

    if (NULL == pstField || NULL == szBuf || NULL == pstField->pstFieldInfo || iBufLen <= 0) return ERROR_INPUT_PARAM_NULL;

    pszBuf = szBuf + sizeof(pstField->nTag) + sizeof(pstField->pstFieldInfo->bType);

    M_CHAR_TO_U64(ullValue, pszBuf);

    ((LPULONGFIELD)pstField)->ullValue = ullValue;

    return 0;
}

/**
 * 将字段格式化成可读形式
 * @param pstField 要格式化的字段
 * @param pstByteArray 保存字段信息的缓存区
 * @param szPrefix 格式化字符串的前缀
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 */
int ulong_field_format(LPFIELD pstField, LPBYTEARRAY pstByteArray, const char* szPrefix, U16 nVer)
{
    char szValue[MAX_FIELD_INFO_LEN];

    if (NULL == pstField || NULL == pstByteArray || NULL == szPrefix) return ERROR_INPUT_PARAM_NULL;

    CHECK_FIELD_VER_RET(pstField, nVer);

    snprintf(szValue, sizeof(szValue), "%s%s=%llu\n", szPrefix, pstField->szName,
         ((LPULONGFIELD)pstField)->ullValue);

    return bytearray_append_string(pstByteArray, szValue, strlen(szValue));
}

/**
 * 将字段格式化成XML形式
 * @param pstField 要格式化的字段
 * @param pstByteArray 保存字段信息的缓存区
 * @param szPrefix 格式化字符串的前缀
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 */
int ulong_field_to_xml(LPFIELD pstField, LPBYTEARRAY pstByteArray, const char* szPrefix, U16 nVer)
{
    char szValue[MAX_FIELD_INFO_LEN];

    if (NULL == pstField || NULL == pstByteArray || NULL == szPrefix) return ERROR_INPUT_PARAM_NULL;

    CHECK_FIELD_VER_RET(pstField, nVer);

    snprintf(szValue, sizeof(szValue), "%s<%s>%llu</%s>\n", szPrefix, pstField->szName,
        ((LPULONGFIELD)pstField)->ullValue, pstField->szName);

    return bytearray_append_string(pstByteArray, szValue, strlen(szValue));
}

/**
 * 设置字段的值
 * @param szValue 字段的值
 */
void ulong_field_set_value(LPFIELD pstField, const char* szValue)
{
    if (NULL == pstField) return;

    if (NULL == szValue)
    {
        ((LPULONGFIELD)pstField)->ullValue = 0;
    }
    else
    {
        ((LPULONGFIELD)pstField)->ullValue = (U64)atoll(szValue);
    }
}

