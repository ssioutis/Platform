#include "CStrategy.h"



CStrategy::CStrategy(QTableWidget *out, QPlainTextEdit *debug, CTechnical *tech)
{
	output = out;
	debugOutput = debug;
	ct = tech;
}


CStrategy::~CStrategy()
{
}

void CStrategy::Init(vector<double> x, vector<double> y)
{
    s.x = x;
    s.value = y;
    reset();
}

void CStrategy::reset()
{
    dt = 1.0 / 252.0;	//compound daily
    r = 0.01;
    rate = exp(r * dt);	//rate of cash growth
    cash = 100;	//invested in bonds
    shares = 10;	//invested in 'path'
    returns = 0;	//portfolio returns
    txnCost = 0.01;

    output->clearContents();
    lastRow = 0;
}

void CStrategy::evolve(int t)
{
    cash *= rate;
    value.push_back(cash + shares*s.value[t]);
}

void CStrategy::updateTable(string order)
{
    //1st cell is an order description with number of shares
    output->insertRow(lastRow);
    output->setItem(lastRow, 0, new QTableWidgetItem(QString(order.c_str())));


    //2nd cell is current value of portfolio
    string out = to_string(value.back());
    output->setItem(lastRow, 1, new QTableWidgetItem(QString(out.c_str())));

    //3rd cell is cash amount
    out = to_string(cash);
    output->setItem(lastRow, 2, new QTableWidgetItem(QString(out.c_str())));

    //4th cell is number of shares
    out = to_string(shares);
    output->setItem(lastRow, 3, new QTableWidgetItem(QString(out.c_str())));

    lastRow++;
}

void CStrategy::execLong(int size, int t)
{
    shares += size;

    double totalCost = s.value[t] * size + txnCost;
    cash -= totalCost;

    string out = "Buy " + to_string(size) + " shares";
    updateTable(out);
}

void CStrategy::execShort(int size, int t)
{
    shares -= size;

    double totalProfit = size * s.value[t] - txnCost;
    cash += totalProfit;

    string out = "Sell " + to_string(size) + " shares";
    updateTable(out);
}



vector<double> CStrategy::SMA_crossover(vector<double> p)
{
    reset();

	int slowPeriod = 50;
	int fastPeriod = 10;

	pair<vector<double>, vector<double>> ma_slow = ct->SMA(p, slowPeriod);
	pair<vector<double>, vector<double>> ma_fast = ct->SMA(p, fastPeriod);

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

					double numShares = longSize * cash / p[i];
                    execLong(numShares, i);
				}
			}

			//look for a sell signal
			if (ma_slow.second[idx] > ma_fast.second[idx]) {
				if ((ma_slow.second[idx - 1] < ma_fast.second[idx - 1]) && shares >= 10) {

                    double numShares = sellSize * shares;
                    execShort(numShares, i);
				}
			}
		}

        evolve(i);
	}

	return value;
}

vector<double> CStrategy::RSI()
{
    reset();

    vector<double> p = s.value;

	pair<vector<double>, vector<double>> rsi = ct->RSI(p);

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
                execLong(numShares, i);
			}

			//look for a sell signal
			if (rsi.second[idx] > 70 & shares > 0) {
				double numShares = sellSize * shares;
                execShort(numShares, i);
			}
		}

        evolve(i);
	}

	return value;
}
