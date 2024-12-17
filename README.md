# Dungeon Game

This is a simple text-based dungeon exploration game. You can move through rooms, pick up items, fight creatures, and save/load your progress.

## Gameplay

Supported commands:

- `move <direction>`: Move to another room (directions: up, down, left, right).
- `look`: Describe the current room, items, and creature.
- `inventory`: Show items you are carrying.
- `pickup <item>`: Pick up an item in the current room.
- `attack`: Attack the creature in the room (if any).
- `list`: List saved games (dummy function).
- `save <filepath>`: Save the current game state.
- `load <filepath>`: Load a saved game state.
- `exit`: Quit the game.

## Compilation

```bash
make
```
