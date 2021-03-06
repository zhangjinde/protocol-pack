
/*
 * @(#) TestUnion3.h Created by @itfriday message creator
 */

#ifndef TEST_UNION3_H
#define TEST_UNION3_H

#include "field/MFieldInc.h"
#include "StarMacro.h"
#include "TestUnion2.h"

/**
 * a union object
 */
class M_DLLIMPORT TestUnion3 : public MCompositeField
{
private:
	MIntField m_stReason; // 原因
	TestUnion2 m_stUnion2; // Test message
	MIntField m_stReason2; // 原因

public:
	/**
	 * 显式构造函数
	 */
	virtual void construct(U16 nTag = 0, const string& sName = string("TestUnion3"), MField* pParent = NULL, U16 nVer = 0);

	/**
	 * Get Function: 原因
	 */
	MIntField* getReason() {return &m_stReason;}

	/**
	 * Get Function: Test message
	 */
	TestUnion2* getUnion2() {return &m_stUnion2;}

	/**
	 * Get Function: 原因
	 */
	MIntField* getReason2() {return &m_stReason2;}

	/**
	 * @override
	 */
	virtual int encode(MByteArray& baBuf, U16 nVer);

	/**
	 * @override
	 */
	virtual void format(MByteArray& baBuf, const string& sPrefix, U16 nVer);

	/**
	 * @override
	 */
	virtual void toXml(MByteArray& baBuf, const string& sPrefix, U16 nVer);

	/**
	 * @override
	 */
	virtual MField* getSubField(U16 nTag, U8 chMode);

	/**
	 * @override
	 */
	virtual MField* getSubFieldByName(const string& sName);
};

#endif

