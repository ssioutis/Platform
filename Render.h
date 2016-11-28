#pragma once

#include <cmath>
#include <iostream>
#include "SDL.h"
#include "include\ta_libc.h"
#include "include\ta_func.h"

enum RenderType {
	stockChart = 1,
	backtest = 2
};

class CRender
{
public:
	CRender(RenderType t);
	~CRender();


	void init(int WindowWidth, int WindowHeight, 
				int GraphMarginX = 10, int GraphMarginY = 10,
				int XTicks = 20, int YTicks = 21);

	void displayAxes();
	void setPath(double *path, int N, int offset = 0);
	void update(SDL_Event e);

	void addTA();

private:
	void drawPath(double *path, int N, int offset = 0);

	int mWinWidth;
	int mWinHeight;
	int mGraphMarginX;
	int mGraphMarginY;
	int mGraphHeight;
	int mGraphWidth;
	int mXTicks;
	int mYTicks;
	
	SDL_Point *mPath;
	double *mdPath;
	int mLen;
	int mOffset;

	bool mAddMA = false;
	bool mAddBB = false;

	SDL_Window *mSDLWin;
	SDL_Renderer *mSDLr;
};

