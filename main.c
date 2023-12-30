#include "engine.h"
#include "global.h"
#include "rooms.h"

//#define STB_DS_IMPLEMENTATION
//#include "stb_ds.h"

int cam_offset_y = 15;
int cam_offset_x = 9;

int window_height = 640;
int window_width = 450;

int game_width  = 16 * TILE_SIZE; // 640 before
int game_height = 14 * TILE_SIZE;  // 360 before

float player_game_speed = 200.0f;

Camera2D camera = { 0 };
Vector2 ui_center = { 0 };
Player player = { 0 };
Texture2D overworld = { 0 };
int wow = 0 ;

Timer camera_cooldown = { 0 };
bool camera_is_moving;
Rectangle room_exit_coll_box;
Vector2 camera_transition_dest; 
Room *rooms;

void load_textures() {
    overworld = LoadTexture("Overworld.png");
    wow = 1;
}

void load_sounds() {
    
}

RenderTexture2D target;
void startup() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "Legend");
    SetWindowMinSize(16*TILE_SIZE, 14*TILE_SIZE);

    // Render Texture Initialization, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(game_width, game_height);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    
    player.e.pos = (Vector2){ 4 * TILE_SIZE, 11 * TILE_SIZE};
    player.speed = 46.0f;

    //camera.target = player.e.pos;
    camera.offset = (Vector2){game_width / 2, game_height / 2};
    camera.target = (Vector2) {ROOM_WIDTH * TILE_SIZE / 2, (ROOM_HEIGHT+3) * TILE_SIZE / 2};
    camera.rotation = 0.0f;
    camera.zoom = ZOOM; // 3 works

    camera_cooldown.lifetime = 0;

    SetTargetFPS(30);
}

void player_update(Player *player, float delta_time) {
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
    }
    if (IsKeyPressed(KEY_SPACE)) {
        player->is_attacking = true;
        timer_start(&player->cooldown_timer, 0.2f);

        int x_off, y_off;
        if (player->dir == DIR_UP) {
            y_off = -1;
            x_off = 0;
        } else if (player->dir == DIR_DOWN) {
            y_off = 1;
            x_off = 0;
        } else if (player->dir == DIR_RIGHT) {
            y_off = 0;
            x_off = 1;
        } else if (player->dir == DIR_LEFT) {
            y_off = 0;
            x_off = -1;
        }

        player->wp_coll_box = (Rectangle) {
            player->e.pos.x + 16 * x_off, 
            player->e.pos.y + 16 * y_off, 
            TILE_SIZE / 2, 
            TILE_SIZE / 2
        };
    }
if (timer_is_done(&player->cooldown_timer) == false) {
        return;
    }

    float a = 1.0f;

    if (IsKeyDown(KEY_LEFT)) {
        player->e.pos.x -= player->speed * delta_time;
        player->dir = DIR_LEFT;

        /*
        if (player->e.pos.y < ((int)player->curr_tile.y)*TILE_SIZE - 1) {
            player->e.pos.y += player->speed / a * delta_time;
            //player->e.pos.y += 1;
        }
        else if (player->e.pos.y > ((int)player->curr_tile.y)*TILE_SIZE + 1) {
            player->e.pos.y -= player->speed / a * delta_time;
            //player->e.pos.y -= 1;
        }
        */
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        player->e.pos.x += player->speed * delta_time;
        player->dir = DIR_RIGHT;

/*
        if (player->e.pos.y < ((int)player->curr_tile.y)*TILE_SIZE - 1) {
            player->e.pos.y += player->speed / a * delta_time;
            //player->e.pos.y += 1;
        }
        else if (player->e.pos.y > ((int)player->curr_tile.y)*TILE_SIZE + 1) {
            player->e.pos.y -= player->speed / a * delta_time;
            //player->e.pos.y -= 1;
        }
        */
    }
    else if (IsKeyDown(KEY_UP)) {
        player->e.pos.y -= player->speed * delta_time;
        player->dir = DIR_UP;
/*
        if (player->e.pos.x < ((int)player->curr_tile.x)*TILE_SIZE - 1) {
            player->e.pos.x += player->speed / a * delta_time;
            //player->e.pos.x += 1;
        }
        else if (player->e.pos.x > ((int)player->curr_tile.x)*TILE_SIZE + 1) {
            player->e.pos.x -= player->speed / a * delta_time;
            //player->e.pos.x -= 1;
        }
        */
    }
    else if (IsKeyDown(KEY_DOWN)) {
        player->e.pos.y += player->speed * delta_time;
        player->dir = DIR_DOWN;
/*
        if (player->e.pos.x < ((int)player->curr_tile.x)*TILE_SIZE - 1) {
            player->e.pos.x += player->speed * delta_time;
            //player->e.pos.x += 1;
        }
        else if (player->e.pos.x > ((int)player->curr_tile.x)*TILE_SIZE + 1) {
            player->e.pos.x -= player->speed / a * delta_time;
            //player->e.pos.x -= 1;
        }
        */
    }

    if (IsKeyReleased(KEY_LEFT)) {
        if (player->e.pos.y < ((int)player->curr_tile.y)*TILE_SIZE - 1) {
            //player->e.pos.y += player->speed / a * delta_time;
            player->e.pos.y += 1;
        }
        else if (player->e.pos.y > ((int)player->curr_tile.y)*TILE_SIZE + 1) {
            //player->e.pos.y -= player->speed / a * delta_time;
            player->e.pos.y -= 1;
        }
    } else if (IsKeyReleased(KEY_RIGHT)) {
        if (player->e.pos.y < ((int)player->curr_tile.x)*TILE_SIZE - 1) {
            //player->e.pos.x += player->speed / a * delta_time;
            player->e.pos.y += 1;
        }
        else if (player->e.pos.x > ((int)player->curr_tile.x)*TILE_SIZE + 1) {
            //player->e.pos.x -= player->speed / a * delta_time;
            player->e.pos.x -= 1;
        }
    } else if (IsKeyReleased(KEY_UP)) {
        if (player->e.pos.x < ((int)player->curr_tile.x)*TILE_SIZE - 1) {
            //player->e.pos.x += player->speed / a * delta_time;
            player->e.pos.x += 1;
        }
        else if (player->e.pos.x > ((int)player->curr_tile.x)*TILE_SIZE + 1) {
            //player->e.pos.x -= player->speed / a * delta_time;
            player->e.pos.x -= 1;
        }
    } else if (IsKeyReleased(KEY_DOWN)) {
        if (player->e.pos.x < ((int)player->curr_tile.x)*TILE_SIZE - 1) {
            //player->e.pos.x += player->speed / a* delta_time;
            player->e.pos.x += 1;
        }
        else if (player->e.pos.x > ((int)player->curr_tile.x)*TILE_SIZE + 1) {
            //player->e.pos.x -= player->speed / a * delta_time;
            player->e.pos.x -= 1;
        }
    }



    player->coll_box = (Rectangle) {
        player->e.pos.x,
        player->e.pos.y,
        TILE_SIZE,
        TILE_SIZE
    };

/// TEST @TEMP
    player->center = (Vector2) { 
        player->e.pos.x + TILE_SIZE/2, 
        player->e.pos.y + TILE_SIZE/2,
    };
    
    player->curr_tile = (Vector2) {
        (int)(player->center.x / TILE_SIZE),
        (int)(player->center.y / TILE_SIZE),
    };
    

    if (IsKeyDown(KEY_U)) {
        player->speed--;
        printf("Player Speed: %f", player->speed);
    }
    if (IsKeyDown(KEY_I)) {
        player->speed++;
        printf("Player Speed: %f", player->speed);
    }

} 

void player_render(Player *player) {
/*
    DrawRectangleRec(
        (Rectangle){ 
            ((int)player->curr_tile.x) * TILE_SIZE, 
            ((int)player->curr_tile.y) * TILE_SIZE, 
            TILE_SIZE, 
            TILE_SIZE
        },
        GRAY
    );
    */

    // DRAW PLAYER
    DrawRectangleRec(
        (Rectangle){ 
            player->e.pos.x, 
            player->e.pos.y, 
            TILE_SIZE, 
            TILE_SIZE
        },
        GREEN
    );

    DrawRectangleRec(
        (Rectangle){ 
            player->center.x - 2, 
            player->center.y - 2,
            4, 
            4 
        },
        ORANGE
    );

        /*
        // UP
        DrawRectangleRec(
            (Rectangle){ 
                player.e.pos.x + 0, 
                player.e.pos.y - 16, 
                TILE_SIZE / 2, 
                TILE_SIZE / 2
            },
            WHITE
        );
        // DOWN
        DrawRectangleRec(
            (Rectangle){ 
                player.e.pos.x + 0, 
                player.e.pos.y + 16, 
                TILE_SIZE / 2, 
                TILE_SIZE / 2
            },
            WHITE
        );
        // RIGHT
        DrawRectangleRec(
            (Rectangle){ 
                player.e.pos.x + 16, 
                player.e.pos.y + 0, 
                TILE_SIZE / 2, 
                TILE_SIZE / 2
            },
            WHITE
        );
        // LEFT
        DrawRectangleRec(
            (Rectangle){ 
                player.e.pos.x - 16, 
                player.e.pos.y + 0, 
                TILE_SIZE / 2, 
                TILE_SIZE / 2
            },
            WHITE
        );
        */

    if (!timer_is_done(&player->cooldown_timer)) {
        DrawRectangleRec(player->wp_coll_box, WHITE);
    } else {
        player->is_attacking = false;
    }
}

void enemy_update(Enemy *enemy) {
    enemy->coll_box = (Rectangle) {
        enemy->e.pos.x,
        enemy->e.pos.y,
        TILE_SIZE,
        TILE_SIZE
    };
}

void enemy_render(Enemy *enemy) {
    if (enemy->hp <= 0) return;
    DrawRectangleRec(
        (Rectangle){ 
            enemy->e.pos.x, 
            enemy->e.pos.y, 
            TILE_SIZE, 
            TILE_SIZE
        },
        RED
    );
}

void draw_sprite() {
}

void handle_game_logic(Player *player, Enemy *enemy1, float delta_time) {
    if (timer_is_done(&player->cooldown_timer) == false && player->is_attacking) {
        // Check if Attack Box is colliding with the Enemy
        bool collision = CheckCollisionRecs(
            player->wp_coll_box,
            enemy1->coll_box
        );
        if (collision) {
            player->is_attacking = false;
            //@TOD: timer_cancel();
            printf("collision\n");
            enemy1->hp -= 1;
        }
    } 


    // For moving to another room 
        //timer_update(&camera_cooldown

            

            if (camera_is_moving) {
                // Transition to the other room is done
                camera.target = Vector2Add(camera.target, (Vector2){0, -200 * delta_time});
            
                if (camera.target.y <= camera_transition_dest.y) {
                    printf("We have stopped the camera transition");
                    camera_is_moving = false;
                }
            } else {
                bool collision = CheckCollisionRecs(
                    player->coll_box,
                    room_exit_coll_box
                );
                if (collision) {
                    printf("Collision with room");
                    //timer_start(&camera_cooldown, 3);
                    camera_is_moving = true;
                    camera_transition_dest = (Vector2){ 0, -1 * 11/2 * TILE_SIZE};
                }

            }

/*
        if (timer_is_done(&camera_cooldown)) {
            bool collision = CheckCollisionRecs(
                player->coll_box,
                room_exit_coll_box
            );
            if (collision) {
                printf("Collision with room");
                timer_start(&camera_cooldown, 3);
                camera_is_moving = true;
                camera_transition_dest = (Vector2){ 0, -1 * 11/2 * TILE_SIZE};
            }
        }
        else {
            // Transition to the other room is done
            camera.target = Vector2Add(camera.target, (Vector2){0, -200 * delta_time});
            
            if (camera.target.y <= camera_transition_dest.y) {
                printf("We have stopped the camera transition");
                camera_is_moving = false;
            }
        }
        */

}

// Free up stuff.. thanks
void shutdown() {
    //rooms_free(&rooms);
}

int main(void) {
    printf("Hello Sailor\n");

    startup();
    load_textures();
    load_sounds();

    // Game stuff initialization
    rooms = rooms_init();
    if (rooms == NULL) {
        printf("ROOMS IS NULL\n");
    }
    
    Enemy enemy1 = { 0 };
    enemy1.e.pos = (Vector2) { 3 * TILE_SIZE, 3 * TILE_SIZE };
    enemy1.coll_box = (Rectangle) {
        enemy1.e.pos.x,
        enemy1.e.pos.y,
        TILE_SIZE,
        TILE_SIZE
    };
    enemy1.hp = 3;


    room_exit_coll_box = (Rectangle) { 0, 3 * TILE_SIZE, 4 *TILE_SIZE, TILE_SIZE };

    // Main game loop
    while (!WindowShouldClose()) {

        //  Compute Frame Buffer Scaling
        float scale = MIN((float)GetScreenWidth()/game_width, (float)GetScreenHeight()/game_height);

        float delta_time = GetFrameTime();

        handle_game_logic(&player, &enemy1, delta_time);

        player_update(&player, delta_time);

        //update_camera(&camera, &player.e, delta_time, game_width, game_height);


        timer_update(&player.cooldown_timer);

        BeginTextureMode(target);
            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);

                rooms_render(rooms);
                enemy_render(&enemy1);
                player_render(&player);

        DrawTextureRec(
            overworld,
            (Rectangle){ 0 * TILE_SIZE, 10 * TILE_SIZE, TILE_SIZE, TILE_SIZE},
            (Vector2){
                5 * TILE_SIZE,
                (3 + 5) * TILE_SIZE
            },
            WHITE
        );

    DrawRectangleRec(
        room_exit_coll_box,
        PINK
    );


            EndMode2D();

			DrawFPS(
				0,
                0
			);
        EndTextureMode();

		BeginDrawing();
            ClearBackground(BLACK);
            // Draw render texture to screen, properly scaled
            DrawTexturePro(
                target.texture, 
                (Rectangle){ 
                    0.0f, 
                    0.0f, 
                    (float)target.texture.width, 
                    (float)-target.texture.height 
                },
                (Rectangle){ 
                    (GetScreenWidth() - ((float)game_width*scale))*0.5f, 
                    (GetScreenHeight() - ((float)game_height*scale))*0.5f,
                    (float)game_width*scale, 
                    (float)game_height*scale 
                }, 
                (Vector2){ 0, 0 }, 0.0f, 
                WHITE
            );
		EndDrawing();
    }

    return 0;
}
