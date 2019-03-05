#pragma once

#ifndef __WATEREXTRATIONDIALOG_H__
#define __WATEREXTRATIONDIALOG_H__

#include <QWidget>
#include "ui_WaterExtrationDialog.h"
#include <string>

using namespace std;

class WaterExtrationDialog : public QDialog
{
public:
	WaterExtrationDialog(QWidget *parent = 0);
	~WaterExtrationDialog();

private:
	static QString _initialDir;

public:
	string _inputImage;
	string _aoiImage;
	string _outputDir;

	int _subDataset;
	int _band;
	int _imageType;

	double _expThreshold;
	double _lowRange;
	double _highRange;

	int _histBins;
	int _radiusBins;
	int _maxIteration;

private slots:
    void on_btnInputImage_clicked(bool checked);
	void on_btnAoiImage_clicked(bool checked);
	void on_btnOutputDir_clicked(bool checked);
	void on_accepted();

private:
	Ui::WaterExtrationDialog ui;
};


#endif
