
/*
 * @(#) TestIdipMsg.cpp Created by @itfriday message creator
 */

#include "TestIdipMsg.h"
#include "comm/MBaseFuncDef.h"

/**
 * 显式构造函数
 */
void TestIdipMsg::construct(U16 nTag, const string& sName, MField* pParent, U16 nVer)
{
	MCompositeField::construct(nTag, sName, pParent, nVer);
	m_stSequence.init(M_TAG(1), M_NAME("Sequence"), M_PARENT(this), M_VERSION(1), M_DEFAULT(0));
	m_stVersion.init(M_TAG(2), M_NAME("Version"), M_PARENT(this), M_VERSION(1), M_DEFAULT(0));
	m_stIdipMsg.construct(M_TAG(3), M_NAME("IdipMsg"), M_PARENT(this), M_VERSION(1));
}

/**
 * @override
 */
int TestIdipMsg::encode(MByteArray& baBuf, U16 nVer)
{
	M_CHECK_FIELD_VER_RET(nVer);

	MField::encode(baBuf, nVer);
	baBuf.append((U32)0);

	int iOldLen = baBuf.getLength();

	m_stSequence.encode(baBuf, nVer);
	m_stVersion.encode(baBuf, nVer);
	m_stIdipMsg.encode(baBuf, nVer);

	// 这个必须放后面，因为append有可能那个将baBuf的getData的返回地址改变
	U32 nAddLen = (U32)(baBuf.getLength() - iOldLen);
	char* pszMsg = baBuf.getData() + (baBuf.getLength() - nAddLen - sizeof(U32));

	M_U32_TO_CHAR(pszMsg, nAddLen);

	return 0;
}

/**
 * @override
 */
void TestIdipMsg::format(MByteArray& baBuf, const string& sPrefix, U16 nVer)
{
	string sSubPrefix = sPrefix + "    ";
	M_CHECK_FIELD_VER(nVer);

	baBuf.append(sPrefix).append("[").append(getFieldName()).append("]\n");
	m_stSequence.format(baBuf, sSubPrefix, nVer);
	m_stVersion.format(baBuf, sSubPrefix, nVer);
	m_stIdipMsg.format(baBuf, sSubPrefix, nVer);
}

/**
 * @override
 */
void TestIdipMsg::toXml(MByteArray& baBuf, const string& sPrefix, U16 nVer)
{
	string sSubPrefix = sPrefix + "    ";
	M_CHECK_FIELD_VER(nVer);

	baBuf.append(sPrefix).append("<").append(getFieldName()).append(">\n");
	m_stSequence.toXml(baBuf, sSubPrefix, nVer);
	m_stVersion.toXml(baBuf, sSubPrefix, nVer);
	m_stIdipMsg.toXml(baBuf, sSubPrefix, nVer);
	baBuf.append(sPrefix).append("</").append(getFieldName()).append(">\n");
}

/**
 * @override
 */
MField* TestIdipMsg::getSubField(U16 nTag, U8 /*chMode*/)
{
	switch (nTag)
	{
	case 1:
		return &m_stSequence;
	case 2:
		return &m_stVersion;
	case 3:
		return &m_stIdipMsg;
	default:
		return NULL;
	}
}

/**
 * @override
 */
MField* TestIdipMsg::getSubFieldByName(const string& sName)
{
	if (sName == "Sequence") return &m_stSequence;
	if (sName == "Version") return &m_stVersion;
	if (sName == "IdipMsg") return &m_stIdipMsg;
	return NULL;
}

