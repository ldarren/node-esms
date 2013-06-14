// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
// ported to V8 by Darren Liew <2013>
//
// This program is free software, licensed with the GPL (www.fsf.org)
//
#include <node.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <cmath>
#include <climits>
#include <cctype>


using namespace std;
using namespace v8;

#include "util.h"
#include "object_v8.h"

bool waitflag = false; // required by util.cpp

char nationalities[20][4] = {"arg", "aus", "bra", "bul",
                             "cam", "cro", "den", "eng",
                             "fra", "ger", "hol", "ire",
                             "isr", "ita", "jap", "nig",
                             "nor", "saf", "spa", "usa"};

const unsigned N_GAUSS = 1000;

double gaussian_vars[N_GAUSS] = {0};

inline unsigned uniform_random(unsigned max);
inline unsigned averaged_random_part_dev(unsigned average, unsigned div);
inline unsigned averaged_random(unsigned average, unsigned max_deviation);
void fill_gaussian_vars_arr(double* arr, unsigned amount);
string gen_random_name(void);

/*
bool more_st(const RosterPlayer& p1, const RosterPlayer& p2)
{
	return p1.st > p2.st;
}


bool more_tk(const RosterPlayer& p1, const RosterPlayer& p2)
{
	return p1.tk > p2.tk;
}


bool more_ps(const RosterPlayer& p1, const RosterPlayer& p2)
{
	return p1.ps > p2.ps;
}


bool more_sh(const RosterPlayer& p1, const RosterPlayer& p2)
{
	return p1.sh > p2.sh;
}
*/

Handle<Value> create(const Arguments& args)
{
    HandleScope scope;

    ObjectV8 cfg(Handle<Object>::Cast(args[0]));

    // Setting up some default values for the
    // configuration data variables
    //
    int cfg_n_gk = cfg.get("N_GK", 3);
    int cfg_n_df = cfg.get("N_DF", 8);
    int cfg_n_dm = cfg.get("N_DM", 3);
    int cfg_n_mf = cfg.get("N_MF", 8);
    int cfg_n_am = cfg.get("N_AM", 3);
    int cfg_n_fw = cfg.get("N_FW", 5);
    int cfg_average_stamina = cfg.get("AVERAGE_STAMINA", 60);
    int cfg_average_aggression = cfg.get("AVERAGE_AGGRESSION", 30);
    int cfg_average_main_skill = cfg.get("AVERAGE_MAIN_SKILL", 14);
    int cfg_average_mid_skill = cfg.get("AVERAGE_MID_SKILL", 11);
    int cfg_average_secondary_skill = cfg.get("AVERAGE_SECONDARY_SKILL", 7);
    unsigned cfg_rand_seed = cfg.get("RANDOM_SEED", (unsigned)time(NULL));

    int n_players = cfg_n_gk + cfg_n_df + cfg_n_dm + cfg_n_mf + cfg_n_am + cfg_n_fw;

    Handle<Array> playersArr = Array::New(n_players);
    ObjectV8 *player;

    srand(cfg_rand_seed);

    fill_gaussian_vars_arr(gaussian_vars, N_GAUSS);

    for (int pl_count = 1; pl_count <= n_players; ++pl_count)
    {
        player = new ObjectV8();
        int temp_rand = 0;

        // Name: empty, or generated, depends on flag in configuration file
        //
        player->set("name", gen_random_name());

        // Nationality: randomly chosen from 20 possibilities
        //
        temp_rand = uniform_random(19);
        assert(temp_rand >= 0 && temp_rand <= 19);
        player->set("nationality", nationalities[temp_rand]);

        // Age: Varies between 16 and 30
        //
        player->set("age", averaged_random(23, 7));

        // Preferred side: preset probability for each
        //
        temp_rand = uniform_random(150);

        string temp_side;

        if (temp_rand <= 8)
            temp_side = "RLC";
        else if (temp_rand <= 13)
            temp_side = "RL";
        else if (temp_rand <= 23)
            temp_side = "RC";
        else if (temp_rand <= 33)
            temp_side = "LC";
        else if (temp_rand <= 73)
            temp_side = "R";
        else if (temp_rand <= 103)
            temp_side = "L";
        else
            temp_side = "C";

        player->set("pref_side", temp_side);

        int half_average_secondary_skill = cfg_average_secondary_skill / 2;

        // Skills: Depends on the position, first n_goalkeepers
        // will get the highest skill in St, and so on...
        //
        if (pl_count <= cfg_n_gk)
        {
            player->set("st", averaged_random_part_dev(cfg_average_main_skill, 3));
            player->set("tk", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("ps", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("sh", averaged_random_part_dev(half_average_secondary_skill, 2));
        }
        else if (pl_count <= cfg_n_gk + cfg_n_df)
        {
            player->set("tk", averaged_random_part_dev(cfg_average_main_skill, 3));
            player->set("st", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("ps", averaged_random_part_dev(cfg_average_secondary_skill, 2));
            player->set("sh", averaged_random_part_dev(cfg_average_secondary_skill, 2));
        }
        else if (pl_count <= cfg_n_gk + cfg_n_df + cfg_n_dm)
        {
            player->set("ps", averaged_random_part_dev(cfg_average_mid_skill, 3));
            player->set("tk", averaged_random_part_dev(cfg_average_mid_skill, 3));
            player->set("st", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("sh", averaged_random_part_dev(cfg_average_secondary_skill, 2));
        }
        else if (pl_count <= cfg_n_gk + cfg_n_df + cfg_n_dm + cfg_n_mf)
        {
            player->set("ps", averaged_random_part_dev(cfg_average_main_skill, 3));
            player->set("st", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("tk", averaged_random_part_dev(cfg_average_secondary_skill, 2));
            player->set("sh", averaged_random_part_dev(cfg_average_secondary_skill, 2));
        }
        else if (pl_count <= cfg_n_gk + cfg_n_df + cfg_n_dm + cfg_n_mf + cfg_n_am)
        {
            player->set("ps", averaged_random_part_dev(cfg_average_mid_skill, 3));
            player->set("sh", averaged_random_part_dev(cfg_average_mid_skill, 3));
            player->set("tk", averaged_random_part_dev(cfg_average_secondary_skill, 2));
            player->set("st", averaged_random_part_dev(half_average_secondary_skill, 2));
        }
        else
        {
            player->set("sh", averaged_random_part_dev(cfg_average_main_skill, 3));
            player->set("st", averaged_random_part_dev(half_average_secondary_skill, 2));
            player->set("tk", averaged_random_part_dev(cfg_average_secondary_skill, 2));
            player->set("ps", averaged_random_part_dev(cfg_average_secondary_skill, 2));
        }

        // Stamina
        //
        player->set("stamina", averaged_random_part_dev(cfg_average_stamina, 2));

        // Aggression
        //
        player->set("ag", averaged_random_part_dev(cfg_average_aggression, 3));

        // Abilities: set all to 300
        //
        player->set("st_ab", 300);
        player->set("tk_ab", 300);
        player->set("ps_ab", 300);
        player->set("sh_ab", 300);

        // Other stats
        //
        player->set("games", 0);
        player->set("saves", 0);
        player->set("tackles", 0);
        player->set("keypasses", 0);
        player->set("shots", 0);
        player->set("goals", 0);
        player->set("assists", 0);
        player->set("dp", 0);
        player->set("injury", 0);
        player->set("suspension", 0);
        player->set("fitness", 100);
        
        playersArr->Set(pl_count-1, player->toJSObject());
        delete player;
    }
   /* 
    sort(	players_arr.begin(), 
            players_arr.begin() + cfg_n_gk, 
            more_st);
    
    sort(	players_arr.begin() + cfg_n_gk, 
            players_arr.begin() + cfg_n_gk + cfg_n_df + cfg_n_dm, 
            more_tk);
    
    sort(	players_arr.begin() + cfg_n_gk + cfg_n_df + cfg_n_dm, 
            players_arr.begin() + cfg_n_gk + cfg_n_df + cfg_n_dm + cfg_n_mf + cfg_n_am, 
            more_ps);
    
    sort(	players_arr.begin() + cfg_n_gk + cfg_n_df + cfg_n_dm + cfg_n_mf + cfg_n_am, 
            players_arr.end(), 
            more_sh);
    */
    return scope.Close(playersArr);
}


// Return a pseudo-random integer uniformly distributed
// between 0 and max
//
inline unsigned uniform_random(unsigned max)
{
    double d = rand() / (double) RAND_MAX;
    unsigned u = (unsigned)  (d * (max + 1));

    return (u == max + 1 ? max - 1 : u);
}


inline unsigned averaged_random_part_dev(unsigned average, unsigned div)
{
    return averaged_random(average, average / div);
}


inline unsigned averaged_random(unsigned average, unsigned max_deviation)
{
    double rand_gaussian = gaussian_vars[uniform_random(N_GAUSS-1)];
    double deviation = max_deviation * rand_gaussian;

    return average + (unsigned)deviation;
}


void fill_gaussian_vars_arr(double* arr, unsigned amount)
{
    for (unsigned i = 0; i < amount; ++i)
    {
        double s = 0;
        double v1 = 0;
        double v2 = 0;
        double x = 0;

        do
        {
            do
            {
                double u1 = rand() / (double)RAND_MAX;
                double u2 = rand() / (double)RAND_MAX;

                v1 = 2*u1 - 1;
                v2 = 2*u2 - 1;

                s = v1*v1 + v2*v2;
            }
            while (s >= 1.0);

            x = v1*sqrt((-2)*log(s)/s);
        }
        while(x >= 1.0 || x <= -1.0);

        arr[i] = x;
    }
}


// Given a string with comma separated values (like "a,cd,k")
// returns a random value.
//
string rand_elem(string csv)
{
    vector<string> elems = tokenize(csv, ",");
    return elems[uniform_random(unsigned(elems.size()) - 1)];
}


// Throws a bet with probability prob of success. Returns
// true iff succeeded.
//
bool throw_with_prob(unsigned prob)
{
    unsigned a_throw = 1 + uniform_random(99);

    return (prob >= a_throw) ? true : false;
}


// A very rudimentary random name generator
//
string gen_random_name(void)
{
    string vowelish = "a,o,e,i,u";
    string vowelish_not_begin = "ew,ow,oo,oa,oi,oe,ae,ua";
    string consonantish =
        "b,c,d,f,g,h,j,k,l,m,n,p,r,s,t,v,y,z,br,cl,gr,st,jh,tr,ty,dr,kr,ry,bt,sh,ch,pr";
    string consonantish_not_begin =
        "mn,nh,rt,rs,rst,dn,nd,ds,bt,bs,bl,sk,vr,ks,sy,ny,vr,sht,ck";

    char first_name_abbr = 'A' + char(uniform_random(25));

    bool last_was_vowel = false;
    string result = "";

    // Generate first name (a letter + "_")
    result += first_name_abbr;
    result += "_";

    // Generate beginning
    if (throw_with_prob(50))
    {
        result += rand_elem(vowelish);
        last_was_vowel = true;
    }
    else
    {
        result += rand_elem(consonantish);
        last_was_vowel = false;
    }

    unsigned howmany_proceed = 2 + uniform_random(3);

    for (unsigned i = 0; i < howmany_proceed; ++i)
    {
        if (last_was_vowel)
        {
            if (throw_with_prob(50))
                result += rand_elem(consonantish);
            else
                result += rand_elem(consonantish_not_begin);
        }
        else
        {
            if (throw_with_prob(75))
                result += rand_elem(vowelish);
            else
                result += rand_elem(vowelish_not_begin);
        }

        last_was_vowel = !last_was_vowel;
    }

    if (result.size() > 12)
        result = result.substr(0, 9 + uniform_random(3));

    // Eventually, capitalize the first letter of the surename
    //
    result[2] = toupper(result[2]);

    return result;
}

void initialize(Handle<Object> exports, Handle<Object> module){
    module->Set(String::NewSymbol("exports"), FunctionTemplate::New(create)->GetFunction());
}

NODE_MODULE(roster_creator, initialize)
