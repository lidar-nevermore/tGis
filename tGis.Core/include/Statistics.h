#pragma once

#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "Helper.h"


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

	Statistics(bool calcMean = false, bool calcVar = false);

	//�˽ӿڵ�������ʱ �ͷ��� ������ ��׼�� ���Ǵ��Լ���Ľ���ֵ
	//�˽ӿ�������ȡ
	void Recursive(double x);

	//��ʼ�� �ͷ��� ������ ��׼�� �������
	void InitRecursiveVariance();

	//�ýӿڵ��������׼ȷ�� �ͷ��� ������ ��׼��
	//�ýӿ���Ҫ��InitRecursiveVariance��FinishRecursiveVariance���ʹ��
	void RecursiveVariance(double x);

	void FinishRecursiveVariance();
};


END_NAME_SPACE(tGis, Core)

#endif

