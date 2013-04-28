// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#include <node.h>

#include <cstdlib>
#include <cstring>
#include <string>

#include "tsc_v8.h"
#include "object_v8.h"
#include "util.h"

using namespace v8;


// wait on exit
//
bool waitflag = true;



inline int st_getter(Handle<Object> player)
{
	return player->Get(String::NewSymbol("st"))->IntegerValue()  * player->Get(String::NewSymbol("fitness"))->IntegerValue() / 100;
}


inline int tk_getter(Handle<Object> player)
{
	return player->Get(String::NewSymbol("tk"))->IntegerValue()  * player->Get(String::NewSymbol("fitness"))->IntegerValue() / 100;
}


inline int ps_getter(Handle<Object> player)
{
	return player->Get(String::NewSymbol("ps"))->IntegerValue()  * player->Get(String::NewSymbol("fitness"))->IntegerValue() / 100;
}


inline int sh_getter(Handle<Object> player)
{
	return player->Get(String::NewSymbol("sh"))->IntegerValue()  * player->Get(String::NewSymbol("fitness"))->IntegerValue() / 100;
}


/// Gets the best player on some position from an array of roster players.
/// 
/// players 		- the array of players
/// chosen_players 	- a set of already chosen players (those won't be chosen again)
/// skill 			- pointer to a function receiving a player and returning the skill by
/// 				  which "best" is judged.
///
/// Returns the chosen player's name. Note: chosen_players is not modified !
///
Handle<String> choose_best_player(	const Handle<Array> players, 
							Handle<Object> chosen_players, 
							int (*skill)(Handle<Object> player))
{
    HandleScope scope;

    Handle<Object> player;
	int best_skill = -1, ret;
	Handle<String> name_of_best, name;
    Handle<String> keyName = String::NewSymbol("name");
    Handle<String> keyInjury = String::NewSymbol("injury");
    Handle<String> keySuspension = String::NewSymbol("suspension");

    for (int i=0, l=players->Length(); i<l; ++i){
        player = players->Get(i)->ToObject();

        name = player->Get(keyName)->ToString();

        if (chosen_players->Has(name)) continue;

        ret = skill(player);

        if (ret > best_skill && !player->Get(keyInjury)->IntegerValue() && !player->Get(keySuspension)->IntegerValue()){
            best_skill = ret;
            name_of_best = name;
        }
    }
	chosen_players->Set(name, player); // player value is redundant

	return scope.Close(name_of_best);
}


Handle<Value> create(const Arguments &args)
{
    HandleScope scope;

    if (4 != args.Length()) return Undefined();

    Handle<String> teamname = Handle<String>::Cast(args[0]);
    Handle<String> formation = Handle<String>::Cast(args[1]);
    Handle<Array> players(Handle<Array>::Cast(args[2]));
    ObjectV8 leagueDat(Handle<Object>::Cast(args[3]));

    int num_subs = leagueDat.get("num_subs", 7);

    // The number of subs is not constant, therefore there is
    // a need for some smart assignment. The following array
    // sets the positions of thr first 5 subs, and then iterates
    // cyclicly. For example, if there are 2 subs allowed,
    // their positions will be GK (mandatory 1st !) and MF
    // If 7: GK, DF, MF, DF, FW, MF, DF
    //                              ^
    //                              cyclic repetition begins
    //
    const char* sub_position[] = {"DFC", "MFC", "DFC", "FWC", "MFC"};

    // Iterates (cyclicly) over positions of subs,
    //
    int sub_pos_iter = 0;

    if (players->Length() < (unsigned)(11 + num_subs))
		return Undefined(); // Error: not enough players in roster

    int dfs, mfs, fws;
    char tactic[2];

    String::AsciiValue fmt(formation);
    if (!parse_formation(*fmt, dfs, mfs, fws, tactic)) return Undefined();

    // Calculate indices of the last defender and the last midfielder
    //
    int last_df = dfs + 1;
    int last_mf = dfs + mfs + 1;

    // Pick the players
    //
    // First, the best shot stopper is picked as a GK, then
    // others are picker according to the schedule of sub_position
    // as described above
    //
	
	// This will keep us from picking the same players more than once
	//
    Handle<String> keyPos = String::NewSymbol("pos");
    Handle<String> keyName = String::NewSymbol("name");

    Handle<Array> t_players = Array::New();
    Handle<Object> t_player;
    int i;
	
    for (i = 1; i <= 11; i++)
    {
        t_player = Object::New();
        if (i == 1)
            t_player->Set(keyPos, String::NewSymbol("GK"));
        else if (i >= 2 && i <= last_df)
            t_player->Set(keyPos, String::NewSymbol("DFC"));
        else if (i > last_df && i <= last_mf)
            t_player->Set(keyPos, String::NewSymbol("MFC"));
        else if (i > last_mf && i <= 11)
            t_player->Set(keyPos, String::NewSymbol("FWC"));

        t_players->Set(i, t_player);
    }

	Handle<Object> chosen_players = Object::New();
	

    // set the best GK for N.1 position
    //
    t_player = t_players->Get(1)->ToObject();
    t_players->Set(keyName, choose_best_player(players, chosen_players, st_getter));

    // From now on, i is the index for players in the teamsheet
	//

    // Set the starting defenders
	//
    for (i = 2; i <= last_df; i++)
    {
        t_player = t_players->Get(i)->ToObject();
        t_player->Set(keyName, choose_best_player(players, chosen_players, tk_getter));
    }

    // Set the starting midfielders
	//
    for (i = last_df + 1; i <= last_mf; i++)
    {
        t_player = t_players->Get(i)->ToObject();
        t_player->Set(keyName, choose_best_player(players, chosen_players, ps_getter));
    }

    // Set the starting forwards
	//
    for (i = last_mf + 1; i <= 11; i++)
    {
        t_player = t_players->Get(i)->ToObject();
        t_player->Set(keyName, choose_best_player(players, chosen_players, sh_getter));
    }

    // Set the substitute GK
	//
    t_player = Object::New();
    Handle<String> name_of_best = choose_best_player(players, chosen_players, st_getter);
    t_player->Set(keyName, name_of_best);
    t_player->Set(keyPos, String::NewSymbol("GK"));
    t_players->Set(Integer::New(12), t_player);
    chosen_players->Set(name_of_best, t_player);

    for (i = 13; i <= num_subs + 11; ++i)
    {
        t_player = Object::New();
        const char *pos = sub_position[sub_pos_iter];
        // What position should the current sub be on ?
        //
        if (!strcmp(pos, "DFC"))
			name_of_best = choose_best_player(players, chosen_players, tk_getter);
        else if (!strcmp(pos, "MFC"))
			name_of_best = choose_best_player(players, chosen_players, ps_getter);
        else // if (!strcmp(pos, "FWC"))
			name_of_best = choose_best_player(players, chosen_players, sh_getter);

        t_player->Set(keyName, name_of_best);
        t_player->Set(keyPos, String::NewSymbol(pos));
        chosen_players->Set(name_of_best, t_player);
        sub_pos_iter = (sub_pos_iter + 1) % 5;
        t_players->Set(Integer::New(i), t_player);
    }

    Handle<Object> teamsheet = Object::New();

    // Start filling the team sheet with the roster name and the
    // tactic
    //
    teamsheet->Set(String::New("teamName"),  teamname);
    teamsheet->Set(String::New("tactic"),  String::New(tactic));

    // Print all the players and their position
    for (i = 1; i <= 11 + num_subs; i++)
    {
        t_player = t_players->Get(i)->ToObject();
        teamsheet->Set(t_player->Get(keyPos), t_player->Get(keyName));
    }

    // Print the penalty kick taker (player number last_mf + 1)
    t_player = t_players->Get(last_mf + 1)->ToObject();
    teamsheet->Set(String::New("PK"), t_player->Get(keyName));

    return scope.Close(teamsheet);
}


// Parses the formation line, finds out how many defenders,
// midfielders and forwards to pick, and the tactic to use,
// performs error checking
//
// For example: 442N means 4 DFs, 4 MFs, 2 FWs, playing N
//
bool parse_formation(char* formation, int& dfs, int& mfs,
                     int& fws, char* tactic)
{
    if (strlen(formation) != 4)
    {
        return false; // The formation string must be exactly 4 characters long, For example: 442N
    }

    // Random formation ?
    //
    if (!strncmp(formation, "rnd", 3))
    {
        srand(time(NULL));

        // between 3 and 5
        dfs = 3 + rand() % 3;

        // if there are 5 dfs, max of 4 mfs
        if (dfs == 5)
        {
            mfs = 1 + rand() % 4;
        }
        else // 5 mfs is also possible
        {
            mfs = 1 + rand() % 5;
        }

        fws = 10 - dfs - mfs;

        tactic[0] = formation[3];
        tactic[1] = '\0';

        return true;
    }

    dfs = formation[0] - '0';
    mfs = formation[1] - '0';
    fws = formation[2] - '0';

    tactic[0] = formation[3];
    tactic[1] = '\0';

    if (!verify_position_range(dfs)) return false;
    if (!verify_position_range(mfs)) return false;
    if (!verify_position_range(fws)) return false;

    if (dfs + mfs + fws != 10)
    {
        return false; // The number of players on all positions added together must be 10, For example: 442N
    }
    return true;
}


bool verify_position_range(int n)
{
    if (n < 1 || n > 8)
    {
        return false; // The number of players on each position must be between 1 and 8, For example: 442N
    }
    return true;
}

void initialize(Handle<Object> exports, Handle<Object> module){
    module->Set(String::NewSymbol("exports"), FunctionTemplate::New(create)->GetFunction());
}

NODE_MODULE(tsc, initialize)
