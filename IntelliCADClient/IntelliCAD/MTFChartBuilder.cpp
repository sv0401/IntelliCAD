#include <sstream>
#include "MTFChartBuilder.h"
#include "Parser.hpp"
#include "System.h"

#define ABGR(a, b, g, r) (((a) << 24) | RGB(b, g, r))
#define BGR(b, g, r) RGB(b, g, r)

using namespace std;

void MTFChartBuilder::import(const MonoTransferFunction &transferFunc)
{
	__data = transferFunc.exportAs<double>();
	__numData = static_cast<int>(__data.size());
}

void MTFChartBuilder::setData(const int index, const double value)
{
	if (!__chainUpdate)
	{
		__chainUpdate = true;

		if (index < __numData)
			__data[index] = value;
	}
	else
	{
		const double ITER = (index - __chainPrevIndex);
		const double LEVEL = ((value - __chainPrevValue) / ITER);

		if (__chainPrevIndex < index)
		{
			for (int i = (__chainPrevIndex + 1); i <= index; i++)
			{
				if (i >= __numData)
					break;

				__data[i] = (__chainPrevValue + ((i - __chainPrevIndex) * LEVEL));
			}
		}
		else
		{
			for (int i = (__chainPrevIndex - 1); i >= index; i--)
			{
				if (i >= __numData)
					continue;

				__data[i] = (__chainPrevValue - ((__chainPrevIndex - i) * LEVEL));
			}
		}
	}

	__chainPrevIndex = index;
	__chainPrevValue = value;
}

void MTFChartBuilder::finishSettingData()
{
	__chainUpdate = false;

	System::getSystemContents().getEventBroadcaster().
		notifyTransferFunctionUpdate(TransferFunctionType::IMAGE_PROCESSING, __data.data());
}

void MTFChartBuilder::setBackgroundColor(
	const ubyte redStart, const ubyte greenStart, const ubyte blueStart,
	const ubyte redEnd, const ubyte greenEnd, const ubyte blueEnd)
{
	__bgStartColor = BGR(blueStart, greenStart, redStart);
	__bgEndColor = BGR(blueEnd, greenEnd, redEnd);
}

void MTFChartBuilder::setPlotBackgroundColor(const ubyte red, const ubyte green, const ubyte blue)
{
	__plotBgColor = BGR(blue, green, red);
}

void MTFChartBuilder::setHorizGridColor(const ubyte red, const ubyte green, const ubyte blue)
{
	__horizGridColor = BGR(blue, green, red);
}

void MTFChartBuilder::setVertGridColor(const ubyte red, const ubyte green, const ubyte blue)
{
	__vertGridColor = BGR(blue, green, red);
}

void MTFChartBuilder::setDatalineColor(const ubyte red, const ubyte green, const ubyte blue)
{
	__datalineColor = BGR(blue, green, red);
}

shared_ptr<XYChart> MTFChartBuilder::build(const int canvasWidth, const int canvasHeight)
{
	// new XYChart(차트가 그려질 전체 영역), 10은 하드코딩 스타일의 마진 조정
	__lastBuilt = make_shared<XYChart>(canvasWidth, canvasHeight + 9);

	// 차트의 배경색 설정 (그라디언트)
	__lastBuilt->setBackground(
		__lastBuilt->linearGradientColor(
			0, 0, 0, __lastBuilt->getHeight() / 2, __bgStartColor, __bgEndColor));

	const Size2D<> PLOT_SIZE =
	{
		canvasWidth - ((2 * padding.width)),
		canvasHeight - ((2 * padding.height) + 20)	// 마진 조정
	};

	// 차트 플롯이 그려질 영역 (좌상단 꼭지점, 크기)
	PlotArea *const pPlotArea = __lastBuilt->setPlotArea(
		padding.width + 13, padding.height, PLOT_SIZE.width, PLOT_SIZE.height, __plotBgColor, -1, -1,
		__lastBuilt->dashLineColor(__horizGridColor, Chart::DotLine),
		__lastBuilt->dashLineColor(__vertGridColor, Chart::DotLine));

	// 범례 추가
	__lastBuilt->addLegend(70, 13, false, "arialbd.ttf", 8)->setBackground(Chart::Transparent);

	// 선형 그래프를 그린다. (그래프 데이터를 함께 입력함)
	LineLayer *const pLayer = __lastBuilt->addLineLayer();
	pLayer->setLineWidth(2);

	pLayer->addDataSet(
		DoubleArray(__data.data(), static_cast<int>(__data.size())),
		__datalineColor, Parser::tstring$string(__datalineName).c_str());

	// X축에 관한 정보를 다룰 수 있는 XAxis 객체
	XAxis *const pXAxis = __lastBuilt->xAxis();
	pXAxis->setLinearScale(0., static_cast<double>(__data.size() - 1));

	// Tick 캡션 설정
	pXAxis->setTitle(Parser::tstring$string(__xAxisTitle).c_str());

	YAxis *const pYAxis = __lastBuilt->yAxis();
	pYAxis->setLinearScale(0., 1.);
	pYAxis->setTitle(Parser::tstring$string(__yAxisTitle).c_str());

	return __lastBuilt;
}

void MTFChartBuilder::drawCrosshair(XYChart *const pChart, const int mouseXInPlot, const int mouseYInPlot)
{
	PlotArea *const pPlotArea = pChart->getPlotArea();

	// Crosshair를 그리기 위한 추가 레이어 생성
	DrawArea *const pDrawArea = pChart->initDynamicLayer();

	// 가로 축 그리기
	pDrawArea->hline(
		pPlotArea->getLeftX(), pPlotArea->getRightX(),
		mouseYInPlot, pDrawArea->dashLineColor(BGR(10, 10, 10)));

	// 세로 축 그리기
	pDrawArea->vline(
		pPlotArea->getTopY(), pPlotArea->getBottomY(),
		mouseXInPlot, pDrawArea->dashLineColor(BGR(10, 10, 10)));

	// 레이블 달기
	ostringstream labelStream;
	const double INDEX = pChart->getXValue(mouseXInPlot);
	const double VALUE = pChart->getYValue(mouseYInPlot, pChart->yAxis());

	// X축 레이블
	labelStream <<
		"<*block,bgColor=FFFFDD,margin=4,edgeColor=0A0A0A*>" <<
		pChart->formatValue(INDEX, "{value|P0}") <<
		"<*/*>";

	TTFText *const pXLabelBox = pDrawArea->text(labelStream.str().c_str(), "arialbd.ttf", 8);
	pXLabelBox->draw(mouseXInPlot, pPlotArea->getBottomY() + 5, BGR(10, 10, 10), Chart::Top);
	pXLabelBox->destroy();

	// Y축 레이블
	labelStream.str("");
	labelStream <<
		"<*block,bgColor=FFFFDD,margin=4,edgeColor=0A0A0A*>" <<
		pChart->formatValue(VALUE, "{value|P2}") <<
		"<*/*>";

	TTFText *const pYLabelBox = pDrawArea->text(labelStream.str().c_str(), "arialbd.ttf", 8);
	pYLabelBox->draw(pPlotArea->getLeftX() - 5, mouseYInPlot, BGR(10, 10, 10), Chart::Right);
	pYLabelBox->destroy();
}