#pragma once

#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API Statistics
{
	double max;
	double min;
	double mean;
	double sum;
	bool   calcmean;//�Ƿ�����ֵ
	bool   calcvar;//�Ƿ���㷽��
	double sumvar;//�ͷ���
	double meanvar;//������
	double stddev;//��׼��
	size_t count;//����

	//��ʼ��ͳ����Ϣ�������
	void InitRecursive(double x,bool calcMean = false, bool calcVar = false);

	//һ�ε�������ʱ �ͷ��� ������ ��׼�� ���Ǵ��Լ���Ľ���ֵ
	void Recursive(double x);

	//��ʼ�� �ͷ��� ������ ��׼�� �������
	void InitRecursiveVariance();

	//��Ҫ�����׼ȷ�� �ͷ��� ������ ��׼��ô˷����ٵ���һ����������
	void RecursiveVariance(double x);

	void FinishRecursiveVariance();
};


END_NAME_SPACE(tGis, Core)

#endif

