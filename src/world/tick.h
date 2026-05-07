#pragma once

#include "world.h"

extern double last_tick_time;
extern double tick_interval;

void world_tick(struct world *world);

