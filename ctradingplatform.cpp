#include "ctradingplatform.h"

CTradingPlatform::CTradingPlatform()
{
}

void CTradingPlatform::Init(Ui::MainWindow *ui)
{
    chart = ui->Chart;
    backtest = ui->Backtest;
    debugOutput = ui->debugOutput;
    valueTable = ui->valueTable;
    autoStats = ui->autoStats;
    statsOutput = ui->statsOutput;

    chart->setGeometry(QRect(0, 0, 800, 400));
    backtest->setGeometry(QRect(0, 400, 800, 200));
    debugOutput->setGeometry(QRect(20, 630, 700, 200));

    valueTable->setGeometry(QRect(800, 20, 450, 500));
    valueTable->setRowCount(0);
    valueTable->setColumnCount(4);

    autoStats->setGeometry(QRect(800, 550, 440, 440));
    statsOutput->setGeometry(QRect(20,20,400,400));

    ct = new CTechnical(debugOutput);
    cs = new CStrategy(valueTable, debugOutput, ct);
    stat = new CStats(statsOutput);

    debugOutput->insertPlainText("Initialization complete\n");
}

//Execute the SMA crossover strategy
void CTradingPlatform::runStrategy()
{
	//Random stock path
    const int pathLength = 500;
    vector<double> y = brownian_motion(pathLength);
    vector<double> x;

	//Generate x data
    for (int i = 0; i < pathLength; i++)
        x.push_back(i);

    plotChart(x, y);

    cs->Init(x, y);

	//Backtesting results
    vector<double> backtest = cs->RSI();
    x.clear();

    for (int i = 0; i < backtest.size(); i++)
        x.push_back(i);

    plotBacktest(x, backtest);

    stat->runUnivariateTests(CAsset(x, y));

    debugOutput->insertPlainText("Running RSI ... done\n");
}

//Plot the backtesting window with the results of testing the strategy
void CTradingPlatform::plotBacktest(vector<double> x, vector<double> y)
{
    double max = -10000;
    double min = y[0];

	//find min and max to fit the graph dimensions
    for (int i = 0; i < y.size(); i++) {
        if (max < y[i]) max = y[i];
        if (min > y[i]) min = y[i];
    }

	//attach the plot to the backtest widget
    QwtPlot *plot = new QwtPlot(backtest);

    plot->setTitle("Backtest");
    plot->setCanvasBackground(Qt::white);
    plot->setAxisScale(QwtPlot::yLeft, min, max);
    plot->insertLegend(new QwtLegend());

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach(plot);

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle("Performance");
    curve->setPen(Qt::blue, 2);
    curve->setSamples(x.data(), y.data(), (int)x.size());
    curve->attach(plot);

    plot->resize(800, 200);
    plot->show();
}

//Plot a chart with 2 SMA's given stock data y indexed by x
void CTradingPlatform::plotChart(vector<double> x, vector<double> y)
{
    double max = -10000;
    double min = y[0];

	//find min and max to fit the graph dimensions
    for (int i = 0; i < y.size(); i++) {
        if (max < y[i]) max = y[i];
        if (min > y[i]) min = y[i];
    }

	//attach the plot object to the chart widget
    chartPlot = new QwtPlot(chart);

    chartPlot->setTitle("SMA Crossover");
    chartPlot->setCanvasBackground(Qt::white);
    chartPlot->setAxisScale(QwtPlot::yLeft, min, max);
    chartPlot->insertLegend(new QwtLegend());

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach(chartPlot);

    string title = "Stock data";
    addCurve(make_pair(x, y), title, Qt::blue);

    addSMA(y, 10);
    addSMA(y, 50);

    chartPlot->resize(800,400);
    chartPlot->show();

    debugOutput->insertPlainText("Plot Chart ... done\n");
}

//Add simple moving average to the chart
void CTradingPlatform::addSMA(vector<double> y, int period)
{
    string title = to_string(period) + string("-day SMA");
    pair<vector<double>, vector<double>> sma_data = ct->SMA(y, period);

    addCurve(sma_data, title, Qt::red);
}


void CTradingPlatform::addCurve(pair<vector<double>, vector<double>> p, string title, Qt::GlobalColor c)
{
    QwtPlotCurve *pc = new QwtPlotCurve();
    pc->setTitle(title.c_str());
    //need a way to rotate colors?
    pc->setPen(c, 1);
    pc->setSamples(p.first.data(), p.second.data(), (int)p.first.size());
    pc->attach(chartPlot);

    chartPlot->show();
}


//Returns a geometric brownian motion with length N
vector<double> CTradingPlatform::brownian_motion(const int N)
{
    default_random_engine gen(random_device{}());
    normal_distribution<double> dist;
    double sigma = 0.1;
    double mu = 0.1;
    double T = 1;
    double dt = T / double(N);

    vector<double> Wt(N);
    vector<double> p(N);

    Wt[0] = 0;
    p[0] = exp((mu + 1 / 2 * sigma * sigma)*dt + 0);

    for (int i = 1; i < N; i++) {
        Wt[i] = sqrt(dt)*dist(gen) + Wt[i - 1];
        p[i] = exp((mu - 1 / 2 * sigma * sigma)*dt + Wt[i]);
    }

    return p;
}
