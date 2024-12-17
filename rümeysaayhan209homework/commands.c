#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "game.h"
#include "player.h"
#include "rooms.h"
#include "creatures.h"

static void cmd_help() {
    printf("Available commands:\n");
    printf("  move <direction>\n");
    printf("  look\n");
    printf("  inventory\n");
    printf("  pickup <item>\n");
    printf("  attack\n");
    printf("  list\n");
    printf("  save <filepath>\n");
    printf("  load <filepath>\n");
    printf("  exit\n");
}

static void cmd_move(char* dir) {
    Room* r = get_current_room();
    if (!r) {
        printf("You are nowhere!\n");
        return;
    }

    int next = -1;
    if (strcmp(dir, "up") == 0) next = r->up;
    else if (strcmp(dir, "down") == 0) next = r->down;
    else if (strcmp(dir, "left") == 0) next = r->left;
    else if (strcmp(dir, "right") == 0) next = r->right;
    else {
        printf("Invalid direction.\n");
        return;
    }

    if (next < 0) {
        printf("You can't go that way.\n");
        return;
    }

    set_current_room_index(next);
    printf("You move %s.\n", dir);
}

static void cmd_look() {
    Room* r = get_current_room();
    if (!r) {
        printf("You see nothing...\n");
        return;
    }
    printf("Room: %s\n", r->description);
    if (r->item_count > 0) {
        printf("Items here:\n");
        for (int i = 0; i < r->item_count; i++) {
            printf(" - %s\n", r->items[i]);
        }
    }
    if (r->creature_index >= 0) {
        Creature* c = get_creature(r->creature_index);
        if (c && c->health > 0) {
            printf("A %s is here, looking dangerous.\n", c->name);
        }
    }
}

static void cmd_inventory() {
    Player* p = get_player();
    if (p->inventory_count == 0) {
        printf("Your inventory is empty.\n");
        return;
    }
    printf("You carry:\n");
    for (int i = 0; i < p->inventory_count; i++) {
        printf(" - %s\n", p->inventory[i]);
    }
}

static void cmd_pickup(char* item_name) {
    Room* r = get_current_room();
    Player* p = get_player();
    if (!r || !item_name) return;

    int found = -1;
    for (int i = 0; i < r->item_count; i++) {
        if (strcmp(r->items[i], item_name) == 0) {
            found = i;
            break;
        }
    }
    if (found < 0) {
        printf("No such item here.\n");
        return;
    }

    if (add_item_to_player(p, item_name) != 0) {
        printf("Your inventory is full.\n");
        return;
    }

    for (int j = found; j < r->item_count - 1; j++) {
        strcpy(r->items[j], r->items[j+1]);
    }
    r->item_count--;

    printf("You picked up %s.\n", item_name);
}

static void cmd_attack() {
    Room* r = get_current_room();
    Player* p = get_player();
    if (!r || r->creature_index < 0) {
        printf("There's nothing to attack here.\n");
        return;
    }

    Creature* c = get_creature(r->creature_index);
    if (!c || c->health <= 0) {
        printf("There's nothing to attack here.\n");
        return;
    }

   
    while (p->health > 0 && c->health > 0) {
        
        c->health -= p->strength;
        if (c->health <= 0) {
            printf("You defeated the %s!\n", c->name);
            break;
        }
        
        p->health -= c->strength;
        if (p->health <= 0) {
            printf("The %s defeated you...\n", c->name);
            break;
        }
    }
}

static void cmd_list() {
 
    printf("Saved games:\n");
    printf("  - save1.dat\n");
    printf("  - save2.dat\n");
}

static void cmd_save(char* filepath) {
    if (!filepath) {
        printf("Usage: save <filepath>\n");
        return;
    }
    if (save_game(filepath) == 0) {
        printf("Game saved to %s\n", filepath);
    } else {
        printf("Failed to save game.\n");
    }
}

static void cmd_load(char* filepath) {
    if (!filepath) {
        printf("Usage: load <filepath>\n");
        return;
    }
    if (load_game(filepath) == 0) {
        printf("Game loaded from %s\n", filepath);
    } else {
        printf("Failed to load game.\n");
    }
}

void handle_command(const char* line) {
    if (strcmp(line, "help") == 0) {
        cmd_help();
        return;
    }
    if (strcmp(line, "look") == 0) {
        cmd_look();
        return;
    }
    if (strcmp(line, "inventory") == 0) {
        cmd_inventory();
        return;
    }
    if (strcmp(line, "attack") == 0) {
        cmd_attack();
        return;
    }
    if (strcmp(line, "list") == 0) {
        cmd_list();
        return;
    }
    if (strcmp(line, "exit") == 0) {
        get_player()->health = 0; 
        return;
    }

    
    char cmd[64], arg[128];
    if (sscanf(line, "%63s %127[^\n]", cmd, arg) == 2) {
        if (strcmp(cmd, "move") == 0) {
            cmd_move(arg);
            return;
        } else if (strcmp(cmd, "pickup") == 0) {
            cmd_pickup(arg);
            return;
        } else if (strcmp(cmd, "save") == 0) {
            cmd_save(arg);
            return;
        } else if (strcmp(cmd, "load") == 0) {
            cmd_load(arg);
            return;
        }
    }

    printf("Unknown command. Type 'help' to see available commands.\n");
}
