#pragma once


#include "ChessRenderer.h"

#include <iostream>
#include <chrono>
#include <thread>

class Game {
    ChessRenderer chess;
public:
    Game() {
        chess.Init();
    }
	void Run() {
        bool quit = false;
        while(!quit) {
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    chess.MouseDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    chess.MouseUp(event.button);
                    break;
                case SDL_MOUSEMOTION:
                    chess.MouseMotion(event.motion);
                    break;
                }
            }

            chess.Render();
        }

	}
    ~Game() {
        SDL_Quit();
    }
};
