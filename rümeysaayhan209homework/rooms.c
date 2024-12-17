#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rooms.h"

int load_rooms(const char* filename, Room** rooms, int* count) {
    

    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    int num_rooms;
    if (fscanf(f, "%d\n", &num_rooms) != 1) {
        fclose(f);
        return -1;
    }

    Room* r = malloc(sizeof(Room)*num_rooms);
    if (!r) {
        fclose(f);
        return -1;
    }

    for (int i = 0; i < num_rooms; i++) {
        if (!fgets(r[i].description, MAX_ROOM_DESC, f)) {
            fclose(f);
            free(r);
            return -1;
        }
        
        char* nl = strchr(r[i].description, '\n');
        if (nl) *nl = '\0';

        if (fscanf(f, "%d %d %d %d\n", &r[i].up, &r[i].down, &r[i].left, &r[i].right) != 4) {
            fclose(f);
            free(r);
            return -1;
        }

        if (fscanf(f, "%d\n", &r[i].item_count) != 1) {
            fclose(f);
            free(r);
            return -1;
        }
        for (int j = 0; j < r[i].item_count; j++) {
            if (!fgets(r[i].items[j], 64, f)) {
                fclose(f);
                free(r);
                return -1;
            }
            nl = strchr(r[i].items[j], '\n');
            if (nl) *nl = '\0';
        }

        if (fscanf(f, "%d\n", &r[i].creature_index) != 1) {
            fclose(f);
            free(r);
            return -1;
        }
    }

    fclose(f);
    *rooms = r;
    *count = num_rooms;
    return 0;
}

void free_rooms(Room* rooms, int count) {
    (void)count; 
    if (rooms) free(rooms);
}
