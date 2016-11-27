#include "Render.h"


CRender::CRender()
{
}

CRender::~CRender()
{
	SDL_DestroyRenderer(r);
}

void CRender::init(SDL_Renderer *render, int WindowWidth, int WindowHeight, int GraphWidth,
	int GraphHeight, int GraphMarginX, int GraphMarginY, int XTicks, int YTicks) {

	r = render;

	mWinWidth = WindowWidth;
	mWinHeight = WindowHeight;
	mGraphWidth = mWinWidth - 2*GraphMarginX;
	mGraphHeight = mWinHeight - 2*GraphMarginY;
	mGraphMarginX = GraphMarginX;
	mGraphMarginY = GraphMarginY;
	mXTicks = XTicks;
	mYTicks = YTicks;

	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	SDL_RenderClear(r);

	displayAxes();
}

void CRender::displayAxes() {

	int ymin = mGraphMarginY;
	int ymax = mWinHeight - mGraphMarginY;
	int xmin = mGraphMarginX;
	double dy = double(mGraphHeight) / double(mYTicks);
	double dx = double(mGraphWidth) / double(mXTicks);

	SDL_SetRenderDrawColor(r, 200, 200, 200, 200);

	for (int i = 0; i <= mYTicks; i++) {
		int y = mGraphMarginY + double(i)*dy;
		for (int j = mGraphMarginX; j < mGraphWidth; j += 10)
			SDL_RenderDrawLine(r, j, y, j + 5, y);
	}

	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderDrawLine(r, xmin, mGraphMarginY, xmin, mGraphHeight + mGraphMarginY);
	int zero = double(mGraphHeight) / 2.0;
	SDL_RenderDrawLine(r, xmin, zero, mGraphWidth, zero);

	SDL_RenderPresent(r);
}


void CRender::drawPath(double *path, int N) {

	SDL_Point *p = new SDL_Point[N];
	double dx = double(mGraphWidth) / double(N);
	int zero = mGraphHeight / 2.0;

	for (int i = 0; i < N; i++) {
		SDL_Point pt;
		pt.x = dx*i + mGraphMarginX;
		pt.y = zero + (path[i] * 6);
		p[i] = pt;
	}

	SDL_SetRenderDrawColor(r, 0, 0, 255, 0);
	SDL_RenderDrawLines(r, p, N);
	SDL_RenderPresent(r);

	delete[] p;
}