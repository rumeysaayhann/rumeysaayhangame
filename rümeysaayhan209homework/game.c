#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

static Player player;
static Room* rooms = NULL;
static int room_count = 0;
static int current_room_index = 0;

int init_game(const char* rooms_file, const char* items_file, const char* creatures_file) {
    
    init_player(&player);

    
    if (load_rooms(rooms_file, &rooms, &room_count) != 0) {
        fprintf(stderr, "Error loading rooms.\n");
        return -1;
    }

   
    if (load_items(items_file) != 0) {
        fprintf(stderr, "Error loading items.\n");
        return -1;
    }

   
    if (load_creatures(creatures_file) != 0) {
        fprintf(stderr, "Error loading creatures.\n");
        return -1;
    }

    current_room_index = 0; 
    return 0;
}

void cleanup_game() {
    free_rooms(rooms, room_count);
    free_items();
    free_creatures();
}

int game_over() {
    return (player.health <= 0);
}

Player* get_player() {
    return &player;
}

Room* get_current_room() {
    if (current_room_index < 0 || current_room_index >= room_count) return NULL;
    return &rooms[current_room_index];
}


int get_current_room_index() {
    return current_room_index;
}


void set_current_room_index(int i) {
    if (i >= 0 && i < room_count) {
        current_room_index = i;
    }
}

int save_game(const char* filepath) {
    FILE* f = fopen(filepath, "w");
    if (!f) return -1;

    
    fprintf(f, "PLAYER\n");
    fprintf(f, "%d %d %d\n", get_player()->health, get_player()->strength, get_player()->inventory_count);
    for (int i = 0; i < get_player()->inventory_count; i++) {
        fprintf(f, "%s\n", get_player()->inventory[i]);
    }

    fprintf(f, "CURRENT_ROOM %d\n", current_room_index);

   

    fclose(f);
    return 0;
}

int load_game(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    if (!f) return -1;

    
    cleanup_game();
    if (init_game("data/rooms.txt", "data/items.txt", "data/creatures.txt") != 0) {
        fclose(f);
        return -1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        char* newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';

        if (strcmp(buffer, "PLAYER") == 0) {
            if (fscanf(f, "%d %d %d\n", &get_player()->health, &get_player()->strength, &get_player()->inventory_count) != 3) {
                fclose(f);
                return -1;
            }
            for (int i = 0; i < get_player()->inventory_count; i++) {
                if (!fgets(buffer, sizeof(buffer), f)) {
                    fclose(f);
                    return -1;
                }
                newline = strchr(buffer, '\n');
                if (newline) *newline = '\0';
                strncpy(get_player()->inventory[i], buffer, MAX_ITEM_NAME_LEN-1);
                get_player()->inventory[i][MAX_ITEM_NAME_LEN-1] = '\0';
            }
        } else if (strncmp(buffer, "CURRENT_ROOM", 12) == 0) {
            int idx;
            if (sscanf(buffer, "CURRENT_ROOM %d", &idx) == 1) {
                set_current_room_index(idx);
            }
        }
    }

    fclose(f);
    return 0;
}
