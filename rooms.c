#include "rooms.h"
#include "global.h"


char room1[ROOM_HEIGHT][ROOM_WIDTH] = {
    { 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 't', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't'}
};
char room2[ROOM_HEIGHT][ROOM_WIDTH] = {
    { 't', 't', 't', 't', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 't', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 'g', 't', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't'},
    { 't', 't', 't', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't'},
    { 'g', 'g', 'g', 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't'},
    { 'g', 'g', 'g', 'g', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't'}
};

// Read the .csv files and construct the array of rooms 
Room *rooms_init() {

    // Right now its single array of rooms
    Room *rooms = (Room *)malloc( sizeof(Room) * 2);

    memcpy( 
        &rooms[0].tiles[0][0], 
        &room2[0][0], 
        ROOM_HEIGHT * ROOM_WIDTH * sizeof(room1[0][0])
    );
    memcpy(
        &rooms[1].tiles[0][0],
        &room1[0][0],
        ROOM_HEIGHT * ROOM_WIDTH * sizeof(room2[0][0])
    );

    printf("First character is %c\n", rooms[0].tiles[0][0]);
    printf("First character is %c\n", rooms[1].tiles[0][0]);

    for (int r = 0; r < 2; r++) {
        for (int y = 0; y < ROOM_HEIGHT; y++) {
            for (int x = 0; x < ROOM_WIDTH; x++) {
                printf("%c,", rooms[r].tiles[y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }

    return rooms;
}

void rooms_free(Room *rooms) {
    free(rooms);
}

void rooms_render(Room *rooms) {
    
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < ROOM_WIDTH; x++) {
            DrawRectangleRec(
                (Rectangle){ x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE},
                YELLOW
            );
        }
    }

    // Render the first layer of tiles
for (int r = 0; r < 2; r++) {
    for (int y = 0; y < ROOM_HEIGHT; y++) {
        for (int x = 0; x < ROOM_WIDTH; x++) {

				DrawTextureRec(
					overworld,
					(Rectangle){ 7 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE},
					(Vector2){
						x * TILE_SIZE,
						(3 + y + r*ROOM_HEIGHT) * TILE_SIZE 
					},
					WHITE
				);

			if ( rooms[r].tiles[y][x] == 't') {
				DrawTextureRec(
					overworld,
					(Rectangle){ 0 * TILE_SIZE, 10 * TILE_SIZE, TILE_SIZE, TILE_SIZE},
					(Vector2){
						x * TILE_SIZE,
						(3 + y + r*ROOM_HEIGHT) * TILE_SIZE
					},
					WHITE
				);
			} 

        }
    }
}   // END OF r LOOP

}
