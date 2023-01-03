#include "lanplayer.h"

static size_t cbk(void *buffer, size_t size, size_t nmemb, void *stream) {
  	char **response_ptr = (char**)stream;
  	*response_ptr = strndup(buffer, (size_t)(size *nmemb));
}

char** read_servers_from_json(int* size) {
    *size = 0;
    FILE* server_file = fopen("servers.json", "r");
    char file_cnt[1000];    
    char file_ac_cnt[1000] = "";
    while(fgets(file_cnt, 1000, server_file) != NULL) {
        strcat(file_ac_cnt, file_cnt);
    }

    char** servers = malloc(sizeof(char*)*100);
    for(int i = 0; i < 100; i++) {
        servers[i] = malloc(sizeof(char)*50);
    }

    cJSON* arr = cJSON_Parse(file_ac_cnt);
    int n = cJSON_GetArraySize(arr);
    for(int i = 0; i < n; i++) {
        cJSON* e = cJSON_GetArrayItem(arr, i);
        *size += 1;
        strcpy(servers[i], e->valuestring);
    }
    return servers;
}

card_data read_server_data(char* server) {
    char server_t[strlen(server)*10];
    strcpy(server_t, "http://");
    strcat(server_t, server);
    strcat(server_t, "/info");
    char* response_string;
    CURL* c = curl_easy_init();
    curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(c, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(c, CURLOPT_URL, server_t);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, cbk);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &response_string);
    CURLcode curl_code = curl_easy_perform(c);

    long http_code = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &http_code);

    card_data d;

    printf("%ld", http_code);

    if (http_code != 200 || curl_code == CURLE_ABORTED_BY_CALLBACK)
    {
        return d;
    }

    if(response_string) {
        cJSON* j = cJSON_Parse(response_string);
        cJSON* o = cJSON_GetObjectItemCaseSensitive(j, "online");
        cJSON* i = cJSON_GetObjectItemCaseSensitive(j, "idle");

        d.online_players = o->valueint;
        d.idle_players = i->valueint;
        d.server = server;
    } else {}

    curl_easy_cleanup(c);

    return d;
}