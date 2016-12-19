#include "CStrategy.h"



CStrategy::CStrategy(QListWidget *out, QPlainTextEdit *debug, CTechnical *tech)
{
	output = out;
	debugOutput = debug;
	ct = tech;
}


CStrategy::~CStrategy()
{
}

vector<double> CStrategy::SMA_crossover(vector<double> p)
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

vector<double> CStrategy::RSI(vector<double> p)
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