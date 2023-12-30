#pragma once

#include "engine.h"

#define MAP_WIDTH 9
#define MAP_HEIGHT 15
#define ROOM_WIDTH 16
#define ROOM_HEIGHT 11

#define MAX_NODES 8
#define TILE_SIZE 16
#define ZOOM 1.0f
#define HALF_TILE_SIZE 8

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

//
// TYPES
//
typedef enum Direction {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
} Direction;

typedef struct Player {
    Entity      e;
    float       speed;
    Direction   dir;
    bool        is_attacking;
    Timer       cooldown_timer; // Cooldown Timer for attacking
    Rectangle   coll_box;       // Player Collision Box
    Rectangle   wp_coll_box;    // Weapon Collision Box
    Vector2     center;         // @TODO @TEMP
    Vector2     curr_tile;      // @TODO @TEMP
} Player;

typedef struct Enemy {
    Entity      e;
    Direction   dir;
    //unsigned char hp;
    int         hp;
    Rectangle   coll_box;
} Enemy;

typedef struct Tile {
    //TileType type;
    Vector2     center_position;
} Tile;

typedef struct Room {
	char        tiles[ROOM_HEIGHT][ROOM_WIDTH];
	int         objects[ROOM_HEIGHT][ROOM_WIDTH];
    Rectangle   exits[4];
} Room;


//
// GLOBALS
//

extern int cam_offset_y;
extern int cam_offset_x;

extern int window_height;
extern int window_width;

extern int game_width; // 640 before
extern int game_height;  // 360 before

extern float player_game_speed;

extern Camera2D camera;
extern Vector2 ui_center;
extern Player player;
extern Texture2D overworld;
extern Room *rooms;
extern int wow;

extern Timer camera_cooldown;
extern bool camera_is_moving;
extern Rectangle room_exit_coll_box;
extern Vector2 camera_transition_dest; 


