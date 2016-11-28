#include "Render.h"


CRender::CRender()
{
	mPath = NULL;
	mdPath = NULL;
}

CRender::~CRender()
{
	SDL_DestroyRenderer(r);
	delete[] mPath;
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

void CRender::setPath(double *path, int N, int offset) {
	mPath = new SDL_Point[N];
	mdPath = path;
	mLen = N;
	mOffset = offset;

	double dx = double(mGraphWidth - mOffset) / double(mLen);
	int zero = mGraphHeight / 2.0;
	double yScale = double(mGraphHeight) / double(mYTicks);

	for (int i = 0; i < mLen; i++) {
		SDL_Point pt;
		pt.x = dx*i + mGraphMarginX + offset;
		pt.y = zero + (path[i] * 6);
		mPath[i] = pt;
	}
}

void CRender::update() {
	
	//draw main path
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderDrawLines(r, mPath, mLen);

	if (mAddMA) {
		double *ma = new double[mLen];
		int outBeg;
		int outEl;

		TA_RetCode ret = TA_MA(0, mLen - 1, &mdPath[0], 20, TA_MAType_SMA, &outBeg, &outEl, &ma[0]);

		SDL_SetRenderDrawColor(r, 0, 0, 255, 0);
		drawPath(ma, outEl, outBeg);
	}
	if (mAddBB) {
		double *bbu = new double[mLen];
		double *bbm = new double[mLen];
		double *bbd = new double[mLen];
		int outBeg;
		int outEl;
		TA_BBANDS(0, mLen - 1, &mdPath[0], 20, 2, 2, TA_MAType_SMA,
			&outBeg, &outEl, &bbu[0], &bbm[0], &bbd[0]);


		SDL_SetRenderDrawColor(r, 200, 200, 255, 0);
		drawPath(bbu, outEl, outBeg);
		SDL_SetRenderDrawColor(r, 0, 0, 255, 0);
		drawPath(bbm, outEl, outBeg);
		SDL_SetRenderDrawColor(r, 200, 200, 255, 0);
		drawPath(bbd, outEl, outBeg);
	}
	SDL_RenderPresent(r);
}

void CRender::drawPath(double *path, int N, int offset) {

	SDL_Point *p = new SDL_Point[N];
	double dx = double(mGraphWidth-offset) / double(N);
	int zero = mGraphHeight / 2.0;

	for (int i = 0; i < N; i++) {
		SDL_Point pt;
		pt.x = dx*i + mGraphMarginX + offset;
		pt.y = zero + (path[i] * 6);
		p[i] = pt;
	}

	SDL_RenderDrawLines(r, p, N);

	delete[] p;
}

void CRender::addTA() {
	mAddMA = false;
	mAddBB = true;
}