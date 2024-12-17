#ifndef PLAYER_H
#define PLAYER_H

#define MAX_INVENTORY 10
#define MAX_ITEM_NAME_LEN 64

typedef struct {
    int health;
    int strength;
    char inventory[MAX_INVENTORY][MAX_ITEM_NAME_LEN];
    int inventory_count;
} Player;

void init_player(Player* p);
int add_item_to_player(Player* p, const char* item_name);
int remove_item_from_player(Player* p, const char* item_name);
int player_has_item(Player* p, const char* item_name);

#endif
