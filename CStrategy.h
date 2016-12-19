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

class CStrategy
{
public:
	CStrategy(QListWidget *out, QPlainTextEdit *debug, CTechnical *tech);
	~CStrategy();

	vector<double> SMA_crossover(vector<double> p);
	vector<double> RSI(vector<double> p);

private:
	QListWidget *output;
	QPlainTextEdit *debugOutput;
	CTechnical *ct;
};

