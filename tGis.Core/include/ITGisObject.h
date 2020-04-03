#pragma once

#ifndef __I_TGISOBJECT_H__
#define __I_TGISOBJECT_H__

#include "Helper.h"
#include "tGisCoreCfg.h"
#include "elr_mpl.h"


class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

//��������new��delete������ԭ��
//�ڿ�����кܶ�ۺϹ�ϵ���������������Ǳ��ۺ϶����ָ�룬
//Ϊ��ʵ�����������ͷ�ʱ�Զ��ͷű��ۺϵĶ���
//��һ����Ҫ֪�����ۺϵĶ������ڶ��ﻹ����ջ�
//�ڶ���Ķ�������Ҫ������������ͷŵģ���ջ��Ķ�������Ҫ��
//�ڶ�������������ͷű��ۺ϶���Ĵ�����뵽�˱����У�
//�����ۺ϶���Ĵ����ڿͻ����У��ͻ���ͱ������ʹ���˲�ͬ�Ķѣ�
//�ڱ�����delete�ͻ�����еĶ����ǲ��Եģ�Ҫȷ�����м̳���ITGisObject
//�Ķ������ڱ���Ķ��ﴴ���ġ�
struct TGIS_CORE_API ITGisObject
{
	ITGisObject();
	virtual ~ITGisObject() {};

	static bool CanTransform(const OGRSpatialReference* from, const OGRSpatialReference* to);

	//����ʱ���ʶ��
	virtual const char* GetType() = 0;
	virtual bool IsTypeOf(const char* type) = 0;
	virtual bool IsTypeOf(ITGisObject* object) = 0;

	//û������new[]��delete[]��
	//Ҳ����˵�������������ڴ棬�����ڲ��ǲ������ͷŵ�
	static void* operator new(size_t size);
	static void operator delete(void *p);

private:
	static void* _heap_ptr_map;

protected:
	bool _is_in_heap;

private:
	ITGisObject(const ITGisObject &) = delete;
	ITGisObject &operator=(const ITGisObject &) = delete;
};

typedef ITGisObject* ITGisObjectPtr;

END_NAME_SPACE(tGis, Core)


#endif
