#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <vector>
#include "SDL.h"

#undef main
using namespace std;


#define WIN_HEIGHT	500
#define WIN_WIDTH	1000
#define X_MIN		10
#define X_TICKS		600
#define Y_TICKS		20

double *brownian_motion(const int N) {
	default_random_engine gen(random_device{}());
	normal_distribution<double> dist;

	double *p = new double[N];

	p[0] = 0;

	for (int i = 1; i < N; i++) {
		p[i] = (dist(gen) + p[i - 1]);
	}

	return p;
}

void backtest(double *path, int N, double r) {
	double dt = 1.0/252.0;	//compound daily
	double rate = exp(r * dt);	//rate of cash growth
	double cash = 100;	//invested in bonds
	double shares = 1;	//invested in 'path'
	double returns = 0;	//portfolio returns
	double value = 0;	//portfolio value
	for (int i = 0; i < N; i++) {
		cash *= rate;
		returns = shares * path[i];
		value = cash + returns;
		cout << "returns: " << path[i] << "    \t cash value: " << cash << "\tportfolio returns: " << returns << "\tportfolio value: " << value << endl;
	}

	cout << endl << "total value: " << value;
}


void drawAxes(SDL_Renderer *r) {

	int ymin = 0;
	int ymax = WIN_HEIGHT;
	double dy = WIN_HEIGHT / Y_TICKS;

	SDL_SetRenderDrawColor(r, 200, 200, 200, 200);
	
	for (int i = ymin; i < ymax; i ++) {
		int y = i*dy;
		for (int j = X_MIN; j < WIN_WIDTH; j+=10)
			SDL_RenderDrawLine(r, j, y, j+5, y);
	}

	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderDrawLine(r, X_MIN, 0, X_MIN, WIN_HEIGHT);
	SDL_RenderDrawLine(r, X_MIN, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2);

	SDL_RenderPresent(r);
}

void renderPath(SDL_Renderer *r, double *path, int N) {

	SDL_Point *p = new SDL_Point[N];
	double dx = X_TICKS / N;

	for (int i = 0; i < N; i++) {
		SDL_Point pt;
		pt.x = dx*i + 20;
		pt.y = WIN_HEIGHT / 2 + (path[i] * 6);
		p[i] = pt;
	}

	SDL_SetRenderDrawColor(r, 0, 0, 255, 0);
	SDL_RenderDrawLines(r, p, N);
	SDL_RenderPresent(r);

	delete[] p;
}


int main()
{
	SDL_Event event;
	SDL_Window *win;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

	win = SDL_CreateWindow("Stock chart", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_OPENGL);

	if (win == NULL)
		return -1;

	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderClear(rend);
	
	int pathLength = 500;
	double *p = brownian_motion(pathLength);


	bool quit = false;

	drawAxes(rend);

	while (!quit) {

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quit = true;

			renderPath(rend, p, pathLength);
		}
	}

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}

