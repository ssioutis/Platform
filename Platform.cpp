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

/*
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
*/

int main()
{
	SDL_Event event;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;
	
	const int pathLength = 500;
	double *p = brownian_motion(pathLength);

	CRender r(RenderType::stockChart);
	CRender b(RenderType::backtest);
	

	r.setPath(p, pathLength);
	r.addTA();

	bool quit = false;

	//Currently, the only way to exit is to close the cmd output.
	while (!quit) {

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quit = true;

			r.update(event);
			b.update(event);
		}
	}

	SDL_Quit();
    return 0;
}

