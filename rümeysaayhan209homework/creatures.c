#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatures.h"

static Creature* creatures = NULL;
static int creature_count = 0;

int load_creatures(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    if (fscanf(f, "%d\n", &creature_count) != 1) {
        fclose(f);
        return -1;
    }

    creatures = malloc(sizeof(Creature)*creature_count);
    if (!creatures) {
        fclose(f);
        return -1;
    }

    for (int i = 0; i < creature_count; i++) {
        char buf[64];
        if (!fgets(buf, sizeof(buf), f)) {
            free(creatures);
            fclose(f);
            return -1;
        }
        char* nl = strchr(buf, '\n');
        if (nl) *nl = '\0';
        strncpy(creatures[i].name, buf, 63);
        creatures[i].name[63] = '\0';

        if (fscanf(f, "%d %d\n", &creatures[i].health, &creatures[i].strength) != 2) {
            free(creatures);
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    return 0;
}

void free_creatures() {
    if (creatures) free(creatures);
    creatures = NULL;
    creature_count = 0;
}

Creature* get_creature(int index) {
    if (index < 0 || index >= creature_count) return NULL;
    return &creatures[index];
}
