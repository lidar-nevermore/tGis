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
	string _inputImage;
	string _outputShape;
	QString _initialDir;

public:
	const char* GetInputImage();
	const char* GetOutputShape();

private slots:
	void on_btnInputImg_clicked(bool checked);
	void on_btnOutputShp_clicked(bool checked);

private:
	Ui::SegmentationDialog ui;
};


#endif


