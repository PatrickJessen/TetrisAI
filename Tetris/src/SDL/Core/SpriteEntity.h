#pragma once
#include "Sprite.h"


class SpriteEntity
{
public:
	SpriteEntity(Sprite* sprite, SDL_Rect rect, int layer)
		: sprite(sprite), rect(rect), layer(layer)
	{}

	Sprite* sprite;
	SDL_Rect rect;
	int layer;
};