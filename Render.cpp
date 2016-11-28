#include "Render.h"


CRender::CRender(RenderType t)
{
	mPath = NULL;
	mdPath = NULL;

	switch (t)
	{
	case stockChart:
		mSDLWin = SDL_CreateWindow("Stock chart", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1000, 500,
			SDL_WINDOW_OPENGL);

		mSDLr = SDL_CreateRenderer(mSDLWin, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(mSDLr, 255, 255, 255, 255);
		SDL_RenderClear(mSDLr);

		init(1000, 500);

		break;
	case backtest:

		mSDLWin = SDL_CreateWindow("Backtest", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1000, 200,
			SDL_WINDOW_OPENGL);

		mSDLr = SDL_CreateRenderer(mSDLWin, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(mSDLr, 255, 255, 255, 255);
		SDL_RenderClear(mSDLr);

		init(1000, 200, 10, 10, 20, 11);

		break;
	default:
		break;
	}
	
}

CRender::~CRender()
{
	SDL_DestroyRenderer(mSDLr);
	SDL_DestroyWindow(mSDLWin);
	delete[] mPath;
}

//Called once by the constructor depending on RenderType.
//Each possible RenderType will have its own unique call to init()
void CRender::init(int WindowWidth, int WindowHeight, int GraphMarginX,
					int GraphMarginY, int XTicks, int YTicks) {

	mWinWidth = WindowWidth;
	mWinHeight = WindowHeight;
	mGraphWidth = mWinWidth - 2*GraphMarginX;
	mGraphHeight = mWinHeight - 2*GraphMarginY;
	mGraphMarginX = GraphMarginX;
	mGraphMarginY = GraphMarginY;
	mXTicks = XTicks;
	mYTicks = YTicks;

	SDL_SetRenderDrawColor(mSDLr, 255, 255, 255, 255);
	SDL_RenderClear(mSDLr);

	displayAxes();
}


void CRender::displayAxes() {

	int ymin = mGraphMarginY;
	int ymax = mWinHeight - mGraphMarginY;
	int xmin = mGraphMarginX;
	double dy = double(mGraphHeight) / double(mYTicks);
	double dx = double(mGraphWidth) / double(mXTicks);

	SDL_SetRenderDrawColor(mSDLr, 200, 200, 200, 200);

	for (int i = 0; i <= mYTicks; i++) {
		int y = mGraphMarginY + double(i)*dy;
		for (int j = mGraphMarginX; j < mGraphWidth; j += 10)
			SDL_RenderDrawLine(mSDLr, j, y, j + 5, y);
	}

	SDL_SetRenderDrawColor(mSDLr, 0, 0, 0, 0);
	SDL_RenderDrawLine(mSDLr, xmin, mGraphMarginY, xmin, mGraphHeight + mGraphMarginY);
	int zero = mGraphHeight / 2.0;
	SDL_RenderDrawLine(mSDLr, xmin, zero, mGraphWidth, zero);

	SDL_RenderPresent(mSDLr);
}

//Set the one (and currently, only) asset price path for this particular window
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

//Called repetitively during main loop.
//Needs to handle window events
void CRender::update(SDL_Event e) {
	
	if (e.type == SDL_QUIT) {
		this->~CRender();	//doesnt work
	}

	//draw main path
	SDL_SetRenderDrawColor(mSDLr, 0, 0, 0, 0);
	SDL_RenderDrawLines(mSDLr, mPath, mLen);

	if (mAddMA) {
		double *ma = new double[mLen];
		int outBeg;
		int outEl;

		TA_RetCode ret = TA_MA(0, mLen - 1, &mdPath[0], 20, TA_MAType_SMA, &outBeg, &outEl, &ma[0]);

		SDL_SetRenderDrawColor(mSDLr, 0, 0, 255, 0);
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


		SDL_SetRenderDrawColor(mSDLr, 200, 200, 255, 0);
		drawPath(bbu, outEl, outBeg);
		SDL_SetRenderDrawColor(mSDLr, 0, 0, 255, 0);
		drawPath(bbm, outEl, outBeg);
		SDL_SetRenderDrawColor(mSDLr, 200, 200, 255, 0);
		drawPath(bbd, outEl, outBeg);
	}
	SDL_RenderPresent(mSDLr);
}

//Draw any sequence of doubles on the graph
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

	SDL_RenderDrawLines(mSDLr, p, N);

	delete[] p;
}

void CRender::addTA() {
	mAddMA = false;
	mAddBB = true;
}