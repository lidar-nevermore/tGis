#pragma once


#ifndef __REGIONMERGINGDIALOG_H__
#define __REGIONMERGINGDIALOG_H__

#include <QWidget>
#include "ui_RegionMergingDialog.h"
#include <string>

using namespace std;

class RegionMergingDialog : public QDialog
{
public:
	RegionMergingDialog(QWidget *parent = 0);
	~RegionMergingDialog();


private:
	static QString _initialDir;

public:
	string _inputImage;
	string _outputShape;
	double _colorWeight;
	double _compactWeight;
	double _smoothWeight;
	double _scale;

private slots:
	void on_btnInputImg_clicked(bool checked);
	void on_btnOutputShp_clicked(bool checked);
	void on_accepted();

private:
	Ui::RegionMergingDialog ui;
};


#endif


