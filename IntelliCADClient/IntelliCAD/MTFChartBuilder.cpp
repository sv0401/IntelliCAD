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
	// new XYChart(��Ʈ�� �׷��� ��ü ����), 10�� �ϵ��ڵ� ��Ÿ���� ���� ����
	__lastBuilt = make_shared<XYChart>(canvasWidth, canvasHeight + 9);

	// ��Ʈ�� ���� ���� (�׶���Ʈ)
	__lastBuilt->setBackground(
		__lastBuilt->linearGradientColor(
			0, 0, 0, __lastBuilt->getHeight() / 2, __bgStartColor, __bgEndColor));

	const Size2D<> PLOT_SIZE =
	{
		canvasWidth - ((2 * padding.width)),
		canvasHeight - ((2 * padding.height) + 20)	// ���� ����
	};

	// ��Ʈ �÷��� �׷��� ���� (�»�� ������, ũ��)
	PlotArea *const pPlotArea = __lastBuilt->setPlotArea(
		padding.width + 13, padding.height, PLOT_SIZE.width, PLOT_SIZE.height, __plotBgColor, -1, -1,
		__lastBuilt->dashLineColor(__horizGridColor, Chart::DotLine),
		__lastBuilt->dashLineColor(__vertGridColor, Chart::DotLine));

	// ���� �߰�
	__lastBuilt->addLegend(70, 13, false, "arialbd.ttf", 8)->setBackground(Chart::Transparent);

	// ���� �׷����� �׸���. (�׷��� �����͸� �Բ� �Է���)
	LineLayer *const pLayer = __lastBuilt->addLineLayer();
	pLayer->setLineWidth(2);

	pLayer->addDataSet(
		DoubleArray(__data.data(), static_cast<int>(__data.size())),
		__datalineColor, Parser::tstring$string(__datalineName).c_str());

	// X�࿡ ���� ������ �ٷ� �� �ִ� XAxis ��ü
	XAxis *const pXAxis = __lastBuilt->xAxis();
	pXAxis->setLinearScale(0., static_cast<double>(__data.size() - 1));

	// Tick ĸ�� ����
	pXAxis->setTitle(Parser::tstring$string(__xAxisTitle).c_str());

	YAxis *const pYAxis = __lastBuilt->yAxis();
	pYAxis->setLinearScale(0., 1.);
	pYAxis->setTitle(Parser::tstring$string(__yAxisTitle).c_str());

	return __lastBuilt;
}

void MTFChartBuilder::drawCrosshair(XYChart *const pChart, const int mouseXInPlot, const int mouseYInPlot)
{
	PlotArea *const pPlotArea = pChart->getPlotArea();

	// Crosshair�� �׸��� ���� �߰� ���̾� ����
	DrawArea *const pDrawArea = pChart->initDynamicLayer();

	// ���� �� �׸���
	pDrawArea->hline(
		pPlotArea->getLeftX(), pPlotArea->getRightX(),
		mouseYInPlot, pDrawArea->dashLineColor(BGR(10, 10, 10)));

	// ���� �� �׸���
	pDrawArea->vline(
		pPlotArea->getTopY(), pPlotArea->getBottomY(),
		mouseXInPlot, pDrawArea->dashLineColor(BGR(10, 10, 10)));

	// ���̺� �ޱ�
	ostringstream labelStream;
	const double INDEX = pChart->getXValue(mouseXInPlot);
	const double VALUE = pChart->getYValue(mouseYInPlot, pChart->yAxis());

	// X�� ���̺�
	labelStream <<
		"<*block,bgColor=FFFFDD,margin=4,edgeColor=0A0A0A*>" <<
		pChart->formatValue(INDEX, "{value|P0}") <<
		"<*/*>";

	TTFText *const pXLabelBox = pDrawArea->text(labelStream.str().c_str(), "arialbd.ttf", 8);
	pXLabelBox->draw(mouseXInPlot, pPlotArea->getBottomY() + 5, BGR(10, 10, 10), Chart::Top);
	pXLabelBox->destroy();

	// Y�� ���̺�
	labelStream.str("");
	labelStream <<
		"<*block,bgColor=FFFFDD,margin=4,edgeColor=0A0A0A*>" <<
		pChart->formatValue(VALUE, "{value|P2}") <<
		"<*/*>";

	TTFText *const pYLabelBox = pDrawArea->text(labelStream.str().c_str(), "arialbd.ttf", 8);
	pYLabelBox->draw(pPlotArea->getLeftX() - 5, mouseYInPlot, BGR(10, 10, 10), Chart::Right);
	pYLabelBox->destroy();
}