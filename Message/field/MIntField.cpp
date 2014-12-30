/*
 * @(#) MIntField.cpp Created on 2014-03-29
 *
 * Copyright (c) 2014-2054 Erisen Xu(徐勇) All Rights Reserved
 */

#include "MIntField.h"

#include "comm/MBaseFuncDef.h"

/**
 * 显式构造函数
 */
void MIntField::construct(U16 nTag, const string& sName, MField* pParent, U16 nVer)
{
    constructField(nTag, M_FIELD_TYPE_INT, sName, pParent, nVer);
}

/**
 * 初始化函数
 */
void MIntField::init(U16 nTag, const string& sName, MField* pParent, U16 nVer, int iValue)
{
    construct(nTag, sName, pParent, nVer);
    m_iValue = iValue;
}

/**
 * 字段编码
 * @param baBuf 保存字段编码后的协议信息
 * @param nVer 编码消息的版本，版本比这个高的字段将被裁剪掉不编码在消息中
 * @return 成功返回0，失败返回错误码
 */
int MIntField::encode(MByteArray& baBuf, U16 nVer)
{
    M_CHECK_FIELD_VER_RET(nVer);
    
    MField::encode(baBuf, nVer);
    baBuf.append(m_iValue);
    return 0;
}

/**
 * 字段解码
 * @param szBuf 要解析的协议
 * @param iBufLen 协议的长度
 * @return 成功返回0，失败返回错误码
 */
int MIntField::decode(const char* szBuf, int /*iBufLen*/)
{
    //const char* pszValue = szBuf + sizeof(m_nTag) + sizeof(m_bType);
    M_CHAR_TO_U16(m_nTag, szBuf);
    M_CHAR_TO_U8(m_bType, szBuf);
    M_CHAR_TO_U32(m_iValue, szBuf);
    
    return 0;
}

/**
 * 将字段格式化成可读形式
 * @param baBuf 保存字段信息的缓存区
 * @param sPrefix 格式化字符串的前缀
 */
void MIntField::format(MByteArray& baBuf, const string& sPrefix, U16 nVer)
{
    M_CHECK_FIELD_VER(nVer);

    char szValue[32];
    snprintf(szValue, sizeof(szValue), "%d(0x%x)", m_iValue, m_iValue);

    baBuf.append(sPrefix).append(m_sName).append(" = ").append(szValue).append("\n");
}

/**
 * 将字段格式化成XML形式
 * @param baBuf 保存字段信息的缓存区
 * @param sPrefix 格式化字符串的前缀
 */
void MIntField::toXml(MByteArray& baBuf, const string& sPrefix, U16 nVer)
{
    M_CHECK_FIELD_VER(nVer);

    char szValue[32];
    snprintf(szValue, sizeof(szValue), "%d", m_iValue);

    baBuf.append(sPrefix).append("<").append(m_sName).append(">").append(szValue).
        append("</").append(m_sName).append(">\n");
}

