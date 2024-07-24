#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"

struct Body {
	SDL_Texture* mTexture;
	SDL_Rect mDestRect;

	Vector2D mPosition;
	Vector2D mVelocity;

	float mMass;

	Body(float x, float y, SDL_Renderer* renderer);

	Body(float x, float y, float initialV, SDL_Renderer* renderer);
};