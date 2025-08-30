//
// Created by felix on 8/30/25.
//
#include "Tile.h"
#include <SDL.h>
#include <SDL_image.h>


Tile::Tile(int x, int y, SDL_Renderer *renderer) {
    this->x = x;
    this->y = y;
    this->renderer = renderer;
}

void Tile::setTexture(const unsigned char *img,int img_len) {
    this->texture = loadTextureFromMem(renderer, img, img_len);
}

void Tile::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Tile::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}
