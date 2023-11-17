#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ecs/ECS.h"
#include "../components/AnimationComponent.h"
#include "../components/SpriteComponent.h"
#include "../logger/Logger.h"

class AnimationSystem : public System {
    public:
        AnimationSystem() {
            RequireComponent<AnimationComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(double deltaTime) {
            for (auto entity : GetSystemEntities()) {
                auto& animation = entity.GetComponent<AnimationComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) 
                * animation.frameSpeedRate / 1000) % animation.numFrames;

                    Logger::Log("currentFrame: " + std::to_string(animation.currentFrame));
                    Logger::Log("currentFrame: " + std::to_string(animation.currentFrame));

                sprite.srcRect.x = animation.currentFrame * sprite.width;
            }
        }
};

#endif