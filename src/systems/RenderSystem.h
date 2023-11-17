#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include <memory>
#include <algorithm>

class RenderSystem : public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            struct RenderableEntity {
                TransformComponent transformComponent;
                SpriteComponent spriteComponent;
            };

            std::vector<RenderableEntity> renderableEntities;
            for (auto entity : GetSystemEntities()) {
                RenderableEntity e;
                e.spriteComponent = entity.GetComponent<SpriteComponent>();
                e.transformComponent = entity.GetComponent<TransformComponent>();

                renderableEntities.emplace_back(e);
            }
            std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

            for (auto entity : renderableEntities) {
                const auto transform = entity.transformComponent;
                const auto sprite = entity.spriteComponent;

                SDL_Rect srcRect = sprite.srcRect; 
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y),
                };

                SDL_RenderCopyEx(
                    renderer, 
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                );
            }
        }
};

#endif