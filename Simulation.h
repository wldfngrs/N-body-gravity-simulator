#pragma once

#include <vector>

#include "Body.h"
#include "SDL.h"

class Simulation {
	const int mSimW = 800;
	const int mSimH = 600;

	const int mBodyLH = 20;

	const float h = 0.001;
	//const float h = 0.03;

	SDL_Window* mSimWindow = nullptr;
	SDL_Renderer* mSimRenderer = nullptr;

	SDL_Event mEvent;

	std::vector<Body> mBodies;
	int mNum;

	void initSDL();

	bool positionIsOccupied(float x, float y, size_t index);

	void initBodies();

	float calculateDistanceBetween(Vector2D positionA, Vector2D positionB) const;

	float calculateAngleBetween(Vector2D positionA, Vector2D positionB);

	void handleBorderCollision(Body& body) const;

	void updateBodies();

	void drawBodies();

public:
	Simulation(int N);

	void run();
};
