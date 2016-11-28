#pragma once

#include <cmath>
#include <iostream>
#include "SDL.h"
#include "include\ta_libc.h"
#include "include\ta_func.h"

class CRender
{
public:
	CRender();
	~CRender();


	void init(SDL_Renderer *render, int WindowWidth, int WindowHeight, 
				int GraphWidth = NULL, int GraphHeight = NULL,
				int GraphMarginX = 10, int GraphMarginY = 10,
				int XTicks = 20, int YTicks = 21);

	void displayAxes();
	void setPath(double *path, int N, int offset = 0);
	void update();

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

	SDL_Renderer *r;
};

