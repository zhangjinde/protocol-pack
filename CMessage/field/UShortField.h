/*
 * @(#) UShortField.h Created on 2014-03-27
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

#ifndef USHORT_FIELD_H
#define USHORT_FIELD_H

#include "Field.h"

/**
 * UShort类型字段对象
 */
struct tagUShortField
{
    FIELD stField;
    U16 nValue;
};

typedef struct tagUShortField  USHORTFIELD;
typedef struct tagUShortField* LPUSHORTFIELD;

/**
 * Short字段信息定义
 */
extern FIELDINFO g_stUShortFieldInfo;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * 字段初始化
 * @param pstUShortField 要初始化的字段
 * @param szName 字段名字
 * @param nTag 字段Tag
 * @param nVer 字段引人版本号
 */
void construct_ushort_field(LPUSHORTFIELD pstUShortField, const char* szName, U16 nTag, LPFIELD pstParent, U16 nVer);

/**
 * 字段初始化
 * @param pstUShortField 要初始化的字段
 * @param szName 字段名字
 * @param nTag 字段Tag
 * @param nVer 字段引人版本号
 */
void init_ushort_field(LPUSHORTFIELD pstUShortField, const char* szName, U16 nTag, LPFIELD pstParent, U16 nVer, U16 nDefault);

#ifdef __cplusplus
}
#endif

#endif /* defined(USHORT_FIELD_H) */
