#include "Game.h"
#include "../logger/Logger.h"
#include "../ecs/ECS.h"
#include "../systems/MovementSystem.h"
#include "../systems/AnimationSystem.h"
#include "../systems/RenderSystem.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>

Game::Game() { 
  isRunning = false; 
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  millisecsPrevFrame = SDL_GetTicks();

  Logger::Log("Game constructor called");
}

Game::~Game() {
  Logger::Log("Game destructor called");
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("Error initializing SDL.");
    return;
  }

  // SDL_DisplayMode displayMode;
  // SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = 800;
  windowHeight = 600;

  window = SDL_CreateWindow("2D Game Engine", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                            SDL_WINDOW_BORDERLESS);

  if (!window) {
    Logger::Err("Error creating SDL window.");
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    Logger::Err("Error creating SDL renderer.");
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  isRunning = true;
}

void Game::Setup() {
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();
  registry->AddSystem<AnimationSystem>();

  assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
  assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

  // Load the tilemap
  int tileSize = 32;
  double tileScale = 2.0;
  int mapNumCols = 25;
  int mapNumRows = 20;

  std::fstream mapFile;
  mapFile.open("./assets/tilemaps/jungle.map");

  for (int y = 0; y < mapNumRows; y++) {
    for(int x = 0; x < mapNumCols; x++) {
      char ch;

      mapFile.get(ch);
      int srcRectY = std::atoi(&ch) * tileSize;

      mapFile.get(ch);
      int srcRectX = std::atoi(&ch) * tileSize;

      mapFile.ignore();

      Entity tile = registry->CreateEntity();
      tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale*tileSize), y * (tileScale*tileSize)), glm::vec2(tileScale, tileScale), 0.0);
      tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
    }
  }
  mapFile.close();

  Entity tank = registry->CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(3.0, 3.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 25.0));
  tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);

  Entity helicopter = registry->CreateEntity();
  helicopter.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(3.0, 3.0), 0.0);
  helicopter.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 25.0));
  helicopter.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2);
  helicopter.AddComponent<AnimationComponent>(2, 5, true);
}

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;

  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
      case SDL_QUIT:
        isRunning = false;
        break;

      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        break;
    }
  }
}

void Game::Update() {
  // waste some time until we reach MILLISECS_PER_FRAME
  int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPrevFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  double deltaTime = (SDL_GetTicks() - millisecsPrevFrame) / 1000.0;

  millisecsPrevFrame = SDL_GetTicks();

  registry->Update();
  registry->GetSystem<MovementSystem>().Update(deltaTime);
  registry->GetSystem<AnimationSystem>().Update(deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);

  registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
