#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct Definitions
typedef struct Item {
    char* name;
    char* description;
    int weight;
} Item;

typedef struct Creature {
    char* name;
    int health;
    int strength;
} Creature;

typedef struct Room {
    char* description;
    struct Room* connections[4]; // 0: up, 1: right, 2: down, 3: left
    Item* items[10]; // Maximum 10 items per room
    Creature* creature; // Creature in the room, if any
} Room;

typedef struct Player {
    int health;
    int strength;
    int inventoryCapacity;
    Item* inventory[5]; // Maximum 5 items in inventory
    Room* currentRoom;
} Player;

// Global Room Definitions
Room room1, room2, room3, room4, room5;

// Function Prototypes
void move(Player* player, int direction);
void look(Player* player);
void pickup(Player* player, const char* itemName);
void attack(Player* player);
void saveGame(Player* player, const char* filepath);
void loadGame(Player* player, const char* filepath);
void restartGame(Player* player);
void resetRooms();
void displayRoom(Room* room);
void displayInventory(Player* player);
Item* findItemInRoom(Room* room, const char* itemName);
void addItemToInventory(Player* player, Item* item);
Creature* createCreature(const char* name, int health, int strength);
void freeCreature(Creature* creature);

// Main Function
int main() {
    // Initialization
    srand(time(NULL)); // Seed random number generator

    // Sample Items
    Item sword = { "Sword", "A sharp blade", 3 };
    Item shield = { "Shield", "A sturdy shield", 5 };
    Item potion = { "Potion", "A health-restoring potion", 2 };
    Item key = { "Key", "A mysterious golden key", 1 };
    Item treasure = { "Treasure", "A chest filled with gold", 10 };

    // Sample Creatures
    Creature goblin = { "Goblin", 30, 5 };
    Creature dragon = { "Dragon", 100, 20 };

    // Sample Rooms
    room1 = (Room){ "You are in a dark, damp room with stone walls.", {NULL, NULL, NULL, NULL}, {&sword, NULL}, &goblin };
    room2 = (Room){ "You are in a bright room with a glowing crystal in the center.", {NULL, NULL, NULL, NULL}, {&potion, NULL}, NULL };
    room3 = (Room){ "You are in a narrow corridor with flickering torches on the walls.", {NULL, NULL, NULL, NULL}, {&shield, NULL}, NULL };
    room4 = (Room){ "You are in a treasure chamber filled with gold and jewels.", {NULL, NULL, NULL, NULL}, {&treasure, NULL}, NULL };
    room5 = (Room){ "You are in a dragon's lair, the air thick with smoke.", {NULL, NULL, NULL, NULL}, {&key, NULL}, &dragon };

    // Connect Rooms
    room1.connections[1] = &room2; // Room 1 right to Room 2
    room2.connections[3] = &room1; // Room 2 left to Room 1
    room2.connections[2] = &room3; // Room 2 down to Room 3
    room3.connections[0] = &room2; // Room 3 up to Room 2
    room3.connections[2] = &room4; // Room 3 down to Room 4
    room4.connections[0] = &room3; // Room 4 up to Room 3
    room4.connections[1] = &room5; // Room 4 right to Room 5
    room5.connections[3] = &room4; // Room 5 left to Room 4

    // Player Initialization
    Player player = { 100, 10, 5, {NULL}, &room1 };

    // Game Loop
    char command[100];
    printf("Welcome to Dungeon Explorer!\n");
    while (1) {
        printf(">>> ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;

        char* action = strtok(command, " \n");
        if (strcmp(action, "move") == 0) {
            char* directionStr = strtok(NULL, " \n");
            if (directionStr) {
                int direction = atoi(directionStr);
                move(&player, direction);
            }
            else {
                printf("Usage: move <direction>\n");
            }
        }
        else if (strcmp(action, "look") == 0) {
            look(&player);
        }
        else if (strcmp(action, "pickup") == 0) {
            char* itemName = strtok(NULL, " \n");
            if (itemName) {
                pickup(&player, itemName);
            }
            else {
                printf("Usage: pickup <item name>\n");
            }
        }
        else if (strcmp(action, "inventory") == 0) {
            displayInventory(&player);
        }
        else if (strcmp(action, "attack") == 0) {
            attack(&player);
        }
        else if (strcmp(action, "save") == 0) {
            char* filepath = strtok(NULL, " \n");
            if (filepath) {
                saveGame(&player, filepath);
            }
            else {
                printf("Usage: save <filepath>\n");
            }
        }
        else if (strcmp(action, "load") == 0) {
            char* filepath = strtok(NULL, " \n");
            if (filepath) {
                loadGame(&player, filepath);
            }
            else {
                printf("Usage: load <filepath>\n");
            }
        }
        else if (strcmp(action, "exit") == 0) {
            printf("Exiting the game...\n");
            break;
        }
        else {
            printf("Unknown command!\n");
        }
    }

    return 0;
}

// Function Definitions
void move(Player* player, int direction) {
    if (direction < 0 || direction > 3) {
        printf("Invalid direction! Use 0 (up), 1 (right), 2 (down), or 3 (left).\n");
        return;
    }

    Room* nextRoom = player->currentRoom->connections[direction];
    if (nextRoom) {
        player->currentRoom = nextRoom;
        printf("You move to a new room.\n");
        look(player);
    }
    else {
        printf("You cannot move in that direction.\n");
    }
}

void look(Player* player) {
    displayRoom(player->currentRoom);
}

void pickup(Player* player, const char* itemName) {
    Room* currentRoom = player->currentRoom;
    Item* item = findItemInRoom(currentRoom, itemName);

    if (item) {
        addItemToInventory(player, item);
        printf("You picked up the %s.\n", item->name);

        // Remove item from the room
        for (int i = 0; i < 10; i++) {
            if (currentRoom->items[i] == item) {
                currentRoom->items[i] = NULL;
                break;
            }
        }
    }
    else {
        printf("There is no %s here.\n", itemName);
    }
}

void attack(Player* player) {
    Creature* creature = player->currentRoom->creature;
    if (!creature) {
        printf("There is nothing to attack here!\n");
        return;
    }

    printf("You engage in combat with the %s!\n", creature->name);
    printf("You attack the %s for %d damage!\n", creature->name, player->strength);
    creature->health -= player->strength;

    if (creature->health <= 0) {
        printf("You defeated the %s!\n", creature->name);
        player->currentRoom->creature = NULL;
        freeCreature(creature);
    }
    else {
        printf("The %s is still alive but weakened!\n", creature->name);
    }
}

void saveGame(Player* player, const char* filepath) {
    FILE* file = fopen(filepath, "w");
    if (!file) {
        printf("Failed to save the game to %s.\n", filepath);
        return;
    }

    fprintf(file, "%d %d %d\n", player->health, player->strength, player->inventoryCapacity);
    fprintf(file, "%s\n", player->currentRoom->description);

    for (int i = 0; i < 5; i++) {
        if (player->inventory[i]) {
            fprintf(file, "%s\n", player->inventory[i]->name);
        }
        else {
            fprintf(file, "(null)\n");
        }
    }

    fclose(file);
    printf("Game saved successfully to %s.\n", filepath);
}

void loadGame(Player* player, const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Failed to load the game from %s.\n", filepath);
        return;
    }

    fscanf(file, "%d %d %d\n", &player->health, &player->strength, &player->inventoryCapacity);
    char roomDescription[256];
    fgets(roomDescription, sizeof(roomDescription), file);

    // Update room description
    free(player->currentRoom->description);
    player->currentRoom->description = strdup(roomDescription);

    for (int i = 0; i < 5; i++) {
        char itemName[50];
        fgets(itemName, sizeof(itemName), file);
        if (strcmp(itemName, "(null)\n") != 0) {
            player->inventory[i] = malloc(sizeof(Item));
            player->inventory[i]->name = strdup(itemName);
        }
        else {
            player->inventory[i] = NULL;
        }
    }

    fclose(file);
    printf("Game loaded successfully from %s.\n", filepath);
}

void restartGame(Player* player) {
    printf("Resetting the game...\n");

    // Reset player stats
    player->health = 100;
    player->strength = 10;

    // Clear inventory
    for (int i = 0; i < 5; i++) {
        player->inventory[i] = NULL;
    }

    // Reset the starting room
    player->currentRoom = &room1;

    // Reset room states
    resetRooms();

    printf("Game restarted. You are back in the starting room.\n");
    look(player);
}

void resetRooms() {
    room1.creature = createCreature("Goblin", 30, 5);
    room5.creature = createCreature("Dragon", 100, 20);

    room1.items[0] = &(Item) { "Sword", "A sharp blade", 3 };
    room2.items[0] = &(Item) { "Potion", "A health-restoring potion", 2 };
    room3.items[0] = &(Item) { "Shield", "A sturdy shield", 5 };
    room4.items[0] = &(Item) { "Treasure", "A chest filled with gold", 10 };
    room5.items[0] = &(Item) { "Key", "A mysterious golden key", 1 };
}

void displayRoom(Room* room) {
    printf("%s\n", room->description);

    printf("Items in the room:\n");
    for (int i = 0; i < 10; i++) {
        if (room->items[i]) {
            printf("- %s: %s\n", room->items[i]->name, room->items[i]->description);
        }
    }

    if (room->creature) {
        printf("A %s is here!\n", room->creature->name);
    }
}

void displayInventory(Player* player) {
    printf("Your inventory:\n");
    for (int i = 0; i < 5; i++) {
        if (player->inventory[i]) {
            printf("- %s\n", player->inventory[i]->name);
        }
    }
}

Item* findItemInRoom(Room* room, const char* itemName) {
    for (int i = 0; i < 10; i++) {
        if (room->items[i] && strcmp(room->items[i]->name, itemName) == 0) {
            return room->items[i];
        }
    }
    return NULL;
}

void addItemToInventory(Player* player, Item* item) {
    for (int i = 0; i < 5; i++) {
        if (player->inventory[i] == NULL) {
            player->inventory[i] = item;
            break;
        }
    }
}

Creature* createCreature(const char* name, int health, int strength) {
    Creature* creature = malloc(sizeof(Creature));
    creature->name = strdup(name);
    creature->health = health;
    creature->strength = strength;
    return creature;
}

void freeCreature(Creature* creature) {
    if (creature) {
        free(creature->name);
        free(creature);
    }
}
