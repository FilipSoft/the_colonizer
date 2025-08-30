// src/main.cpp
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "car.h"
#include "soundtrack.h"
#include "World.cpp"
//helping function
static void render_now(SDL_Renderer* ren) {
    SDL_SetRenderDrawColor(ren, 20,20,20,255);
    SDL_RenderClear(ren);
    // draw your scene...
    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[]) {
    SDL_SetMainReady();

    //initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Ask SDL_mixer to enable MP3
    int flags = MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags) {
        SDL_Log("Mix_Init: %s", Mix_GetError());
        return 1;
    }

    //Open audio channel
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        SDL_Log("Mix_OpenAudio: %s", Mix_GetError());
        return 1;
    }

    // Create an SDL_RWops that reads from the embedded memory
    SDL_RWops* rw = SDL_RWFromConstMem(sound_soundtrack_mp3, sound_soundtrack_mp3_len);
    if (!rw) {
        SDL_Log("SDL_RWFromConstMem: %s", SDL_GetError());
        return 1;
    }

    // Load music from RWops (freesrc=1 tells mixer to free the RWops when done)
    Mix_Music* mus = Mix_LoadMUS_RW(rw, 1);
    if (!mus) {
        SDL_Log("Mix_LoadMUS_RW: %s", Mix_GetError());
        return 1;
    }

    // Play once (loops = 1 means play 1 time; use -1 for infinite loop)
    if (Mix_PlayMusic(mus, -1) == -1) {
        SDL_Log("Mix_PlayMusic: %s", Mix_GetError());
        Mix_FreeMusic(mus);
        return 1;
    }


    //setting up graphics
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { std::cerr << SDL_GetError() << "\n"; return 1; }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) { std::cerr << IMG_GetError() << "\n"; return 1; }

    //creating window
    SDL_Window *window= SDL_CreateWindow("The_colonizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    //creating renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GL_SetSwapInterval(1);
    //create texture
    SDL_Texture* texture = loadTextureFromMem(renderer, __img_car_png, __img_car_png_len);
    if (!texture) return 1;

    //set fullscreen and virtual resolution
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    World* world = new World(20,20,renderer);

    bool running = true;
    SDL_Event e;
    int x = 0;

    while (running) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;  // quit on ESC
            }
            if (e.key.keysym.sym == SDLK_F11) {
                Uint32 flags = SDL_GetWindowFlags(window);
                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    SDL_SetWindowFullscreen(window, 0);
                } else {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
        }
        if (e.type == SDL_WINDOWEVENT) {
            switch (e.window.event) {
                case SDL_WINDOWEVENT_EXPOSED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                    // Recompute layout, viewport, etc., then redraw once
                    render_now(renderer);
                case SDL_WINDOWEVENT_MOVED:
                    // Optional: redraw after move to avoid stale blits
                    render_now(renderer);
            }
        }

        x = (x + 3) % SCREEN_WIDTH;

        world->renderMAp();

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 18,18,22,255);
        SDL_Rect dst{ x, 200, 256, 128 };

        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_RenderPresent(renderer);
    }


    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); SDL_Quit();
    return 0;
}
