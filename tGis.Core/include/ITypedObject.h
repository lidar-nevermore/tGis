#pragma once

#ifndef __I_TGISOBJECT_H__
#define __I_TGISOBJECT_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"
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
//�ڱ�����delete�ͻ�����еĶ����ǲ��Եģ�Ҫȷ�����м̳���ITypedObject
//�Ķ������ڱ���Ķ��ﴴ���ġ�
struct TGIS_CORE_API ITypedObject
{
	ITypedObject();
	virtual ~ITypedObject() {};

	static bool CanTransform(const OGRSpatialReference* from, const OGRSpatialReference* to);

	//�����ж�����Դ�����ݼ���ͼ�㡢ͼ����Ⱦ���ľ�������
	//��ͬ�����������������ϵĲ�����ͬ
	//�����ڱ��湤���ռ�ʱ��¼�������ͣ�Ȼ����ع����ռ�ʱ���Դ����������

	//��Ҫ�������־������͵������Ͳ���Ҫʵ���⼸���ӿ�
	virtual const char* GetType() = 0;
	virtual bool IsTypeOf(const char* type) = 0;

	//û������new[]��delete[]��
	//Ҳ����˵��������鷽ʽ������ڴ棬�����ڲ��ǲ������ͷŵ�
	static void* operator new(size_t size);
	static void operator delete(void *p);

private:
	static void* _heap_ptr_map;

protected:
	bool _is_in_heap;

private:
	ITypedObject(const ITypedObject &) = delete;
	ITypedObject &operator=(const ITypedObject &) = delete;
};

typedef ITypedObject* ITGisObjectPtr;

END_NAME_SPACE(tGis, Core)


#endif
