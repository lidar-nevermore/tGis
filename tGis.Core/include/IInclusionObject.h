#pragma once

#ifndef __I_INCLUSIONOBJECT_H__
#define __I_INCLUSIONOBJECT_H__

#include "Helper.h"
#include "elr_mpl.h"


class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

//�ں�����ӿ�
//��ʾ����Ķ��󱻾ۺϵ�����������з������õ�

//��������new��delete������ԭ��
//�ڿ�����кܶ�ۺϹ�ϵ���������������Ǳ��ۺ϶����ָ�룬
//Ϊ��ʵ�����������ͷ�ʱ�Զ��ͷű��ۺϵĶ���
//��һ����Ҫ֪�����ۺϵĶ������ڶ��ﻹ����ջ�
//�ڶ���Ķ�������Ҫ������������ͷŵģ���ջ��Ķ�������Ҫ��
//�ڶ�������������ͷű��ۺ϶���Ĵ�����뵽�˱����У�
//�����ۺ϶���Ĵ����ڿͻ����У��ͻ���ͱ������ʹ���˲�ͬ�Ķѣ�
//�ڱ�����delete�ͻ�����еĶ����ǲ��Եģ�Ҫȷ�����м̳���ITypedObject
//�Ķ������ڱ���Ķ��ﴴ���ġ�
struct TGIS_CORE_API IInclusionObject
{
	IInclusionObject();
	virtual ~IInclusionObject() {};

	//û������new[]��delete[]��
	//Ҳ����˵��������鷽ʽ������ڴ棬�����ڲ��ǲ������ͷŵ�
	static void* operator new(size_t size);
	static void operator delete(void *p);

private:
	static void* _heap_ptr_map;

protected:
	bool _is_in_heap;
};

END_NAME_SPACE(tGis, Core)


#endif
