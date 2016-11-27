#pragma once

#include <cmath>
#include <iostream>
#include "SDL.h"



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
	void drawPath(double *path, int N);

private:
	int mWinWidth;
	int mWinHeight;
	int mGraphMarginX;
	int mGraphMarginY;
	int mGraphHeight;
	int mGraphWidth;
	int mXTicks;
	int mYTicks;
	
	SDL_Renderer *r;
};

