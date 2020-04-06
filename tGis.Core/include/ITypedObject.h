#pragma once

#ifndef __I_TYPEDOBJECT_H__
#define __I_TYPEDOBJECT_H__

#include "Helper.h"
#include "elr_mpl.h"


class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API ITypedObject
{
	ITypedObject() {};
	virtual ~ITypedObject() {};

	//�����ж�����Դ�����ݼ���ͼ�㡢ͼ����Ⱦ�������Ӷ���ľ�������
	//��ͬ�����������������ϵĲ�����ͬ
	//�����ڱ��湤���ռ�ʱ��¼�������ͣ�Ȼ����ع����ռ�ʱ���Դ����������

	//��Ҫ�������־������͵������Ͳ���Ҫʵ���⼸���ӿ�
	virtual const char* GetType() = 0;
	virtual bool IsTypeOf(const char* type) = 0;
};

END_NAME_SPACE(tGis, Core)


#endif
