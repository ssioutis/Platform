#ifndef CSTATS_H
#define CSTATS_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "ctechnical.h"
#include "CStrategy.h"
#include <QtWidgets/QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <vector>

using namespace std;

class AR1
{
public:
    //residuals
    double e(vector<double> data)
    {
        double e;
        vector<double> m = model(data.size());
        for (int i = 0; i < data.size(); i++)
            e += m[i] - data[i];
        return e;
    }

    vector<double> model(int n)
    {
        vector<double> ar;
        ar.push_back(phi0);
        for (int i = 0; i < n; i++)
            ar.push_back(phi1 * ar.back() + phi0);
        return ar;
    }

    double phi0;
    double phi1;
};

class CStats
{
public:
    CStats(QListWidget *output);

    void runUnivariateTests(CAsset a);

    //strategy performance? maybe should be part of CStrategy class
    //void performanceAnalysis()

    vector<double> acf(CAsset s, int max_lag = 30);
    vector<double> acf_test(CAsset s, int max_lag = 30);

    //need least squares!!
    vector<double> pacf(CAsset s, int max_lag = 30) {}

    AR1 fitAR(CAsset s);

    double lag_acf(CAsset s, int lag);
    double mean(CAsset s);
    double var(CAsset s);
    double cov(CAsset a, CAsset b);
    double corr(CAsset a, CAsset b);
    double skew(CAsset s) {}

private:
    QListWidget *out;
};

#endif // CSTATS_H
