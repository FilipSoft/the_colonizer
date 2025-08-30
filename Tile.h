//
// Created by felix on 8/30/25.
//

#ifndef THE_COLONIZER_TILE_H
#define THE_COLONIZER_TILE_H


#include <SDL.h>
#include "lib.h"

class Tile {
    protected:
        SDL_Texture *texture;
        SDL_Renderer *renderer;
        SDL_Rect rect;
        int x, y;

public:
    Tile(int x, int y, SDL_Renderer *renderer);
    void setTexture(const unsigned char *img,int img_len);
    void render();
    void setPosition(int x, int y) ;
    void moveX(int x);
    void moveY(int y);
};



#endif //THE_COLONIZER_TILE_H