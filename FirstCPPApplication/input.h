#ifndef INPUT_H
#define INPUT_H

#pragma once
#include "stdafx.h"
#include <map>
#include <iostream>

#include "libtcod.hpp"


class Person;
class Map;
class Game;

extern enum directions_t;
// extern std::map<TCOD_key_t, directions_t> movemap;


bool process_movement(Game* the_game,TCOD_key_t request, Person *player);
bool process_mouse_event(Game* the_game,TCOD_mouse_t request, Person *player);
bool process_key_event(Game* the_game,TCOD_key_t request, Person *player);
directions_t direction_pressed(TCOD_key_t key);

#endif
