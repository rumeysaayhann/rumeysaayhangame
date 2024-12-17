#ifndef CREATURES_H
#define CREATURES_H

typedef struct {
    char name[64];
    int health;
    int strength;
} Creature;

int load_creatures(const char* filename);
void free_creatures();

Creature* get_creature(int index);

#endif
