#pragma once

#ifndef __WATEREXTRACTIONTOOL_H__
#define __WATEREXTRACTIONTOOL_H__

#include "tGisCore.h"
#include <string>

using namespace std;
using namespace tGis::Core;

class WaterExtractionTool : public ITool
{
public:
	static WaterExtractionTool INSTANCE;

public:
	WaterExtractionTool();
	~WaterExtractionTool();

public:
	const char* GetName();
	void Execute();

private:
	void WaterExtract(
		string& inputImage,
		string& aoiImage,
		string& outputDir,
		int subDataset,
		int band,
		int imgType,
		bool isBalanced,
		double lowRange,
		double highRange,
		int histBins,
		int radiusBins,
		int maxIteration);
};


#endif

