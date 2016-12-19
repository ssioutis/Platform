#include "ctradingplatform.h"

CTradingPlatform::CTradingPlatform()
{
}

void CTradingPlatform::Init(Ui::MainWindow *ui)
{
    chart = ui->Chart;
    backtest = ui->Backtest;
    output = ui->backtestOutput;
    debugOutput = ui->debugOutput;

    chart->setGeometry(QRect(0, 0, 800, 400));
    backtest->setGeometry(QRect(0, 400, 800, 200));
    output->setGeometry(QRect(800, 20, 300, 400));
    debugOutput->setGeometry(QRect(20, 630, 800, 200));

    ct = new CTechnical(debugOutput);
	cs = new CStrategy(output, debugOutput, ct);

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

	//Backtesting results
    //vector<double> backtest = RSI(y);
	vector<double> backtest = cs->RSI(y);
    x.clear();

    for (int i = 0; i < backtest.size(); i++)
        x.push_back(i);

    plotBacktest(x, backtest);

    debugOutput->insertPlainText("Running RSI ... done\n");
}


vector<double> CTradingPlatform::SMA_crossover(vector<double> p)
{
    double dt = 1.0 / 252.0;	//compound daily
    double r = 0.01;
    double rate = exp(r * dt);	//rate of cash growth
    double cash = 100;	//invested in bonds
    double shares = 10;	//invested in 'path'
    double returns = 0;	//portfolio returns
    vector<double> value;	//portfolio value

    int slowPeriod = 50;
    int fastPeriod = 10;

    pair<vector<double>, vector<double>> ma_slow = ct->SMA(p, slowPeriod);
    pair<vector<double>, vector<double>> ma_fast = ct->SMA(p, fastPeriod);

    bool buySignal = false;
    bool shortSignal = false;

    int idx = 0;

    double longSize = 0.9; //90% of cash used for each buy order
    double sellSize = 0.5; //sell 90% of shares

    for (int i = 1; i < p.size(); i++) {

		//a 20 day SMA has the first 20 days blank...
        if (i > slowPeriod) {
            idx = i - slowPeriod;
			//A buy signal is when the fast SMA cross above the slow SMA
            if ((ma_slow.second[idx] < ma_fast.second[idx]) && cash > 0) {
				//only if the fast SMA was below the slow SMA in the last period
                if (ma_slow.second[idx - 1] > ma_fast.second[idx - 1]) {
					//size of this order
					double numShares = longSize * cash / p[i];
                    shares += numShares;

					double totalCost = p[i] * numShares;
					cash -= totalCost;

                    string out = "Buy shares at x = " + to_string(i) + ". Total shares: " + to_string(shares);
                    output->insertItem(output->count() + 1, QString(out.c_str()));
                }
            }

			//look for a sell signal
            if (ma_slow.second[idx] > ma_fast.second[idx]) {
                if ((ma_slow.second[idx - 1] < ma_fast.second[idx - 1]) && shares >= 10) {
					double numShares = sellSize * shares;
					shares -= numShares;

					double totalProfit = numShares * p[i];
					cash += totalProfit;

                    string out = "Sold shares at x = " + to_string(i) + ". Total shares: " + to_string(shares);
                    output->insertItem(output->count() + 1, QString(out.c_str()));
                }
            }
        }

        cash *= rate;
        value.push_back(cash + shares*p[i]);
    }

    return value;
}

vector<double> CTradingPlatform::RSI(vector<double> p)
{
	double dt = 1.0 / 252.0;	//compound daily
	double r = 0.01;
	double rate = exp(r * dt);	//rate of cash growth
	double cash = 100;	//invested in bonds
	double shares = 10;	//invested in 'path'
	double returns = 0;	//portfolio returns
	vector<double> value;	//portfolio value

	pair<vector<double>, vector<double>> rsi = ct->RSI(p);

	bool buySignal = false;
	bool shortSignal = false;

	int idx = 0;

	double longSize = 0.9; //90% of cash used for each buy order
	double sellSize = 0.5; //sell 90% of shares

	int start = 15;

	for (int i = 1; i < p.size(); i++) {

		if (i > start) {
			idx = i - start;
			if ((rsi.second[idx] < 30) && cash > 0) {
				//size of this order
				double numShares = longSize * cash / p[i];
				shares += numShares;

				double totalCost = p[i] * numShares;
				cash -= totalCost;

				string out = "Buy shares at x = " + to_string(i) + ". Total shares: " + to_string(shares);
				output->insertItem(output->count() + 1, QString(out.c_str()));
			}

			//look for a sell signal
			if (rsi.second[idx] > 70 & shares > 0) {
				double numShares = sellSize * shares;
				shares -= numShares;

				double totalProfit = numShares * p[i];
				cash += totalProfit;

				string out = "Sold shares at x = " + to_string(i) + ". Total shares: " + to_string(shares);
				output->insertItem(output->count() + 1, QString(out.c_str()));
			}
		}

		cash *= rate;
		value.push_back(cash + shares*p[i]);
	}

	return value;
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
        p[i] = exp((mu + 1 / 2 * sigma * sigma)*dt + Wt[i]);
    }

    return p;
}
