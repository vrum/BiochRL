#include "stdafx.h"

#include "item_effect.h"
#include "attribute_container.h"
#include "attribute.h"
#include <actors/actor.h>
#include "utils.h"
#include "libtcod.hpp"

bool TimedEffect::is_expired(long double turn_count)
{
    return (this->effect->duration + this->turn_applied) <= turn_count;
};

ItemEffect::ItemEffect()
{
    this->health_current_val = 100;
    this->health_max_val = 0;
    this->health_regen_rate = 0;
    this->health_regen_interval = 0; 

    this->mana_current_val = 0;
    this->mana_max_val = 0;
    this->mana_regen_rate = 0;
    this->mana_regen_interval = 0; 

    this->armor_current_val = 0;
    this->armor_max_val = 0;
    this->armor_regen_rate = 0;
    this->armor_regen_interval = 0; 

    this->damage_current_val = 0;
    this->damage_max_val = 0;
    this->damage_regen_rate = 0;
    this->damage_regen_interval = 0; 

    this->hunger_current_val = 0;
    this->hunger_max_val = 0;
    this->hunger_regen_rate = 0;
    this->hunger_regen_interval = 0; 

    this->duration = -1;

    this->actors_applied_to = new std::vector<applied_to_s*>;
};

bool ItemEffect::is_timed()
{
    return this->duration != NO_DURATION;
};

void ItemEffect::set_all_vals_to(int new_val)
{
    this->set_health_vals_to(new_val);
    this->set_mana_vals_to(new_val);
    this->set_armor_vals_to(new_val);
    this->set_damage_vals_to(new_val);
    this->set_hunger_vals_to(new_val);
};

void ItemEffect::set_health_vals_to(int new_val)
{
    this->health_current_val = new_val;
    this->health_max_val = new_val;
    this->health_regen_rate = new_val;
    this->health_regen_interval = new_val; 
};

void ItemEffect::set_mana_vals_to(int new_val)
{
    this->mana_current_val = new_val;
    this->mana_max_val = new_val;
    this->mana_regen_rate = new_val;
    this->mana_regen_interval = new_val; 
};

void ItemEffect::set_armor_vals_to(int new_val)
{
    this->armor_current_val = new_val;
    this->armor_max_val = new_val;
    this->armor_regen_rate = new_val;
    this->armor_regen_interval = new_val; 
};

void ItemEffect::set_damage_vals_to(int new_val)
{
    this->damage_current_val = new_val;
    this->damage_max_val = new_val;
    this->damage_regen_rate = new_val;
    this->damage_regen_interval = new_val; 
};

void ItemEffect::set_hunger_vals_to(int new_val)
{
    this->hunger_current_val = new_val;
    this->hunger_max_val = new_val;
    this->hunger_regen_rate = new_val;
    this->hunger_regen_interval = new_val; 
};

bool ItemEffect::already_applied_health(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return ((*it)->health.all);
        };

    };
    return false;
};

bool ItemEffect::already_applied_mana(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return (*it)->mana.all;
        };

    };
    return false;
};

bool ItemEffect::already_applied_armor(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return ((*it)->armor.all);
        };

    };
    return false;
};

bool ItemEffect::already_applied_damage(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return ((*it)->damage.all);
        };

    };
    return false;
};

bool ItemEffect::already_applied_hunger(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return ((*it)->hunger.all);
        };

    };
    return false;
};

bool ItemEffect::already_applied_all(Actor* actor)
{

    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
            return ((*it)->health.all && (*it)->mana.all && (*it)->armor.all  && (*it)->damage.all );
        };

    };

    //if actor in list of people this item's already applied to, no reason to do
    // //it again
    // auto it = std::find(this->actors_applied_to->begin(), this->actors_applied_to->end(), actor);

    // return  it != this->actors_applied_to->end();

    // // The thing is that I need to sort out how to apply the item's effects to
    // // the actor once, because if it gets reapplied they might be invincible or
    // // gaining way too much health for ever.
    // //TODO TODO TODO

    return false;
};


void ItemEffect::mark_applied_health(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->health.all = true);
        };

    };
};

void ItemEffect::mark_applied_mana(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->mana.all = true);
        };

    };
};

void ItemEffect::mark_applied_armor(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->armor.all = true);
        };

    };
};

void ItemEffect::mark_applied_damage(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->damage.all = true);
        };

    };
};

void ItemEffect::mark_applied_hunger(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->hunger.all = true);
        };

    };
};

void ItemEffect::mark_applied_all(Actor* actor)
{
    //loop through the actors_applied_to vector and find one with the actor
    //passed in
    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
    for (it; it != this->actors_applied_to->end(); ++it)
    {
        if ((*it)->actor == actor)
        {
           ((*it)->health.all = true);
           ((*it)->mana.all = true);
           ((*it)->armor.all = true);
           ((*it)->damage.all = true);
        };

    };
};


//void ItemEffect::mark_applied_all(Actor* actor)
//{
//    //loop through the actors_applied_to vector and find one with the actor
//    //passed in
//    std::vector<applied_to_s*>::iterator it = this->actors_applied_to->begin();
//    for (it; it != this->actors_applied_to->end(); ++it)
//    {
//        if ((*it)->actor == actor)
//        {
//           ((*it)->all = true);
//        };
//
//    };

    // auto it = std::find(this->actors_applied_to->begin(), this->actors_applied_to->end(), actor);

    // if ( it != this->actors_applied_to->end() )
    // {
    //     //do nothing because it already exists in the vector
    // }
    // else
    // {
    //     this->actors_applied_to->push_back(actor);
    // };
//};

void ItemEffect::ApplyAllEffects(Actor* actor)
{
    this->ApplyHealthEffects(actor);
    this->ApplyManaEffects(actor);
    this->ApplyArmorEffects(actor);
    this->ApplyDamageEffects(actor);
    this->ApplyHungerEffects(actor);
};

void ItemEffect::ApplyHealthEffects(Actor* actor)
{
    if (! this->already_applied_health(actor))
    {
        actor->attrs->health->AddToMaxVal(this->health_max_val);
        actor->attrs->health->AddToCurrentVal(this->health_current_val);
        actor->attrs->health->AddToRegenRate(this->health_regen_rate);
        actor->attrs->health->AddToRegenInterval(this->health_regen_interval);
        this->mark_applied_health(actor);
    }
};

void ItemEffect::ApplyManaEffects(Actor* actor)
{
    if (! this->already_applied_mana(actor))
    {
        actor->attrs->mana->AddToMaxVal(this->mana_max_val);
        actor->attrs->mana->AddToCurrentVal(this->mana_current_val);
        actor->attrs->mana->AddToRegenRate(this->mana_regen_rate);
        actor->attrs->mana->AddToRegenInterval(this->mana_regen_interval);
        this->mark_applied_mana(actor);
    }
};

void ItemEffect::ApplyArmorEffects(Actor* actor)
{
    if (! this->already_applied_armor(actor))
    {
        actor->attrs->armor->AddToMaxVal(this->armor_max_val);
        actor->attrs->armor->AddToCurrentVal(this->armor_current_val);
        actor->attrs->armor->AddToRegenRate(this->armor_regen_rate);
        actor->attrs->armor->AddToRegenInterval(this->armor_regen_interval);
        this->mark_applied_armor(actor);
    }
};

void ItemEffect::ApplyDamageEffects(Actor* actor)
{
    if (! this->already_applied_damage(actor))
    {
        actor->attrs->damage->AddToMaxVal(this->damage_max_val);
        actor->attrs->damage->AddToCurrentVal(this->damage_current_val);
        actor->attrs->damage->AddToRegenRate(this->damage_regen_rate);
        actor->attrs->damage->AddToRegenInterval(this->damage_regen_interval);
        this->mark_applied_damage(actor);
    }
};

void ItemEffect::ApplyHungerEffects(Actor* actor)
{
    if (! this->already_applied_hunger(actor))
    {
        actor->attrs->hunger->AddToMaxVal(this->hunger_max_val);
        actor->attrs->hunger->AddToCurrentVal(this->hunger_current_val);
        actor->attrs->hunger->AddToRegenRate(this->hunger_regen_rate);
        actor->attrs->hunger->AddToRegenInterval(this->hunger_regen_interval);
        this->mark_applied_hunger(actor);
    }
};

void ItemEffect::RemoveAllEffects(Actor* actor)
{
    this->RemoveHealthEffects(actor);
    this->RemoveManaEffects(actor);
    this->RemoveArmorEffects(actor);
    this->RemoveDamageEffects(actor);
};

void ItemEffect::RemoveHealthEffects(Actor* actor)
{
    actor->attrs->health->RemoveFromCurrentVal(this->health_current_val);
    actor->attrs->health->RemoveFromMaxVal(this->health_max_val);
    actor->attrs->health->RemoveFromRegenRate(this->health_regen_rate);
    actor->attrs->health->RemoveFromRegenInterval(this->health_regen_interval);
};

void ItemEffect::RemoveManaEffects(Actor* actor)
{
    actor->attrs->mana->RemoveFromCurrentVal(this->mana_current_val);
    actor->attrs->mana->RemoveFromMaxVal(this->mana_max_val);
    actor->attrs->mana->RemoveFromRegenRate(this->mana_regen_rate);
    actor->attrs->mana->RemoveFromRegenInterval(this->mana_regen_interval);
};

void ItemEffect::RemoveArmorEffects(Actor* actor)
{
    actor->attrs->armor->RemoveFromCurrentVal(this->armor_current_val);
    actor->attrs->armor->RemoveFromMaxVal(this->armor_max_val);
    actor->attrs->armor->RemoveFromRegenRate(this->armor_regen_rate);
    actor->attrs->armor->RemoveFromRegenInterval(this->armor_regen_interval);
};

void ItemEffect::RemoveDamageEffects(Actor* actor)
{
    actor->attrs->damage->RemoveFromCurrentVal(this->damage_current_val);
    actor->attrs->damage->RemoveFromMaxVal(this->damage_max_val);
    actor->attrs->damage->RemoveFromRegenRate(this->damage_regen_rate);
    actor->attrs->damage->RemoveFromRegenInterval(this->damage_regen_interval);
};


std::string ItemEffect::full_str()
{

    std::string result = "";
    std::vector<std::string> string_vec;

    string_vec.push_back("HCV: "+std::to_string((long double)this->health_current_val));
    string_vec.push_back("HMV: "+std::to_string((long double)this->health_max_val));
    string_vec.push_back("HRR: "+std::to_string((long double)this->health_regen_rate));
    string_vec.push_back("HRI: "+std::to_string((long double)this->health_regen_interval));

    string_vec.push_back("MCV: "+std::to_string((long double)this->mana_current_val));
    string_vec.push_back("MMV: "+std::to_string((long double)this->mana_max_val));
    string_vec.push_back("MRR: "+std::to_string((long double)this->mana_regen_rate));
    string_vec.push_back("MRI: "+std::to_string((long double)this->mana_regen_interval));

    string_vec.push_back("ACV: "+std::to_string((long double)this->armor_current_val));
    string_vec.push_back("AMV: "+std::to_string((long double)this->armor_max_val));
    string_vec.push_back("ARR: "+std::to_string((long double)this->armor_regen_rate));
    string_vec.push_back("ARI: "+std::to_string((long double)this->armor_regen_interval));

    string_vec.push_back("DCV: "+std::to_string((long double)this->damage_current_val));
    string_vec.push_back("DMV: "+std::to_string((long double)this->damage_max_val));
    string_vec.push_back("DRR: "+std::to_string((long double)this->damage_regen_rate));
    string_vec.push_back("DRI: "+std::to_string((long double)this->damage_regen_interval));

    if (string_vec.size() != 0)
        string_vec.push_back("%c");

    return StringJoin(string_vec, '\n', false);

};

std::string ItemEffect::small_convert(std::string prefix, long double val)
{
    if (val != 0)
    {
        return prefix + std::to_string(val);
    }
    else
    {
        return "";
    };

};

std::vector<TCOD_colctrl_t> ItemEffect::oneline_str_colours()
{
    TCODColor health_color = TCODColor::desaturatedGreen;
    TCODConsole::setColorControl(TCOD_COLCTRL_1, health_color, TCODColor::black);
    TCODColor mana_color = TCODColor::desaturatedBlue;
    TCODConsole::setColorControl(TCOD_COLCTRL_2, mana_color, TCODColor::black);
    TCODColor armor_color = TCODColor::lightGrey;
    TCODConsole::setColorControl(TCOD_COLCTRL_3, armor_color, TCODColor::black);
    TCODColor damage_color = TCODColor::desaturatedRed;
    TCODConsole::setColorControl(TCOD_COLCTRL_4, damage_color, TCODColor::black);
    std::vector<TCOD_colctrl_t>  color_vector;   
    //for each attr in this item effect that isn't 0, return a attr specific
    //color
    if (this->health_current_val != 0)
        color_vector.push_back(TCOD_COLCTRL_1);
    if (this->health_max_val != 0)
        color_vector.push_back(TCOD_COLCTRL_1);
    if (this->health_regen_rate != 0)
        color_vector.push_back(TCOD_COLCTRL_1);
    if (this->health_regen_interval != 0)
        color_vector.push_back(TCOD_COLCTRL_1);
    
    if (this->mana_current_val != 0)
        color_vector.push_back(TCOD_COLCTRL_2);
    if (this->mana_max_val != 0)
        color_vector.push_back(TCOD_COLCTRL_2);
    if (this->mana_regen_rate != 0)
        color_vector.push_back(TCOD_COLCTRL_2);
    if (this->mana_regen_interval != 0)
        color_vector.push_back(TCOD_COLCTRL_2);

    if (this->armor_current_val != 0)
        color_vector.push_back(TCOD_COLCTRL_3);
    if (this->armor_max_val != 0)
        color_vector.push_back(TCOD_COLCTRL_3);
    if (this->armor_regen_rate != 0)
        color_vector.push_back(TCOD_COLCTRL_3);
    if (this->armor_regen_interval != 0)
        color_vector.push_back(TCOD_COLCTRL_3);

    if (this->damage_current_val != 0)
        color_vector.push_back(TCOD_COLCTRL_4);
    if (this->damage_max_val != 0)
        color_vector.push_back(TCOD_COLCTRL_4);
    if (this->damage_regen_rate != 0)
        color_vector.push_back(TCOD_COLCTRL_4);
    if (this->damage_regen_interval != 0)
        color_vector.push_back(TCOD_COLCTRL_4);

    //color_vector.push_back(TCOD_COLCTRL_STOP);
    return color_vector;
    
};

// std::string ItemEffect::oneline_str()
// {
// 
// };

std::string ItemEffect::oneline_str()
{

    std::string result = "";
    std::vector<std::string> string_vec;

    string_vec.push_back(this->small_convert("%cHCV: ", (long double)this->health_current_val));
    string_vec.push_back(this->small_convert("%cHMV: ", (long double)this->health_max_val));
    string_vec.push_back(this->small_convert("%cHRR: ", (long double)this->health_regen_rate));
    string_vec.push_back(this->small_convert("%cHRI: ", (long double)this->health_regen_interval));

    string_vec.push_back(this->small_convert("%cMCV: ", (long double)this->mana_current_val));
    string_vec.push_back(this->small_convert("%cMMV: ", (long double)this->mana_max_val));
    string_vec.push_back(this->small_convert("%cMRR: ", (long double)this->mana_regen_rate));
    string_vec.push_back(this->small_convert("%cMRI: ", (long double)this->mana_regen_interval));

    string_vec.push_back(this->small_convert("%cACV: ", (long double)this->armor_current_val));
    string_vec.push_back(this->small_convert("%cAMV: ", (long double)this->armor_max_val));
    string_vec.push_back(this->small_convert("%cARR: ", (long double)this->armor_regen_rate));
    string_vec.push_back(this->small_convert("%cARI: ", (long double)this->armor_regen_interval));

    string_vec.push_back(this->small_convert("%cDCV: ", (long double)this->damage_current_val));
    string_vec.push_back(this->small_convert("%cDMV: ", (long double)this->damage_max_val));
    string_vec.push_back(this->small_convert("%cDRR: ", (long double)this->damage_regen_rate));
    string_vec.push_back(this->small_convert("%cDRI: ", (long double)this->damage_regen_interval));

    return StringJoin(string_vec, ' ', true);

};

