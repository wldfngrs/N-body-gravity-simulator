#pragma once

struct Vector2D {
	float x;
	float y;

	void zero() {
		x = y = 0;
	}

	void invert() {
		x = -x;
		y = -y;
	}
};