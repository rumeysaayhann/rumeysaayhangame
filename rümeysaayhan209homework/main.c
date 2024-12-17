#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "commands.h"

int main() {
    
    if (init_game("data/rooms.txt", "data/items.txt", "data/creatures.txt") != 0) {
        fprintf(stderr, "Failed to initialize game.\n");
        return 1;
    }

    printf("Welcome to the Dungeon!\n");
    printf("Type 'help' for a list of commands.\n");

    char line[256];

    
    while (!game_over()) {
        printf("dungeon> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; 
        }

        
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        handle_command(line);
    }

    cleanup_game();
    return 0;
}
