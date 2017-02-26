#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ctradingplatform.h"
#include <vector>
#include <fstream>
#include <fcntl.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CTradingPlatform tp;
	tp.Init(ui);
	tp.runStrategy();
}

MainWindow::~MainWindow()
{
    delete ui;
}










