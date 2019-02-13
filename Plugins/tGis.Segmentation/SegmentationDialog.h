#pragma once


#ifndef __SEGMENTATIONDIALOG_H__
#define __SEGMENTATIONDIALOG_H__

#include <QWidget>
#include "ui_SegmentationDialog.h"
#include <string>

using namespace std;

class SegmentationDialog : public QDialog
{
public:
	SegmentationDialog(QWidget *parent = 0);
	~SegmentationDialog();


private:
	static QString _initialDir;

public:
	string _inputImage;
	string _outputShape;
	double _gaussianCoef;
	int _texturePeriod;
	double _lowPercentage;
	double _highPercentage;
	double _damStrength;
	double _poolStrength;

private slots:
	void on_btnInputImg_clicked(bool checked);
	void on_btnOutputShp_clicked(bool checked);
	void on_accepted();

private:
	Ui::SegmentationDialog ui;
};


#endif


