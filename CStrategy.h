#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "ctechnical.h"
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

class CAsset
{
public:
    CAsset() {}
    CAsset(vector<double>_x, vector<double> _y) {x = _x, value = _y;}
    pair<vector<double>, vector<double>> makepair();

    vector<double> x;
    vector<double> value;
};

class CStrategy
{
public:
    CStrategy(QTableWidget *out, QPlainTextEdit *debug, CTechnical *tech);
	~CStrategy();

	vector<double> SMA_crossover(vector<double> p);
    vector<double> RSI();

    void Init(vector<double> x, vector<double> y);

private:
    void reset();
    void evolve(int t);
    void execLong(int size, int t);
    void execShort(int size, int t);
    void updateTable(string order);

	vector<double> asset_data;
    QTableWidget *output;
	QPlainTextEdit *debugOutput;
	CTechnical *ct;

    CAsset s;

    vector<double> value;
    double r;
    double rate;
    double T;
    double cash;
    double dt;
    double shares;
    double returns;
    double txnCost;


    int lastRow;
};

