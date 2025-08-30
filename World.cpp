//
// Created by felix on 8/30/25.
//
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL_render.h>
#include "grass.h"
#include "lib.h"
#include "Tile.h"

class World {
    static int randomNumber(int limit) {
    // Get a different random number each time the program runs
    srand(time(nullptr));

    // Generate a random number between 0 and 100
    int randomNum = rand() % limit + 1;

    return randomNum;
}

public:
    int map_width() const {
        return mapWidth;
    }

    int map_height() const {
        return mapHeight;
    }

    World(int mapWidth, int mapHeight, SDL_Renderer *renderer) {
        this->mapWidth = mapWidth;
        this->mapHeight = mapHeight;
        this->renderer = renderer;
        createMap();
    }

    int mapWidth, mapHeight;

    SDL_Renderer *renderer;
    int rows = mapHeight/TILE_HEIGHT, cols = mapWidth/TILE_WIDTH;
    Tile *map[0][0] ;

    void createMap() {

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                map[row][col] = new Tile(col * TILE_WIDTH, row * TILE_HEIGHT, renderer);
                map[row][col]->setTexture(__img_grass_png, __img_grass_png_len);
            }
        }
    }

    void renderMAp() {
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                map[row][col]->render();
            }
        }
    }
};


