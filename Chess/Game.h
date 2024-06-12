#pragma once


#include "ChessLogic.h"
#include "ChessRenderer.h"


class Game {
private:
    ChessLogic chessLogic;
    ChessRenderer chessRenderer;
public:
    Game() {
        chessLogic.Init();
        chessRenderer.Init(&chessLogic);
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
                    chessLogic.MouseDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    chessLogic.MouseUp(event.button);
                    break;
                case SDL_MOUSEMOTION:
                    chessLogic.MouseMotion(event.motion);
                    break;
                }
            }
            chessRenderer.Render();
        }

	}
    ~Game() {
        SDL_Quit();
    }
};
