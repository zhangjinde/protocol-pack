/*
 * @(#) MCompositeField.cpp Created on 2014-03-27
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

#include "MCompositeField.h"

#include "comm/MBaseFuncDef.h"
#include "comm/MErrorCode.h"

/**
 * 显式构造函数
 */
void MCompositeField::construct(U16 nTag, const string& sName, MField* pParent, U16 nVer)
{
    constructField(nTag, M_FIELD_TYPE_TLV, sName, pParent, nVer);
}

/**
 * 字段解码
 * @param szBuf 要解析的协议
 * @param iBufLen 协议的长度
 */
int MCompositeField::decode(const char* szBuf, int iBufLen)
{
    if (NULL == szBuf) return M_ERROR_INPUT_PARAM_NULL;

    int iMinLen = sizeof(m_nTag) + sizeof(m_bType);    // 字段最小长度
    MField* pstSubField = NULL;

    U16 nTag = 0;
    U8 bType = 0;
    const char* pszBuf = szBuf + iMinLen + sizeof(U32);
    int iLeftLen = iBufLen - iMinLen - sizeof(U32);
    int iFieldLen;
    int iRet = -1;

    if (iLeftLen < 0)
    {
        return M_ERROR_DECODE_BUFSIZE_SHORT;
    }

    // 求字段长度
    iLeftLen = MField::readInt(szBuf + iMinLen);
    
    while (iLeftLen > 0)
    {
        // 解析Tag和Type
        if (iLeftLen < iMinLen)
        {
            return M_ERROR_DECODE_BUFSIZE_SHORT;
        }

        // Tag和类型
        nTag = readShort(pszBuf);
        bType = readByte(pszBuf + sizeof(nTag));

        // 消息长度
        iFieldLen = MField::getLengthByType(bType, pszBuf, iLeftLen);
        if (iFieldLen == M_ERROR_DECODE_BUFSIZE_SHORT || iLeftLen < iFieldLen)
        {
            return M_ERROR_DECODE_BUFSIZE_SHORT;
        }

        // 取消息对象
        pstSubField = getSubField(nTag, M_GET_SUB_FIELD_MODE_DECODE);

        // 对认识的字段进行解码，不认识的字段直接丢弃
        if (pstSubField)
        {
            iRet = pstSubField->decode(pszBuf, iLeftLen);
            if (iRet != 0) return iRet;
        }
        // 不认识的字段，直接丢弃 (兼容旧版本)
        pszBuf += iFieldLen;
        iLeftLen -= iFieldLen;
    }
    
    return 0;
}
