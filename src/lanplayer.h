#ifndef LANPLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include "winstr.h"
#endif

typedef struct {
    char* server;
    int online_players;
    int idle_players;
} card_data;

char** read_servers_from_json(int* size);

card_data read_server_data(char* server);

#define LANPLAYER_H_
#endif