#include "ctechnical.h"

//Simple moving average
//y contains the moving average data
//x contains the x-axis data
pair<vector<double>, vector<double>> CTechnical::SMA(vector<double> p, int period)
{
    vector<double> sma;
    vector<double> x;
    double avg = 0;
    for (int i = period; i < p.size(); i++) {
        avg = 0;
        for (int j = i - period; j <= i; j++) {
            avg += p[j];
        }
        sma.push_back(avg / period);
        x.push_back(i);
    }
    return make_pair(x, sma);
}

pair<vector<double>, vector<double>> CTechnical::RSI(vector<double> p)
{
    vector<double> RSI;
    vector<double> x;

    for (int i = 15; i < p.size(); i++)\
    {
        double gain = 0;
        double loss = 0;
        double ret = 0;
        for (int j = 1; j <= 14; j++)
        {
            int currentIdx = i-j;
            ret = p[currentIdx] - p[currentIdx-1];
            gain += (ret > 0) ? ret : 0;
            loss += (ret > 0) ? 0 : ret;
        }
        double avgGain_1 = gain/14;
		double avgLoss_1 = loss / 14;

        ret = p[i] - p[i-1];
        double currGain = (ret > 0) ? ret : 0;
		double currLoss = (ret > 0) ? 0 : ret;

        double avgGain = (avgGain_1*13 + currGain)/14;
        double avgLoss = abs(avgLoss_1*13 + currLoss)/14;

        double RS = avgGain / avgLoss;
		double dRSI = 100 - 100 / (1 + RS);
        RSI.push_back(dRSI);
        x.push_back(i);
	}
    return make_pair(x, RSI);
}
