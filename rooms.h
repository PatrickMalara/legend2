#pragma once
#include "global.h"
#include "engine.h"


Room *rooms_init();
void rooms_free(Room *rooms);
void rooms_render(Room *rooms);
