#ifndef CTECHNICAL_H
#define CTECHNICAL_H

#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <vector>

using namespace std;

class CTechnical
{
public:
    CTechnical() {debugOutput = NULL;}
    CTechnical(QPlainTextEdit *debug) {debugOutput = debug;}

    pair<vector<double>, vector<double>> SMA(vector<double> p, int period);
    pair<vector<double>, vector<double>> RSI(vector<double> p);

private:
    QPlainTextEdit *debugOutput;
};

#endif // CTECHNICAL_H
