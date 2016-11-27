#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <vector>
#include "SDL.h"
#include "Render.h"

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


int main()
{
	SDL_Event event;
	SDL_Window *win;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

	win = SDL_CreateWindow("Stock chart", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_OPENGL);

	if (win == NULL)
		return -1;

	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderClear(rend);
	
	int pathLength = 500;
	double *p = brownian_motion(pathLength);

	CRender r;
	r.init(rend, WIN_WIDTH, WIN_HEIGHT);

	bool quit = false;

	while (!quit) {

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quit = true;

			r.drawPath(p, pathLength);
		}
	}

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}

