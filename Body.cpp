#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"

#include <iostream>
#include <random>

#include "Body.h"

int masses[9] = { 100, 200 ,300, 400, 500, 600, 700, 800, 1000 };

int mass[2] = { 1000000000, 1 };

static int i = 0;

Body::Body(float x, float y, SDL_Renderer* renderer) {
	mPosition.x = x;
	mPosition.y = y;

	mVelocity.x = 0;
	mVelocity.y = 0;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> index(0, 8);

	mMass = masses[index(gen)];
	//mMass = mass[i++];

	mDestRect.x = x;
	mDestRect.y = y;
	mDestRect.h = mDestRect.w = 20;

	//std::cout << "x: " << x << ", y: " << y << ", M: " << mMass << std::endl;

	SDL_Surface* surface = IMG_Load("Assets/Body.png");
	mTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}

Body::Body(float x, float y, float initialV, SDL_Renderer* renderer) {
	mPosition.x = x;
	mPosition.y = y;

	mVelocity.x = initialV;
	mVelocity.y = initialV;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> index(0, 8);

	mMass = masses[index(gen)];

	mDestRect.x = x;
	mDestRect.y = y;
	mDestRect.h = mDestRect.w = 20;

	std::cout << "x: " << x << ", y: " << y << ", M: " << mMass << std::endl;

	SDL_Surface* surface = IMG_Load("Assets/Body.png");
	mTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}