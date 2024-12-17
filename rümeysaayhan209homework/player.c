#include <string.h>
#include "player.h"

void init_player(Player* p) {
    p->health = 100;
    p->strength = 10;
    p->inventory_count = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        p->inventory[i][0] = '\0';
    }
}

int add_item_to_player(Player* p, const char* item_name) {
    if (p->inventory_count >= MAX_INVENTORY) return -1;
    strncpy(p->inventory[p->inventory_count], item_name, MAX_ITEM_NAME_LEN-1);
    p->inventory[p->inventory_count][MAX_ITEM_NAME_LEN-1] = '\0';
    p->inventory_count++;
    return 0;
}

int remove_item_from_player(Player* p, const char* item_name) {
    for (int i = 0; i < p->inventory_count; i++) {
        if (strcmp(p->inventory[i], item_name) == 0) {
            
            for (int j = i; j < p->inventory_count - 1; j++) {
                strcpy(p->inventory[j], p->inventory[j+1]);
            }
            p->inventory_count--;
            return 0;
        }
    }
    return -1;
}

int player_has_item(Player* p, const char* item_name) {
    for (int i = 0; i < p->inventory_count; i++) {
        if (strcmp(p->inventory[i], item_name) == 0) {
            return 1;
        }
    }
    return 0;
}
