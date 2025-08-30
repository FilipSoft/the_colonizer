// src/main.cpp
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "car.h"
#include "soundtrack.h"

//create TExture object
SDL_Texture* loadTextureFromMem(SDL_Renderer* ren,
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


    if (SDL_Init(SDL_INIT_VIDEO) != 0) { std::cerr << SDL_GetError() << "\n"; return 1; }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) { std::cerr << IMG_GetError() << "\n"; return 1; }

    SDL_Window* w = SDL_CreateWindow("The_colonizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* tex = loadTextureFromMem(r, __img_car_png, __img_car_png_len);
    if (!tex) return 1;

    bool run = true; SDL_Event e; int x = 0;
    while (run) {
        while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) run = false;
        x = (x + 3) % 800;

        SDL_SetRenderDrawColor(r, 18,18,22,255); SDL_RenderClear(r);
        SDL_Rect dst{ x, 200, 256, 128 };
        SDL_RenderCopy(r, tex, nullptr, &dst);
        SDL_RenderPresent(r);
    }

    SDL_DestroyTexture(tex); SDL_DestroyRenderer(r); SDL_DestroyWindow(w);
    IMG_Quit(); SDL_Quit();
    return 0;
}
