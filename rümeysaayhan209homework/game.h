#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "rooms.h"
#include "creatures.h"
#include "items.h"

int init_game(const char* rooms_file, const char* items_file, const char* creatures_file);
void cleanup_game();
int game_over();

Player* get_player();
Room* get_current_room();


int get_current_room_index();
void set_current_room_index(int i);

int save_game(const char* filepath);
int load_game(const char* filepath);

#endif
