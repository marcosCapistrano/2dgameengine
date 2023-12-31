#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SDL2/SDL.h>
#include "../ecs/ECS.h"
#include "../assetstore/AssetStore.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        int millisecsPrevFrame;
        SDL_Window *window;
        SDL_Renderer *renderer;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;

    public:
        Game();
        ~Game();
        void Initialize();
        void Setup();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        int windowWidth;
        int windowHeight;
};

#endif