#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

//
// Created by felix on 8/30/25.
//

#ifndef THE_COLONIZER_CONSTANTS_H
#define THE_COLONIZER_CONSTANTS_H
static inline constexpr int SCREEN_WIDTH = 1024, SCREEN_HEIGHT = 576, TILE_WIDTH = 64, TILE_HEIGHT = 64;

//create Texture object
inline SDL_Texture* loadTextureFromMem(SDL_Renderer* ren,
                                       const unsigned char* data,
                                       std::size_t len) {
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(len));
    if (!rw) { std::cerr << "SDL_RWFromConstMem failed\n"; return nullptr; }

    SDL_Surface* surf = IMG_Load_RW(rw, 1); // frees rw
    if (!surf) { std::cerr << "IMG_Load_RW: " << IMG_GetError() << "\n"; return nullptr; }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    return tex;
}
#endif //THE_COLONIZER_CONSTANTS_H