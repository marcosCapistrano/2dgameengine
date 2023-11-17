#include "AssetStore.h"
#include <SDL2/SDL_image.h>
#include "../logger/Logger.h"

AssetStore::AssetStore() {

}

AssetStore::~AssetStore() {
    ClearAssets();
}


void AssetStore::ClearAssets() {
    for(auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }

    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        Logger::Err("could not load image: " + filePath);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface) {
        Logger::Err("could not create texture from surface: " + filePath);
    }
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);
}


SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures[assetId];
}