#ifndef ROOMS_H
#define ROOMS_H

#define MAX_ROOM_DESC 256
#define MAX_ROOM_ITEMS 5

typedef struct {
    char description[MAX_ROOM_DESC];
    int up;
    int down;
    int left;
    int right;

    
    char items[MAX_ROOM_ITEMS][64];
    int item_count;

    
    int creature_index;
} Room;

int load_rooms(const char* filename, Room** rooms, int* count);
void free_rooms(Room* rooms, int count);

#endif
