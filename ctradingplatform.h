#ifndef CTRADINGPLATFORM_H
#define CTRADINGPLATFORM_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "ctechnical.h"
#include "CStrategy.h"
#include "cstats.h"
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

    void plotBacktest(vector<double> x, vector<double> y);
    void plotChart(vector<double> x, vector<double> y);

    // Technical overlays

    void addSMA(vector<double> y, int period);

private:
    vector<double> brownian_motion(const int N);

    void addCurve(pair<vector<double>, vector<double>> p, string title, Qt::GlobalColor c = Qt::black);

	//GUI
    
	QwtPlot *chartPlot;
    QWidget *chart;
    QWidget *backtest;
    QPlainTextEdit *debugOutput;
    QTableWidget *valueTable;
    QGroupBox *autoStats;
    QListWidget *statsOutput;
    QTabWidget *tabPages;
    QWidget *page1;
    QWidget *liveData;

	//Non-GUI

	CTechnical *ct;
	CStrategy *cs;
    CStats *stat;
};

#endif // CTRADINGPLATFORM_H
