#pragma once

#include "stdafx.h"
#include <memory>
#include "ChartViewer.h"
#include "MonoTransferFunction.h"
#include "Size2D.hpp"
#include "tstring.h"
#include "Color.hpp"

// Mono Transfer function Chart Builder
class MTFChartBuilder
{
private:
	std::vector<double> __data;
	int __numData = 0;

	std::shared_ptr<XYChart> __lastBuilt = nullptr;

	int __chainPrevIndex;
	double __chainPrevValue;
	bool __chainUpdate = false;

	int __bgStartColor = RGB(231, 220, 214);
	int __bgEndColor = RGB(251, 250, 240);

	int __plotBgColor = RGB(255, 255, 255);
	int __horizGridColor = RGB(190, 190, 190);
	int __vertGridColor = RGB(190, 190, 190);
	int __datalineColor = RGB(200, 150, 100);

public:
	Size2D<> padding = { 60, 40 };

	std::tstring __xAxisTitle = _T("Density");
	std::tstring __yAxisTitle = _T("Value");
	std::tstring __datalineName = _T("Value line");

	void import(const MonoTransferFunction &transferFunc);
	void setData(const int index, const double value);
	void finishSettingData();

	void setBackgroundColor(
		const ubyte redStart, const ubyte greenStart, const ubyte blueStart,
		const ubyte redEnd, const ubyte greenEnd, const ubyte blueEnd);

	void setPlotBackgroundColor(const ubyte red, const ubyte green, const ubyte blue);
	void setHorizGridColor(const ubyte red, const ubyte green, const ubyte blue);
	void setVertGridColor(const ubyte red, const ubyte green, const ubyte blue);
	void setDatalineColor(const ubyte red, const ubyte green, const ubyte blue);

	std::shared_ptr<XYChart> build(const int canvasWidth, const int canvasHeight);

	static void drawCrosshair(XYChart *const pChart, const int mouseXInPlot, const int mouseYInPlot);
};
