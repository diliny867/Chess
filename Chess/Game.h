#pragma once


#include "ChessLogic.h"
#include "ChessRenderer.h"
#include "MenuRenderer.h"


class Game {
private:
    ChessLogic chessLogic;
    ChessRenderer chessRenderer;
    MenuRenderer menuRenderer;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool mouseInChessBoard() {
        return mouse.pos.x >= boardPosition.x && mouse.pos.x <= (boardPosition.x + boardSize.x) &&
            mouse.pos.y >= boardPosition.y && mouse.pos.y <= (boardPosition.y + boardSize.y);
    }
    void mouseDown(const SDL_MouseButtonEvent& mouseButton) {
        mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? true : mouse.left;
        mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? true : mouse.right;
        mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? true : mouse.middle;

        if(mouseButton.button == SDL_BUTTON_LEFT) {
            if(mouseInChessBoard()){
                chessLogic.SelectPiece(GetMousePiece());
            }
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
    void initWindow() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        }

        window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Game::windowSize.x, Game::windowSize.y, SDL_WINDOW_SHOWN);
        if(window == nullptr) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        }

        renderer = SDL_CreateRenderer(Game::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    }
public:
    inline static constexpr SDL_Point menuSize = {0, 640};
    inline static constexpr SDL_Point boardSize = {640, 640};
    inline static constexpr SDL_Point boardPosition = {0,0};
    inline static constexpr SDL_Point menuPosition = {boardSize.x,0};
    inline static constexpr i32 squareSize = boardSize.x/8;
    inline static constexpr SDL_Point windowSize = {std::max(menuPosition.x+menuSize.x, boardPosition.x+boardSize.x),
    	std::max(menuPosition.y+menuSize.y, boardPosition.y+boardSize.y)};

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
    inline static SDL_Point GetMouseRelative(SDL_Point position) {
        return {mouse.pos.x - position.x, mouse.pos.y - position.y};
    }
    inline static ChessBase::XY GetMousePiece() {
        const auto relative = GetMouseRelative(boardPosition);
        return {static_cast<i8>(relative.x/squareSize), static_cast<i8>((boardSize.y - relative.y)/squareSize)};
    }

    Game() {
        initWindow();
        chessLogic.Init();
        chessRenderer.Init(this, boardSize);
        //menuRenderer.Init(this, menuSize);
    }
    ChessLogic* GetChessLogic() {
        return &chessLogic;
    }
    SDL_Window* GetWindow() {
        return window;
    }
    SDL_Renderer* GetRenderer() {
        return renderer;
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
            chessRenderer.Render(renderer, boardPosition, boardSize);
            //menuRenderer.Render(renderer, menuPosition, menuSize);
        }

	}
    ~Game() {
        SDL_Quit();
    }
};
