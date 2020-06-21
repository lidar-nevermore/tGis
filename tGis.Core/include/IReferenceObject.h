#pragma once

#ifndef __I_ReferenceObject_H__
#define __I_ReferenceObject_H__

#include <cassert>
#include "Helper.h"
#include "elr_mpl.h"

#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)


//�̳�IInclusionObject��Ϊ����IReferenceObject�ڱ���Ķ��ﴴ��
//
//�����޷������Բ���Լ��һ��������඼ֻ��new��
//ֻ�ܾ��������Բ���Լ�������ȷʹ�÷�ʽ
//
//һ������IReferenceObject�Ľӿڣ�
//������д�����ֵ����ô�ӿ��ڲ���Ҫ����Reference
//���д�����ֵҲ����˵IReferenceObject���ɵ������ԣ�
//�����GradientColorRepository�л�ȡGradientColor
//�����SimpleSymbolLibrary�л�ȡSymbol
//
//����ӿڹ���Ϊ����IReferenceObject��������
//��ô�ӿ��ڲ������Ե���Reference
//
//һ������÷���IReferenceObject�Ľӿں�������ָ��������ɹ��ܣ�
//��ô�������Ҫ�ɶԵĵ���Reference��Release
//����������ʱ��IReferenceObject�ͱ��������ͬ����������
//
struct TGIS_CORE_API IReferenceObject : public IInclusionObject
{
	IReferenceObject() 
	{
		_refCount = 1;
	};

	virtual ~IReferenceObject() {};

public:
	//Reference �� Release ���������д��û�ж�̬���ԣ�
	//Ҳ����˵Ҫ�󣬱����ڹ��������������ʵ���������
	void Reference()
	{
		assert(_is_in_heap);

		_refCount++;
	}

	void Release()
	{
		assert(_is_in_heap);

		if (_refCount > 0)
			_refCount--;
		if (_refCount == 0)
			delete this;
	}

private:
	int _refCount;
};

END_NAME_SPACE(tGis, Core)


#endif
