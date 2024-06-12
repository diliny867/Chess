#pragma once


#include "ChessLogic.h"
#include "ChessRenderer.h"


class Game {
private:
    ChessLogic chessLogic;
    ChessRenderer chessRenderer;

    void mouseDown(const SDL_MouseButtonEvent& mouseButton) {
        mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? true : mouse.left;
        mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? true : mouse.right;
        mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? true : mouse.middle;

        if(mouseButton.button == SDL_BUTTON_LEFT) {
            chessLogic.SelectPiece(GetMousePiece());
        }
        if(mouseButton.button == SDL_BUTTON_RIGHT) {
            chessLogic.DeselectPiece();
        }
    }
    void mouseUp(const SDL_MouseButtonEvent& mouseButton) {
        mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? false : mouse.left;
        mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? false : mouse.right;
        mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? false : mouse.middle;

        if(mouseButton.button == SDL_BUTTON_LEFT) {
            chessLogic.CheckMouseDeselect(GetMousePiece());
        }
    }
    void mouseMotion(const SDL_MouseMotionEvent& mouseMotion) {
        mouse.pos.x = mouseMotion.x;
        mouse.pos.y = mouseMotion.y;

        chessLogic.CheckMouseLeftSquare(GetMousePiece());
    }
public:
    inline static constexpr int squareSize = 80;
    inline static constexpr SDL_Point screenSize = {640, 640};

    struct Mouse {
        struct {
            i32 x;
            i32 y;
        } pos;
        bool left;
        bool right;
        bool middle;
    };
    inline static Mouse mouse ={{0,0},false,false,false};
    inline static ChessBase::XY GetMousePiece() {
        return {static_cast<i8>(mouse.pos.x/squareSize), static_cast<i8>((screenSize.y - mouse.pos.y)/squareSize)};
    }

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
                    mouseDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseUp(event.button);
                    break;
                case SDL_MOUSEMOTION:
                    mouseMotion(event.motion);
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
