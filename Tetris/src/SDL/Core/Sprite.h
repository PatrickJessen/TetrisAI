#pragma once
#include "SDL_image.h"
#include "../Render/Renderer.h"
#include <string>

class Sprite
{
public:
	Sprite(std::string path, SDL_ScaleMode scaleMode);
	Sprite() {};
	~Sprite();

	void ChangeSprite(std::string path, SDL_ScaleMode scaleMode);

	std::string GetPath();

private:
	std::string path;

public:
	SDL_Surface* surface;
	SDL_Texture* tex;
};