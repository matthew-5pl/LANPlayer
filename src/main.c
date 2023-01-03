#define SDL_MAIN_HANDLED
#include "lanplayer.h"
#include <pthread.h>

void* thread(void* vargp) {
    system((char*)(vargp));
    return NULL;
}

int main(int argc, char** argv) {
    int s = 0;
    char** f = read_servers_from_json(&s);
    card_data datas[s];
    printf("fetching servers, please wait....\n");
    for(int i = 0; i < s; i++) {
        datas[i] = read_server_data(f[i]);
        printf("%s OK...", datas[i].server);
    }

    SDL_Window* w = SDL_CreateWindow("LANPlayer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 420, 0);
    SDL_Renderer* r = SDL_CreateRenderer(w, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Event ev;
    

    TTF_Init();

    TTF_Font* font = TTF_OpenFont("font.ttf", 25);

    int done = 0;

    SDL_Color black = {0,0,0};

    int x = 10, y = 10;

    SDL_Rect rects[s];
    SDL_Rect mouse;
    SDL_Rect ra;

    int showMenu = 1;

    for(int i = 0; i < s; i++) {
        char* txt = malloc(sizeof(char)*200);
        sprintf(txt, "Server: %s Online: %d Idle: %d", datas[i].server, datas[i].online_players, datas[i].idle_players);
        SDL_Surface* s = TTF_RenderText_Solid(font, txt, black);
        SDL_Rect re = {x, y, s->w, s->h};
        rects[i] = re;
        y += 50;
        free(txt);
    }

    char sys[1000];

    while(!done) {
        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(ev.button.button == SDL_BUTTON_LEFT) {
                        for(int i = 0; i < s; i++) {
                            if(SDL_HasIntersection(&rects[i], &mouse)) {
                                ra = rects[i];
                                sprintf(sys, "lan.exe --relay-server-addr %s", datas[i].server);
                                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(SDL_HasIntersection(&ra, &mouse) && ev.button.button == SDL_BUTTON_LEFT) {
                        if(!showMenu) {
                            exit(0);
                        }
                        ra.x = -100;
                        ra.y = -100;
                        pthread_t thread_id;
                        pthread_create(&thread_id, NULL, thread, sys);
                        showMenu = 0;
                    }
                    break;
            }
        }
        
        int mx = 0, my = 0;

        SDL_GetMouseState(&mx, &my);

        mouse = (SDL_Rect){mx, my, 10, 10};

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

        if(showMenu) {
            for(int i = 0; i < s; i++) {
                char* txt = malloc(sizeof(char)*200);
                sprintf(txt, "Server: %s Online: %d Idle: %d", datas[i].server, datas[i].online_players, datas[i].idle_players);
                SDL_Surface* s = TTF_RenderText_Solid(font, txt, black);
                SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
                SDL_FreeSurface(s);
                SDL_RenderCopy(r, t, NULL, &rects[i]);
                SDL_DestroyTexture(t);
                free(txt);
            }
        }

        else {
            SDL_SetWindowSize(w, 700, 80);
            SDL_Surface* s = TTF_RenderText_Solid(font, "Connected. Close window or click here to disconnect", black);
            SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
            SDL_FreeSurface(s);
            SDL_RenderCopy(r, t, NULL, &(SDL_Rect){10, 10, s->w, s->h});
            SDL_DestroyTexture(t);
        }

        SDL_SetRenderDrawColor(r, 36, 84, 125, 255);
        SDL_RenderDrawRect(r, &ra);

        SDL_RenderPresent(r);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}