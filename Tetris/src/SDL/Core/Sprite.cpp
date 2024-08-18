#include "Sprite.h"


Sprite::Sprite(std::string path, SDL_ScaleMode scaleMode)
{
	this->path = path;
	surface = IMG_Load(path.c_str());
	tex = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), surface);
	SDL_SetTextureScaleMode(tex, scaleMode);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surface);
}

void Sprite::ChangeSprite(std::string path, SDL_ScaleMode scaleMode)
{
	this->path = path;
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surface);
	surface = IMG_Load(path.c_str());
	tex = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), surface);
	SDL_SetTextureScaleMode(tex, scaleMode);
}

std::string Sprite::GetPath()
{
	return path;
}