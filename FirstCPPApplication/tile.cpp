#include "stdafx.h"
#include "tile.h"
#include <cstdlib>
#include <sstream>
#include <fstream>

#include "libtcod_cpp_hpp\libtcod.hpp"

#include "game.h"
#include "tile.h"
#include "map.h"
#include "item.h"
#include "inventory.h"
#include "Representation.h"
#include "utils.h"
#include "enums\tiletypes_t.h"
#include <Room.h>
#include <actors\Person.h>
#include <map.h>



using namespace std;

BaseTileType* Tile::BaseType = new BaseTileType; //all tile types must inherit this
WarpTileType* Tile::WarpType = new WarpTileType;
WallTileType* Tile::WallType = new WallTileType;
DoorTileType* Tile::DoorType = new DoorTileType;
StairsUpTileType* Tile::StairsUpType = new StairsUpTileType;
StairsDownTileType* Tile::StairsDownType = new StairsDownTileType;
FloorTileType* Tile::FloorType = new FloorTileType;

Tile::Tile()
{
    this->tile = new BaseTileType;
    this->is_deleted = false;
    this->type_id = TileTypes::BaseTileTypeType;
    this->_is_occupied = false;
    this->_is_known = false;

    this->is_custom_tile = false;
    this->custom_tile = NULL;

    this->occupants = new actor_vec_t;
    this->occupant = NULL;
    // items = new std::vector<Item*>;
    this->inventory = new Inventory();

    this->is_open = false;

    updateTileType(type_id);
};

Tile::~Tile()
{
    // this->occupants->clear();
    // delete this->occupants;


	//if (this->inventory != NULL)
	//{
    delete this->inventory; //TODO impl delete inventory
	this->inventory = NULL;
	//};
    if (this->is_custom_tile)
    {
        delete this->custom_tile;
    };

};

BaseTileType* Tile::get_tile_type()
{
    if (! this->is_custom_tile)
    {
        return this->tile;
    }
    else
    {
        return this->custom_tile;
    };
};

std::string Tile::get_description()
{
    if (! this->is_custom_tile)
    {
        return this->tile->description;
    }
    else
    {
        return this->custom_tile->description;
    };
};
void Tile::set_description(std::string new_desc)
{
    if (!this->is_custom_tile)
    {
        this->updateCustomTileType(this->type_id);
    }
    this->custom_tile->description = new_desc;

};

Representation* Tile::get_representation()
{
    if (!this->is_custom_tile)
    {
        return this->tile->representation;
    }
    else
    {
        return this->custom_tile->representation;
    };
};

void  Tile::set_representation(Representation* new_repr)
{
    if (!this->is_custom_tile)
    {
        this->updateCustomTileType(this->type_id);
    }
    // delete this->custom_tile->representation;
    this->custom_tile->representation = new_repr;
};

bool Tile::check_for_items() { 
    bool are_items = this->inventory->get_count() != 0; 
    return are_items;
};

void Tile::drawColorsToRoot(TCODColor* fg_color, TCODColor* bg_color)
{
    // if (fg_color != 0)
    // {
    //     TCODConsole::root->setCharForeground(this->tile_x, this->tile_y, fg_color);
    // };
    // if (bg_color != NULL)
    // {
    //     TCODConsole::root->setCharBackground(this->tile_x, this->tile_y, bg_color);
    // };
};

void Tile::setKnown(bool is_known)
{
    this->_is_known = is_known;
};

void Tile::place_item_down(Item* item)
{
    this->inventory->add_item(item);
};
void Tile::pick_up_item(Item* item)
{
    this->inventory->remove_item(item);
};

void Tile::makeOccupied(Actor* the_actor)
{
    occupant = the_actor;
    occupants->push_back(the_actor);

    if (occupants->size() > 0)
    {
        _is_occupied = true;
        bool is_trans = map->l_map->isTransparent(tile_x, tile_y);
        map->l_map->setProperties(tile_x, tile_y, is_trans, false);
    }
    else
    {
        cout << "tile ain't occupied" << endl;
    };
};

void Tile::makeUnoccupied(Actor* the_actor)
{
    //remove the actor from vector
    for(actor_vec_t::size_type i = 0; i != occupants->size(); i++) 
    {
        if (occupants->at(i) == the_actor)
        {
            //cout << "ASDADSAD: " << occupants->size() << endl;
            occupants->erase(occupants->begin() + i);
            //cout << "post ASDADSAD: " << occupants->size() << endl;
            break;
        }
    }

    //see if anyone else is left on the tile, if not:
    if (occupants->size() == 0)
    {
        _is_occupied = false;
        occupant = NULL;
        bool is_trans = map->l_map->isTransparent(tile_x, tile_y);
        map->l_map->setProperties(tile_x, tile_y, is_trans, true);
    }
    else
    {
        //make the last actor on top of the tile
        occupant = occupants->back();
        //cout << "tile ain't empty" << endl;
    };
};

void Tile::updateCustomTileType(int type )
{

    this->type_id = type;

    if (type == 0) {
        this->custom_tile = new BaseTileType; 
    }
    else if (type == 1) { 
        this->custom_tile = new WallTileType;
        this->map->l_map->setProperties(this->tile_x, this->tile_y, false, false);
    }

    else if (type == 2) {
        this->custom_tile = new WarpTileType; 
    }
    else if (type == 3) {
        this->custom_tile = new FloorTileType;
        this->map->l_map->setProperties(this->tile_x, this->tile_y, true, true);
    }
    else if (type == 4) { this->custom_tile = new DoorTileType; }
    else if (type == 5) { this->custom_tile = new StairsDownTileType; }
    else if (type == 6) { this->custom_tile = new StairsUpTileType; }
    else 
    {
        cout << type << endl;
        cout << "Invalid TILETYPE" << endl; //probably because the tiletype is being assigned with a `new` call.
    }
    this->tile->tile=this;
    this->is_deleted = false;

    this->is_custom_tile = true;

};

void Tile::updateTileType(int type )
{

    this->type_id = type;
    this->is_custom_tile = false;

    if (type == 0) {
        tile = Tile::BaseType; 
    }
    else if (type == 1) { 
        // tile = new WallTileType;
        tile = Tile::WallType;
        this->map->l_map->setProperties(this->tile_x, this->tile_y, false, false);
    }

    else if (type == 2) {
        // tile = new WarpTileType; 
        tile = Tile::WarpType;
    }
    else if (type == 3) {
        // tile = new FloorTileType;
        tile = Tile::FloorType;
        this->map->l_map->setProperties(this->tile_x, this->tile_y, true, true);
    }
    else if (type == 4) {
        // tile = new DoorTileType; 
        tile = Tile::DoorType;
    }
    else if (type == 5) 
    {
        // tile = new StairsDownTileType; 
        tile = Tile::StairsDownType;
    }
    else if (type == 6) 
    {
        // tile = new StairsUpTileType; 
        tile = Tile::StairsUpType;
    }
    else 
    {
        cout << type << endl;
        cout << "Invalid TILETYPE" << endl; //probably because the tiletype is being assigned with a `new` call.
    }
    // this->tile->tile=this;
    this->is_deleted = false;

};

Tile* Tile::getTileAtRelative(int x, int y)
{
    //if tile is in map get tile, otherwise fail nicely
    int new_x = this->tile_x + x;
    int new_y = this->tile_y + y;

    return this->map->getTileAt(new_x, new_y);
};

Tile* Tile::getTopLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, -1*scale);
};

Tile* Tile::getTopMidTile(int scale)
{
    return this->getTileAtRelative(0*scale, -1*scale);
};

Tile* Tile::getTopRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, -1*scale);
};

Tile* Tile::getMidRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, 0*scale);
};

Tile* Tile::getBotRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, 1*scale);
};

Tile* Tile::getBotMidTile(int scale)
{
    return this->getTileAtRelative(0*scale, 1*scale);
};

Tile* Tile::getBotLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, 1*scale);
};

Tile* Tile::getMidLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, 0*scale);
};

vector<Tile*>* Tile::getAdjacentTiles(int scale)
{
    vector<Tile*>* seed_tiles = new vector<Tile*>;
    if (scale == 0) 
    {
        seed_tiles->push_back(this);
        return seed_tiles;
    };

    //get seed tiles, mark as tested
    seed_tiles->push_back(getTopLeftTile(1));
    seed_tiles->push_back(getTopMidTile(1));
    seed_tiles->push_back(getTopRightTile(1));
    seed_tiles->push_back(getMidRightTile(1));
    seed_tiles->push_back(getBotRightTile(1));
    seed_tiles->push_back(getBotMidTile(1));
    seed_tiles->push_back(getBotLeftTile(1));
    seed_tiles->push_back(getMidLeftTile(1));

    typedef  vector<Tile*> tv;
    if (scale == 1)
    {
        // delete checked_tiles;
        return seed_tiles;
    }
    else
    {
        vector<Tile*>* valid_tiles = new vector<Tile*>;
        tv* perimeter_tiles = new vector<Tile*>(seed_tiles->begin(), seed_tiles->end());
        //take the newly validated tiles and save them
        valid_tiles->insert(valid_tiles->end(), perimeter_tiles->begin(), perimeter_tiles->end());

        for (int count = 1; count < scale; count++)
        {
            tv new_perimeter = tv();
            for (tv::iterator perim = perimeter_tiles->begin(); perim != perimeter_tiles->end(); ++perim)
            {
                //look through the perimeters neighbors
                tv* neighbors = (*perim)->getAdjacentTiles(1);
                for (tv::iterator neighbor = neighbors->begin(); neighbor != neighbors->end(); ++neighbor)
                {
                    //if not already found, add to new perimeter
                    if (std::find(perimeter_tiles->begin(), perimeter_tiles->end(), *neighbor) == perimeter_tiles->end())
                        // if (std::find(valid_tiles->begin(), valid_tiles->end(), *neighbor) == valid_tiles->end())
                    {
                        Tile* tile = *neighbor;
                        // printf("%d %d\n", tile->tile_x, tile->tile_y );
                        new_perimeter.push_back(*neighbor);
                    };
                };
                delete neighbors;

            };

            //take the newly validated tiles and save them
            std::sort(new_perimeter.begin(), new_perimeter.end());
            new_perimeter.erase( unique( new_perimeter.begin(), new_perimeter.end() ), new_perimeter.end() );
            valid_tiles->insert(valid_tiles->end(), new_perimeter.begin(), new_perimeter.end());

            //new_perimeter becomes the next perimeter_tiles to check
            perimeter_tiles->clear();
            perimeter_tiles->reserve(new_perimeter.size());
            perimeter_tiles->insert(perimeter_tiles->end(), new_perimeter.begin(), new_perimeter.end());

            //reset new peri tiles
            new_perimeter.clear();

        };
        return valid_tiles;
    };


};

vector<Tile*>* Tile::getVacantAdjacentTiles()
{
    vector<Tile*>* adjacent_tiles = this->getAdjacentTiles();
    //remove all adjacent null tile ptrs
    adjacent_tiles->erase(
            std::remove_if(
                adjacent_tiles->begin(),
                adjacent_tiles->end(),
                [](Tile* s) -> bool { return s->is_occupied() || !s->map->l_map->isWalkable(s->tile_x, s->tile_y) || s==NULL;}),
            adjacent_tiles->end());

    adjacent_tiles->shrink_to_fit();
    return adjacent_tiles;
};

bool Tile::is_occupied() 
{
    return this->occupant != NULL;
    return this->_is_occupied; 
};

bool Tile::is_walkable()
{
    return this->map->l_map->isWalkable(this->tile_x, this->tile_y);
};

BaseTileType::BaseTileType() 
{
    this->color = new TCODColor;
    this->description = "Another descriptionless tile.";
    this->type_id = TileTypes::BaseTileTypeType;
    this->representation = new BaseRepresentation; 
};

BaseTileType::~BaseTileType() 
{
    delete this->color;
    delete this->representation;
};

WallTileType::WallTileType() : BaseTileType() 
{
    this->description = "Non descript wall.";
    type_id = TileTypes::WallTileTypeType;
    representation = new WallRepresentation;
};

WarpTileType::WarpTileType() : BaseTileType() 
{
    this->description = "A strange color eminates from the ground.";
    type_id = TileTypes::WarpTileTypeType;
    representation = new WarpRepresentation; 
};

DoorTileType::DoorTileType()  : BaseTileType() 
{
    this->description = "A door, it's probably open, but who can even tell?";
    this->is_open = false;
    type_id = TileTypes::DoorTileTypeType;
    representation = new DoorRepresentation; 
};

void Tile::ToggleDoor()
{
    if (this->is_open)
    {
        this->CloseDoor();
    }
    else
    {
        this->OpenDoor();
    };
}

void Tile::CloseDoor()
{
    this->set_description("A door is closed here.");
    this->is_open = false;
    Representation* old_repr = this->get_representation();
    old_repr->repr = '+';
    this->set_representation(old_repr);
    this->map->l_map->setProperties(this->tile_x, this->tile_y, false, false);
};

void Tile::OpenDoor()
{
    this->set_description("An open door.");
    this->is_open = true;
    Representation* old_repr = this->get_representation();
    old_repr->repr = '=';
    this->set_representation(old_repr);
    this->map->l_map->setProperties(this->tile_x, this->tile_y, true, true);
};

StairsDownTileType::StairsDownTileType() : BaseTileType() 
{
    this->description = "Stairs leading downwards.";
    type_id = TileTypes::StairsDownTileTypeType;
    representation = new StairsDownRepresentation; 
};

bool one_floor_up(Map* world)
{
    return world->depth == Game::current_map->depth-1;
};

bool one_floor_down(Map* world)
{
    return world->depth == Game::current_map->depth+1;
};

void StairsUpTileType::GoUp()
{
    // auto map = Game::build_world(Game::current_map->depth+1);
    // find the map with the depth of one less than the current
    auto it = std::find_if(Game::atlas->begin(), Game::atlas->end(), one_floor_up);
    if (it != Game::atlas->end())
    {
        Game::current_map = *it;
        // Room* room = Game::current_map->roomVector->front();
        int x = this->to_x;
        int y = this->to_y;
        // int x = room->center_x;
        // int y = room->center_y;
        Game::player->put_person(Game::current_map->getTileAt(x, y), x, y);
    };


};

void StairsDownTileType::GoDown()
{
    auto it = std::find_if(Game::atlas->begin(), Game::atlas->end(), one_floor_down);
    Map* map;
    if (it == Game::atlas->end())
    {
        map = Game::build_world(Game::current_map->depth+1);
    }
    else
    {
        map = *it;
    };
    Game::current_map = map;

    Room* room = Game::current_map->roomVector->front();
    int x = room->center_x;
    int y = room->center_y;
    Game::player->put_person(Game::current_map->getTileAt(x, y), x, y);

    Tile* stair_tile = Game::current_map->getTileAt(x, y);
    //this->stair_x = x;
    //this->stair_y = y;
    stair_tile->updateTileType(TileTypes::StairsUpTileTypeType);
    //(StairsUpTileType*)(stair_tile->tile)->to_x = x;
    //(StairsUpTileType*)(stair_tile-tile)->to_y = y;
    Game::current_map->l_map->setProperties(stair_tile->tile_x, stair_tile->tile_y, true, true);


};

StairsUpTileType::StairsUpTileType() : BaseTileType() 
{
    this->description = "Stairs leading upwards";
    type_id = TileTypes::StairsUpTileTypeType;
    representation = new StairsUpRepresentation; 

    //this->to_x = NULL;
    //this->to_y = NULL;
};

FloorTileType::FloorTileType() : BaseTileType() 
{
    this->description = "Plain old floor.";
    type_id = TileTypes::FloorTileTypeType;
    representation = new FloorRepresentation; 
};
