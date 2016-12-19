#ifndef CTRADINGPLATFORM_H
#define CTRADINGPLATFORM_H

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


class CTradingPlatform
{
public:
    CTradingPlatform();

    void Init(Ui::MainWindow *ui);

    void runStrategy();

    vector<double> SMA_crossover(vector<double> p);
	vector<double> RSI(vector<double> p);
    void plotBacktest(vector<double> x, vector<double> y);
    void plotChart(vector<double> x, vector<double> y);

    void addSMA(vector<double> y, int period);

private:
    vector<double> brownian_motion(const int N);

    void addCurve(pair<vector<double>, vector<double>> p, string title, Qt::GlobalColor c = Qt::black);

    QwtPlot *chartPlot;
    QWidget *chart;
    QWidget *backtest;
    QListWidget *output;
    QPlainTextEdit *debugOutput;
	CTechnical *ct;
	CStrategy *cs;
	
};

#endif // CTRADINGPLATFORM_H
