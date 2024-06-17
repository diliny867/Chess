#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "Common.h"

class Image{
private:
    SDL_Rect destination = {0,0,0,0};
    i32Vec2 sourceScale = {0,0};
    SDL_Texture* texture = nullptr;
public:
    i32Vec2 position = {0,0};
    i32Vec2 size = {0,0};

    void Init(SDL_Renderer* renderer, const std::string& path){
        SDL_Surface* surface = IMG_Load(path.c_str());
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        size.x = surface->w;
    	size.y = surface->h;
        destination.w = size.x;
        destination.h = size.y;

        SDL_FreeSurface(surface);
    }

    void Render(SDL_Renderer* renderer){
        destination.x = position.x;
        destination.y = position.y;
        destination.w = size.x;
        destination.h = size.y;

        SDL_RenderCopy(renderer, texture, NULL, &destination);
    }

    ~Image(){
        SDL_DestroyTexture(texture);
    }
};