#pragma once

#include <iostream>
#include <random>

#include "Body.h"
#include "SDL.h"

#include "Simulation.h"

constexpr float G = 6.67430e-11;
constexpr float pi = 3.14159265;

Simulation::Simulation(int N) {
	mNum = N;

	initSDL();
	initBodies();
}

void Simulation::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cerr << "Error initializing SDL!";
		exit(-1);
	}

	char title[50];

	sprintf_s(title, 50, "%d-body gravity simulator", mNum);

	mSimWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSimW, mSimH, SDL_WINDOW_SHOWN);

	mSimRenderer = SDL_CreateRenderer(mSimWindow, -1, 0);
}

bool Simulation::positionIsOccupied(float x, float y, size_t index) {
	for (size_t i = 0; i < index; i++) {
		if (i == index) {
			continue;
		}

		if (x + mBodyLH >= mBodies[i].mPosition.x
			&& mBodies[i].mPosition.x + mBodyLH >= x
			&& y + mBodyLH >= mBodies[i].mPosition.y
			&& mBodies[i].mPosition.y + mBodyLH >= y)
		{
			return true;

		}
	}

	return false;
}

void Simulation::initBodies() {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> yDistr(0, mSimH - mBodyLH);
	std::uniform_int_distribution<> xDistr(0, mSimW - mBodyLH);
	std::uniform_int_distribution<> velocityDistr(5, 10);

	for (size_t i = 0; i < mNum; i++) {
		float x = xDistr(gen);
		float y = yDistr(gen);

		int loopCount = 0;
		while (positionIsOccupied(x, y, i)) {
			x = static_cast<float>(xDistr(gen));
			y = static_cast<float>(yDistr(gen));
			loopCount++;

			if (loopCount >= mNum) {
				break;
			}
		}

		mBodies.emplace_back(x, y, velocityDistr(gen), mSimRenderer);

		SDL_RenderCopy(mSimRenderer, mBodies[i].mTexture, NULL, &mBodies[i].mDestRect);

		SDL_RenderPresent(mSimRenderer);
	}
}

float Simulation::calculateDistanceBetween(Vector2D positionA, Vector2D positionB) const {
	float xSqrd = ((positionA.x + mBodyLH / 2.0) - (positionB.x + mBodyLH / 2.0)) * ((positionA.x + mBodyLH / 2.0) - (positionB.x + mBodyLH / 2.0));
	float ySqrd = ((positionA.y + mBodyLH / 2.0) - (positionB.y + mBodyLH / 2.0)) * ((positionA.y + mBodyLH / 2.0) - (positionB.y + mBodyLH / 2.0));

	float diff = sqrt(xSqrd + ySqrd);

	return diff;
}

float Simulation::calculateAngleBetween(Vector2D positionA, Vector2D positionB) {
	float opp = abs(positionA.y - positionB.y);
	float adj = abs(positionA.x - positionB.x);

	float arctan = atanf(opp / adj);

	return arctan;
}

void Simulation::handleBorderCollision(Body& body) const {
	if ((body.mPosition.x + mBodyLH >= mSimW) || (body.mPosition.x <= 0)) {
		body.mVelocity.x = -body.mVelocity.x;
	}

	if ((body.mPosition.y + mBodyLH >= mSimH) || (body.mPosition.y <= 0)) {
		body.mVelocity.y = -body.mVelocity.y;
	}
}

void Simulation::updateBodies() {
	for (int i = 0; i < mNum; i++) {
		for (int j = 0; j < mNum; j++) {
			if (j == i) {
				continue;
			}

			float num = mBodies[i].mMass * mBodies[j].mMass;
			float den = calculateDistanceBetween(mBodies[i].mPosition, mBodies[j].mPosition);

			float angle = calculateAngleBetween(mBodies[i].mPosition, mBodies[j].mPosition);

			float force = G * num / den;

			float force_x = force * cos(angle);
			float force_y = force * sin(angle);

			float accelerationBodyA_x = force_x / mBodies[i].mMass;
			float accelerationBodyA_y = force_y / mBodies[i].mMass;

			float accelerationBodyB_x = force_x / mBodies[j].mMass;
			float accelerationBodyB_y = force_y / mBodies[i].mMass;

			mBodies[i].mVelocity.x += accelerationBodyA_x * h;
			mBodies[i].mVelocity.y += accelerationBodyA_y * h;

			mBodies[i].mPosition.x += mBodies[i].mVelocity.x * h;
			mBodies[i].mPosition.y += mBodies[i].mVelocity.y * h;

			handleBorderCollision(mBodies[i]);

			mBodies[i].mDestRect.x = mBodies[i].mPosition.x;
			mBodies[i].mDestRect.y = mBodies[i].mPosition.y;

			//

			mBodies[j].mVelocity.x += accelerationBodyB_x * h;
			mBodies[j].mVelocity.y += accelerationBodyB_y * h;

			mBodies[j].mPosition.x += mBodies[j].mVelocity.x * h;
			mBodies[j].mPosition.y += mBodies[j].mVelocity.y * h;

			handleBorderCollision(mBodies[j]);

			mBodies[j].mDestRect.x = mBodies[j].mPosition.x;
			mBodies[j].mDestRect.y = mBodies[j].mPosition.y;

			//std::cout << "x: " << mBodies[i].mDestRect.x << ", y: " << mBodies[i].mDestRect.y << ", M: " << mBodies[i].mMass << std::endl;
			//std::cout << "x: " << mBodies[i].mDestRect.x << ", y: " << mBodies[i].mDestRect.y << ", M: " << mBodies[i].mMass << std::endl;
		}
	}
}

void Simulation::drawBodies() {
	SDL_RenderClear(mSimRenderer);

	for (int i = 0; i < mNum; i++) {
		SDL_RenderCopy(mSimRenderer, mBodies[i].mTexture, NULL, &mBodies[i].mDestRect);
	}

	SDL_RenderPresent(mSimRenderer);
}

void Simulation::run() {
	while (true) {
		SDL_PollEvent(&mEvent);

		switch (mEvent.type) {
		case SDL_QUIT:
			exit(0);
		default:
			updateBodies();
			drawBodies();
		}
	}
}
