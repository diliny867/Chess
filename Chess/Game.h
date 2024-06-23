#pragma once


#include "ChessLogic.h"
#include "ChessRenderer.h"
#include "MenuLogic.h"
#include "MenuRenderer.h"


class Game {
private:
    friend MenuLogic;
    ChessLogic chessLogic;
    ChessRenderer chessRenderer;
    MenuLogic menuLogic;
    MenuRenderer menuRenderer;
    GameSectionRenderer overlayRenderer;

    inline static Game* gameInstance = nullptr;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    void mouseDown(const SDL_MouseButtonEvent& mouseButton) {
        mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? true : mouse.left;
        mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? true : mouse.right;
        mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? true : mouse.middle;

        if(mouseButton.button == SDL_BUTTON_LEFT) {
            chessLogic.SelectPiece(GetMousePiece());
            menuLogic.PressLeft();
        }
        if(mouseButton.button == SDL_BUTTON_RIGHT) {
            chessLogic.DeselectPiece();
            menuLogic.PressRight();
        }
    }
    void mouseUp(const SDL_MouseButtonEvent& mouseButton) {
        mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? false : mouse.left;
        mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? false : mouse.right;
        mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? false : mouse.middle;

        if(mouseButton.button == SDL_BUTTON_LEFT) {
            chessLogic.CheckMouseDeselect(GetMousePiece());
            menuLogic.ReleaseLeft();
        }
        if(mouseButton.button == SDL_BUTTON_RIGHT) {
            menuLogic.ReleaseRight();
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
        textRender.Init(renderer);
    }
public:
    inline static constexpr i32Vec2 menuSize = {100, 640};
    inline static constexpr i32Vec2 boardSize = {640, 640};
    inline static constexpr i32Vec2 boardPosition = {0,0};
    inline static constexpr i32Vec2 menuPosition = {boardSize.x,0};
    inline static constexpr i32 squareSize = boardSize.x/8;
    inline static constexpr i32Vec2 windowSize = {std::max(menuPosition.x+menuSize.x, boardPosition.x+boardSize.x),
    	std::max(menuPosition.y+menuSize.y, boardPosition.y+boardSize.y)};

    struct Mouse {
        i32Vec2 pos;
        bool left;
        bool right;
        bool middle;
    };
    inline static Mouse mouse ={{0,0},false,false,false};
    inline static i32Vec2 GetMouseRelative(i32Vec2 position) {
        return {mouse.pos.x - position.x, mouse.pos.y - position.y};
    }
    ChessBase::XY GetMousePiece() {
        const auto relative = Game::GetMouseRelative(Game::boardPosition);
        return {static_cast<i8>(relative.x/Game::squareSize),
            static_cast<i8>((chessRenderer.IsBoardFlipped() ? relative.y : Game::boardSize.y - relative.y)/Game::squareSize)};
    }

    TextRender textRender;

    static Game* GetCurrentGame() {
        return gameInstance;
    }

    Game() {
        gameInstance = this;
        initWindow();
        chessLogic.Init();
        menuLogic.Init();
        chessRenderer.Init(boardSize);
        menuRenderer.Init(menuSize);
        overlayRenderer.Init(windowSize);
        chessLogic.SetTurnChangeCallback([&]() {
            if(chessRenderer.FlipBoardOnTurn()) {
                chessRenderer.FlipBoard();
            }
        });
    }
    ChessLogic* GetChessLogic() {
        return &chessLogic;
    }
	MenuLogic* GetMenuLogic() {
        return &menuLogic;
    }
    SDL_Window* GetWindow() {
        return window;
    }
    SDL_Renderer* GetRenderer() {
        return renderer;
    }
    void SetOverlayRenderContext() {
        overlayRenderer.SetCurrentContext();
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
            menuRenderer.Render(renderer, menuPosition, menuSize);
            overlayRenderer.Render(renderer, {0,0}, windowSize);
            SDL_RenderPresent(renderer);
        }

	}
    ~Game() {
        SDL_Quit();
    }
};
