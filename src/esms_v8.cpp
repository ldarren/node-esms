// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#include <node.h>

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

#include "game.h"
//#include "report_event.h"
#include "cond.h"
#include "util.h"
#include "mt.h"
#include "cond_utils.h"

#include "comment_v8.h"
#include "tactics_v8.h"
#include "object_v8.h"


using namespace std;
using namespace v8;

// temp buffer to convert nonstring to string
char buf[1024];


// aux vars
//
int home_bonus, score_diff;

// Num of players in a teamsheet (the amount of subs can now be
// set in league.dat)
//
int num_players;

struct teams team[2];

//vector<report_event*> report_vec;

// This array is used to store the teams' total stats on
// various minutes during the game
// teamStatsTotal[x][y][z]
// x: team (0/1)
// y: minute (1-90 in steps of 10)
// z: 0 = total tackling, 1 = total passing, 2 = total shooting
//
double teamStatsTotal[2][10][3];
bool team_stats_total_enabled;

// These indicators are used for INJ, RED and YELLOW conditionals
//
// each is an array, [0] for team 0, [1] for team 1 and contains
// the number of the player that was injured or got a card on that
// minute, or -1 if there is no such player
//
int yellow_carded[2];
int red_carded[2];
int injured_ind[2];

// whether there is a wait on exit
//
bool waitflag = true;


/// "Gross" game minute.
///
/// From 1 to 45 + extra time in the first half, and from
/// 46 to 90 + extra time in the second half. Includes the
/// "extra time" added by the referee at the end of each
/// half on account of injuries/delays.
///
int minute;

/// "Net" game minute.
///
/// From 1 to 45 in the first half, from 46 to 90 in the
/// second half - used for game / player statistics.
///
int formal_minute;


string minute_str()
{
    return format_str("%2d", minute);
}


string formal_minute_str()
{
    return format_str("%2d", formal_minute);
}


/// Calculates how much injury time to add.
///
/// Takes into account substitutions, injuries and fouls (by both teams)
///
int how_much_inj_time(double &substitutions, double &injuries, double &fouls)
{
    // Each time this function is called, it subtracts the last
    // totals it had, because the stats accumulate and don't
    // annulize between halves
    //

    substitutions = team[0].substitutions + team[1].substitutions - substitutions;
    injuries = team[0].injuries + team[1].injuries - injuries;
    fouls = team[0].finalfouls + team[1].finalfouls - fouls;

    double calc = ceil(substitutions * 0.5 + injuries * 0.5 + fouls * 0.5);

    return int(calc);
}


// Reads the conditionals from both teamsheets
//
void read_conditionals(Handle<Object> home_teamsheet, Handle<Object> away_teamsheet)
{
    HandleScope scope;

    Handle<Object> teamsheet;
    Handle<Value> condElement;
    Handle<Array> conditions;
    Handle<Value> condition;
    Handle<String> keyCond = String::NewSymbol("conditions");
    // For each team
    //
    for (int team_num = 0; team_num <= 1; ++team_num)
    {
        if (0 == team_num) teamsheet = home_teamsheet;
        else teamsheet = away_teamsheet;

        // Keep track of line number, for error messages
        int line_num = 1;

        // Process all lines in the teamsheet until the end (we're already
        // positioned after the PK: line, so we're ready to read conditionals)
        //
        condElement = teamsheet->Get(keyCond);
        if (condElement->IsUndefined() || condElement->IsNull()) continue;

        conditions = Handle<Array>::Cast(condElement);
        for (int i=0, l=conditions->Length(); i<l; ++i)
        {
            condition = conditions->Get(i);
            if (condition->IsUndefined()) continue;

            cond* cnd = new cond;
            string msg = cnd->create(team_num, toAscii(condition->ToString(), buf));
            if (msg != "")
                cond_error(team_num, line_num, msg);

            team[team_num].conds.push_back(cnd);

            ++line_num;
        }
    }
}



Handle<String> init_teams_data(Handle<Object> home_teamsheet, Handle<Object> away_teamsheet, Handle<Array> home_roster, Handle<Array> away_roster)
{
    HandleScope scope;

    Handle<Object> teamsheet;
    Handle<Array> roster;
    Handle<Object> player;
    Handle<Array> selection;
    Handle<Object> namepos;
    Handle<String> pkname;
    Handle<String> playername;
    const Handle<String> KEY_TEAM_NAME = String::NewSymbol("teamName");
    const Handle<String> KEY_TACTIC = String::NewSymbol("tactic");
    const Handle<String> KEY_PK = String::NewSymbol("PK");
    const Handle<String> KEY_SELECTION = String::NewSymbol("Selection");
    const Handle<String> KEY_POS = String::NewSymbol("pos");
    const Handle<String> KEY_NAME = String::NewSymbol("name");
    const Handle<String> KEY_INJURY = String::NewSymbol("injury");
    const Handle<String> KEY_SUSPENSION = String::NewSymbol("suspension");
    const Handle<String> KEY_PREF_SIDE = String::NewSymbol("pref_side");
    const Handle<String> KEY_TK = String::NewSymbol("tk");
    const Handle<String> KEY_ST = String::NewSymbol("st");
    const Handle<String> KEY_PS = String::NewSymbol("ps");
    const Handle<String> KEY_SH = String::NewSymbol("sh");
    const Handle<String> KEY_STAMINA = String::NewSymbol("stamina");
    const Handle<String> KEY_AG = String::NewSymbol("ag");
    const Handle<String> KEY_FITNESS = String::NewSymbol("fitness");

    int i, im, j, l, k, km, found;

    for (l = 0; l < 2; l++)
    {
        if (l == 0){
            teamsheet = home_teamsheet;
            roster = home_roster;
        }else{
            teamsheet = away_teamsheet;
            roster = away_roster;
        }

        toAscii(teamsheet->Get(KEY_TEAM_NAME)->ToString(), team[l].name);
        toAscii(teamsheet->Get(KEY_TEAM_NAME)->ToString(), team[l].fullname);
        toAscii(teamsheet->Get(KEY_TACTIC)->ToString(), team[l].tactic);

        if (!tact_manager().tactic_exists(string(team[l].tactic)))
            return scope.Close(String::New("Invalid tactic in teamsheet"));

		// There's an optional "PK: <Name>" line.
		// If it exists, the <Name> must be listed in the teamsheet.
		//
		team[l].penalty_taker = -1;
        pkname = teamsheet->Get(KEY_PK)->ToString();

        selection = Handle<Array>::Cast(teamsheet->Get(KEY_SELECTION));

        for (i = 1, im = selection->Length(); i < im; i++)
        {
            namepos = selection->Get(i)->ToObject();
            /* Read players's position and name */
            playername = namepos->Get(KEY_NAME)->ToString();
            if (playername->StrictEquals(pkname)){
			    team[l].penalty_taker = i;
            }

            toAscii(playername, team[l].player[i].name);
            toAscii(namepos->Get(KEY_POS)->ToString(), buf);

            // For GKs, just copy the position as is
            //
            if (!strcmp(buf, "GK"))
                strncpy(team[l].player[i].pos, "GK", 2);
            else
            {
                if (!is_legal_position(string(buf)))
                    return scope.Close(String::New("Illegal position in teamsheet"));

                strncpy(team[l].player[i].pos, fullpos2position(buf).c_str(), 2);
                team[l].player[i].side = fullpos2side(buf);
            }


            /* The first specified player must be a GK */
            if (i == 1 && strcmp(team[l].player[i].pos, "GK"))
                return scope.Close(String::New("The first player in teamsheet must be a GK"));

            if (!strcmp(team[l].player[i].pos, "PK"))
                return scope.Close(String::New("PK: where player was expected"));

            found = 0;
            j = 1;

			// Search for this player in the roster, and when found assign his info
			// to the player structure.
			//
			for (k=0, km=roster->Length(); k<km; ++k)
            {
                player = roster->Get(k)->ToObject();
                toAscii(player->Get(KEY_NAME)->ToString(), buf);
				if (strcmp(team[l].player[i].name, buf))
					continue;

				found = 1;

				// Check if the player is available for the game
				//
				if (player->Get(KEY_INJURY)->IntegerValue() > 0)
					return scope.Close(String::New("Player is injured"));

				if (player->Get(KEY_SUSPENSION)->IntegerValue() > 0)
					return scope.Close(String::New("Player is suspended"));

                toAscii(player->Get(KEY_PREF_SIDE)->ToString(), team[l].player[i].pref_side);

				team[l].player[i].likes_left = false;
				team[l].player[i].likes_right = false;
				team[l].player[i].likes_center = false;

				if (strchr(team[l].player[i].pref_side, 'L'))
					team[l].player[i].likes_left = true;

				if (strchr(team[l].player[i].pref_side, 'R'))
					team[l].player[i].likes_right = true;

				if (strchr(team[l].player[i].pref_side, 'C'))
					team[l].player[i].likes_center = true;

				team[l].player[i].st = player->Get(KEY_ST)->IntegerValue();
				team[l].player[i].tk = player->Get(KEY_TK)->IntegerValue();
				team[l].player[i].ps = player->Get(KEY_PS)->IntegerValue();
				team[l].player[i].sh = player->Get(KEY_SH)->IntegerValue();
				team[l].player[i].stamina = player->Get(KEY_STAMINA)->IntegerValue();

				// Each player has a nominal_fatigue_per_minute rating that's
				// calculated once, based on his stamina.
				//
				// I'd like the average rating be 0.031 - so that an average player
				// (stamina = 50) will lose 30 fitness points during a full game.
				//
				// The range is approximately 50 - 10 points, and the stamina range
				// is 1-99. So, first the ratio is normalized and then subtracted
				// from the average 0.031 (which, times 90 minutes, is 0.279).
				// The formula for each player is:
				//
				// fatigue            stamina - 50
				// ------- = 0.0031 - ------------  * 0.0022
				//  minute                 50
				//
				//
				// This gives (approximately) 30 lost fitness points for average players,
				// 50 for the worse stamina and 10 for the best stamina.
				//
				// A small random factor is added each minute, so the exact numbers are
				// not deterministic.
				//
				double normalized_stamina_ratio = double(team[l].player[i].stamina - 50) / 50.0;
				team[l].player[i].nominal_fatigue_per_minute = 0.0031 - normalized_stamina_ratio * 0.0022;

				team[l].player[i].ag = player->Get(KEY_AG)->IntegerValue();
				team[l].player[i].fatigue = player->Get(KEY_FITNESS)->IntegerValue() / 100.0;
            }

            if (!found)
                return scope.Close(String::New("Player doesn't exist in the roster file"));
        }
    }

    ensure_no_duplicate_names();

    read_conditionals(home_teamsheet, away_teamsheet);

    // Set active flags
    for (j = 0; j <= 1; j++)
    {
        team[j].substitutions = 0;
        team[j].injuries = 0;

        for (i=1; i <= num_players; i++)
        {
            if (i <= 11)
                team[j].player[i].active = 1;
            else
                team[j].player[i].active = 2;
        }
    }

    /* In the beginning, player n.1 is always the GK */
    team[0].current_gk = team[1].current_gk = 1;

    /* Data initialization */
    for (j = 0; j <= 1; j++)
    {
        team[j].score = team[j].finalshots_on = team[j].finalshots_off = 0;
        team[j].finalfouls = 0;
        team[j].team_tackling=team[j].team_passing=team[j].team_shooting = 0;

        for (i = 1; i <= num_players; i++)
        {
            team[j].player[i].tk_contrib = team[j].player[i].ps_contrib =
                                               team[j].player[i].sh_contrib = 0;

            team[j].player[i].yellowcards = 0;
            team[j].player[i].redcards = 0;
            team[j].player[i].injured = 0;
            team[j].player[i].tk_ab = 0;
            team[j].player[i].ps_ab = 0;
            team[j].player[i].sh_ab = 0;
            team[j].player[i].st_ab = 0;

            // final stats initialization
            team[j].player[i].minutes = team[j].player[i].shots = 0;
            team[j].player[i].goals = team[j].player[i].saves = 0;
            team[j].player[i].assists = team[j].player[i].tackles = 0;
            team[j].player[i].keypasses = team[j].player[i].fouls = 0;
            team[j].player[i].redcards = team[j].player[i].yellowcards = 0;
            team[j].player[i].conceded = team[j].player[i].shots_on = 0;
            team[j].player[i].shots_off = 0;
        }
    }

    return scope.Close(String::New(""));
}



/// Goes over both teams and checks that there are no duplicate player
/// names. If there are, exits with an error.
///
void ensure_no_duplicate_names(void)
{
    int i, j, k;

    for (j = 0; j <= 1; j++)
        for (i = 1; i <= num_players; i++)
            for (k = 1; k <= num_players; k++)
            {
                if (k != i && !strcmp(team[j].player[i].name, team[j].player[k].name))
                    die("Player %s (%s) is named twice in the team sheet",
                        team[j].player[i].name, team[j].name);
            }
}


/// Prints the starting tactics & formation
/// of each team to the commentary file.
///
void print_starting_tactics(Handle<Object> output)
{
    HandleScope scope;

    int i, j;
    teams *t;
    playerstruct *ps;

    Handle<Array> teams = Array::New();
    Handle<Object> teamInfo;
    Handle<Object> selection;
    Handle<Array> gkList;
    Handle<Array> dfList;
    Handle<Array> mfList;
    Handle<Array> fwList;
    Handle<String> keyName = String::NewSymbol("name");
    Handle<String> keySelection = String::NewSymbol("selection");
    Handle<String> keyTactic = String::NewSymbol("tactic");
    Handle<String> keyGk = String::NewSymbol("gk");
    Handle<String> keyDf = String::NewSymbol("df");
    Handle<String> keyMf = String::NewSymbol("mf");
    Handle<String> keyFw = String::NewSymbol("fw");

    /* Initialize formation counters */

    // For each team, count the amount of players on each
    // position
    //
    for (j = 0; j <= 1; j++)
    {
        teamInfo = Object::New();
        selection = Object::New();
        gkList = Array::New();
        dfList = Array::New();
        mfList = Array::New();
        fwList = Array::New();

        t = &(team[j]);
        teamInfo->Set(keyName, String::New(t->fullname));

        for (i = 1; i <= 11; i++)
        {
            ps = &(t->player[i]);

            if (!strcmp(ps->pos, "GK")){
                gkList->Set(gkList->Length(), String::New(ps->name));
            }else if (!strcmp(ps->pos, "DF")){
                dfList->Set(dfList->Length(), String::New(ps->name));
            }else if (!strcmp(ps->pos, "DM")){
                mfList->Set(mfList->Length(), String::New(ps->name));
            }else if (!strcmp(ps->pos, "MF")){
                mfList->Set(mfList->Length(), String::New(ps->name));
            }else if (!strcmp(ps->pos, "AM")){
                mfList->Set(mfList->Length(), String::New(ps->name));
            }else if (!strcmp(ps->pos, "FW")){
                fwList->Set(fwList->Length(), String::New(ps->name));
            }
        }
        selection->Set(keyGk, gkList);
        selection->Set(keyDf, dfList);
        selection->Set(keyMf, mfList);
        selection->Set(keyFw, fwList);
        teamInfo->Set(keySelection, selection);

        teamInfo->Set(keyTactic, String::New(tact_manager().get_tactic_full_name(t->tactic).c_str()));

        teams->Set(j, teamInfo);
    }
    output->Set(String::New("teamInfo"), teams);
}


void change_tactic(Handle<Array> commentary, int a, const char* newtct)
{
    if (strcmp(newtct, team[a].tactic))
    {
        strcpy(team[a].tactic, newtct);
    
        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("CHANGETACTIC", 
                    minute_str().c_str(),
                    team[a].name, team[a].name,
                    team[a].tactic).c_str()));
    }
}


// Substitutite player in for player out in team a, he'll play
// position newpos
//
void substitute_player(Handle<Array> commentary, int max_substitutions, int a, int out, int in, string newpos)
{
    if (team[a].player[out].active == 1 && team[a].player[in].active == 2
            && team[a].substitutions < max_substitutions)
    {
        team[a].player[out].active = 0;
        team[a].player[in].active = 1;

        if (newpos == "GK")
            strncpy(team[a].player[in].pos, "GK", 2);
        else
        {
            strncpy(team[a].player[in].pos, fullpos2position(newpos).c_str(), 2);
            team[a].player[in].side = fullpos2side(newpos);
        }

        if (out == team[a].current_gk)
            team[a].current_gk = in;

        team[a].substitutions++;

        sprintf(buf, "%s", the_commentary().rand_comment("SUB", minute_str().c_str(), team[a].name,
                team[a].player[in].name,
                team[a].player[out].name,
                newpos.c_str()).c_str());
        commentary->Set(commentary->Length(), String::New(buf));
    }
}


void change_position(Handle<Array> commentary, int a, int b, string newpos)
{
    // Can't reposition a GK or an inactive player
    if (b != team[a].current_gk && team[a].player[b].active == 1)
    {
        // If he plays on this position anyway, don't change it
        if (pos_and_side2fullpos(team[a].player[b].pos, team[a].player[b].side) != newpos)
        {
            sprintf(buf, "%s", the_commentary().rand_comment("CHANGEPOSITION", minute_str().c_str(),
                    team[a].name,
                    team[a].player[b].name,
                    newpos.c_str()).c_str());
            commentary->Set(commentary->Length(), String::New(buf));

            strncpy(team[a].player[b].pos, fullpos2position(newpos).c_str(), 2);
            team[a].player[b].side = fullpos2side(newpos);
        }
    }
}


/* This function controls the random injuries occurance. */
/* The CHANCE of a player to get injured depends on a    */
/* constant factor + total aggression of the rival team. */
/* The function will find who was injured and substitute */
/* him for player on his position.                       */
void random_injury(Handle<Array> commentary, int max_substitutions, int a)
{
    int injured, b, found = 0;

    if (randomp((1500 + team[!a].aggression)/50)) /* If someone got injured */
    {
        ++team[a].injuries;

        do        /* The inj_player can't be n.0 and must be playing */
        {
            injured = my_random(num_players + 1);
        }
        while (injured == 0 || team[a].player[injured].active != 1);

        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("INJURY", minute_str().c_str(), team[a].name,
                    team[a].player[injured].name).c_str()));

/*        report_event* an_event = new report_event_injury(team[a].player[injured].name,
                                 team[a].name, formal_minute_str().c_str());
        report_vec.push_back(an_event);
*/
        injured_ind[a] = injured;

        /* Only 3 substitutions are allowed per team per game */
        if (team[a].substitutions >= 3) /* No substitutions left */
        {
            team[a].player[injured].active = 0;
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("NOSUBSLEFT").c_str()));

            if (!strcmp(team[a].player[injured].pos, "GK"))
            {
                int n = 11;

                while(team[a].player[n].active != 1)  /* Sub him for another player */
                    n--;

                change_position(commentary, a, n, string("GK"));
                team[a].current_gk = n;
            }
        }
        else
        {
            b = 12;

            while (!found && b <= num_players) /* Look for subs on the same position */
            {
                if (!strcmp(team[a].player[injured].pos, team[a].player[b].pos)
                        && team[a].player[b].active == 2)
                {
                    substitute_player(commentary, max_substitutions, a, injured, b,
                                      pos_and_side2fullpos(team[a].player[injured].pos, team[a].player[injured].side));

                    if (injured == team[a].current_gk)
                        team[a].current_gk = b;

                    found = 1;
                }
                else
                    b++;
            }

            if (!found)          /* If there are no subs on his position */
            {
                /* Then, sub him for any other player on the bench who is not a   */
                /* goalkeeper. If a GK will be injured, he will be subbed for the */
                /* GK on the bench by the previous loop, if there won't be any    */
                /* GK on the bench, he will be subbed for another player          */
                b = 12;

                while (!found && b <= num_players)
                {

                    if (strcmp(team[a].player[b].pos, "GK")
                            && team[a].player[b].active == 2)
                    {
                        substitute_player(commentary, max_substitutions, a, injured, b,
                                          pos_and_side2fullpos(team[a].player[injured].pos, team[a].player[injured].side));
                        found = 1;

                        if (injured == team[a].current_gk)
                            team[a].current_gk = b;
                    }
                    else
                        b++;
                } // while (!found && b <= num_players)
            } // if (!found)
        } // if (team[a].substitutions >= 3)

        team[a].player[injured].injured = 1;
        team[a].player[injured].active = 0;

    } // if (randomp((1500 + team[!a].aggression)/50))
}


// Calculate the contributions of player b of team a
//
void calc_player_contributions(int a, int b)
{
    if (team[a].player[b].active == 1 && team[a].current_gk != b)
    {
        double tk_mult = tact_manager().get_mult(team[a].tactic, team[!a].tactic,
                         team[a].player[b].pos, "TK");
        double ps_mult = tact_manager().get_mult(team[a].tactic, team[!a].tactic,
                         team[a].player[b].pos, "PS");
        double sh_mult = tact_manager().get_mult(team[a].tactic, team[!a].tactic,
                         team[a].player[b].pos, "SH");

        double side_factor;

        if ((team[a].player[b].side == 'R' && team[a].player[b].likes_right) ||
                (team[a].player[b].side == 'L' && team[a].player[b].likes_left) ||
                (team[a].player[b].side == 'C' && team[a].player[b].likes_center))
        {
            side_factor = 1.0;
        }
        else
        {
            side_factor = 0.75;
        }

        team[a].player[b].tk_contrib = tk_mult * side_factor * team[a].player[b].tk * team[a].player[b].fatigue;
        team[a].player[b].ps_contrib = ps_mult * side_factor * team[a].player[b].ps * team[a].player[b].fatigue;
        team[a].player[b].sh_contrib = sh_mult * side_factor * team[a].player[b].sh * team[a].player[b].fatigue;
    }
    // The contributions of an inactive player or of a GK are 0
    //
    else
    {
        team[a].player[b].tk_contrib = 0;
        team[a].player[b].ps_contrib = 0;
        team[a].player[b].sh_contrib = 0;
    }
}


// Adjusts players' total contributions, taking into account the
// side balance on each position
//
void adjust_contrib_with_side_balance(int a)
{
    // The side balance:
    // For each position (w/o side), keep a vector of 3 elements
    // to specify the number of players playing R [0], L [1], C [2] on this position
    //
    map<string, vector<int> > balance;

    // Init the side balance for all positions
    //
    const vector<string>& positions = tact_manager().get_positions_names();
    for (vector<string>::const_iterator pos = positions.begin(); pos != positions.end(); ++pos)
    {
        vector<int> v(3, 0);
        balance[*pos] = v;
    }

    // Go over the team's players and record on what side they play,
    // updating the side balance
    //
    for (int b = 2; b <= num_players; b++)
    {
        if (team[a].player[b].active == 1 && strcmp(team[a].player[b].pos, "GK"))
        {
            if (team[a].player[b].side == 'R')
                balance[string(team[a].player[b].pos)][0]++;
            else if (team[a].player[b].side == 'L')
                balance[string(team[a].player[b].pos)][1]++;
            else if (team[a].player[b].side == 'C')
                balance[string(team[a].player[b].pos)][2]++;
            else
                assert(0);
        }
    }

    // For all positions, check if the side balance is equal for R and L
    // If it isn't, penalize the contributions of the players on those positions
    //
    // Additionally, penalize teams who play with more than 3 C players on
    // some position without R and L
    //
    for (vector<string>::const_iterator pos = positions.begin(); pos != positions.end(); ++pos)
    {
        int on_pos_right = balance[*pos][0];
        int on_pos_left = balance[*pos][1];
        int on_pos_center = balance[*pos][2];

        double taxed_multiplier = 1;

        if (on_pos_left != on_pos_right)
        {
            double tax_ratio = 0.25 * double(abs(on_pos_right - on_pos_left)) / (on_pos_right + on_pos_left);
            taxed_multiplier = 1 - tax_ratio;
        }
        else if (on_pos_left == 0 && on_pos_right == 0 && on_pos_center > 3)
        {
            taxed_multiplier = 0.87;
        }

        if (taxed_multiplier != 1)
            for (int b = 2; b <= num_players; b++)
            {
                if (team[a].player[b].active == 1 && !strcmp(team[a].player[b].pos, pos->c_str()))
                {
                    team[a].player[b].tk_contrib *= taxed_multiplier;
                    team[a].player[b].ps_contrib *= taxed_multiplier;
                    team[a].player[b].sh_contrib *= taxed_multiplier;
                }
            }
    }
}


void calc_shotprob(int a)
{
    // Note: 1.0 is added to tackling, to avoid singularity when the
    // team tackling is 0
    //
    team[a].shot_prob = (double)1.8*(team[a].aggression/50.0 + 800.0 *
                                     (double) pow(((1.0/3.0*team[a].team_shooting + 2.0/3.0*team[a].team_passing)
                                                   / (team[!(a)].team_tackling + 1.0)), 2));

    // If it is the home team, add home bonus
    //
    if (a == 0)
        team[a].shot_prob += home_bonus;
}


// This function is called by the game running loop in the
// beginning of each minute of the game.
// It recalculates player contributions, aggression, fatigue,
// team total contributions and shotprob.
//
void recalculate_teams_data(void)
{
    int a, b;

    for(a = 0; a <= 1; a++)
    {
        team[a].team_tackling = team[a].team_passing=team[a].team_shooting = 0;
        calc_aggression(a);

        for (b = 2; b <= num_players; b++)
            if (team[a].player[b].active == 1)
            {
                double fatigue_deduction = team[a].player[b].nominal_fatigue_per_minute;
                int mrnd = my_random(100);
                fatigue_deduction += double(mrnd - 50) / 50.0 * 0.003;

                team[a].player[b].fatigue -= fatigue_deduction;

                if (team[a].player[b].fatigue < 0.10)
                    team[a].player[b].fatigue = 0.10;
            }

        for (b = 2; b <= num_players; b++)
            calc_player_contributions(a, b);

        adjust_contrib_with_side_balance(a);
        calc_team_contributions_total(a);
    }

    for (a = 0; a <= 1; a++)
        calc_shotprob(a);
}


void calc_team_contributions_total(int a)
{
    for (int b = 2; b <= num_players; b++)
        if (team[a].player[b].active == 1)
        {
            team[a].team_tackling += team[a].player[b].tk_contrib;
            team[a].team_passing  += team[a].player[b].ps_contrib;
            team[a].team_shooting += team[a].player[b].sh_contrib;
        }
}


// This function sets the aggression of all inactive players to 0
// and then adds up all aggressions in the team total aggression
void calc_aggression(int a)
{
    team[a].aggression = 0;

    for (int i = 1;i <= num_players; ++i)
    {
        if (team[a].player[i].active != 1)
            team[a].player[i].ag = 0;

        team[a].aggression += team[a].player[i].ag;
    }
}


int is_goal_cancelled(Handle<Array> commentary)
{
    if (randomp(500))
    {
        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("GOALCANCELLED").c_str()));
        return 1;
    }

    return 0;
}

// Called on each minute to handle a scoring chance of team
// a for this minute.
//
void if_shot(Handle<Array> commentary, int a)
{
    int shooter, assister, tackler;
    int chance_tackled;
    int chance_assisted = 0;

    // Did a scoring chance occur ?
    //
    if (randomp((int) team[a].shot_prob))
    {
        // There's a 0.75 probability that a chance was assisted, and
        // 0.25 that it's a solo
        //
        if (randomp(7500))
        {
            assister = who_did_it(a, DID_ASSIST);
            chance_assisted = 1;

            shooter = who_got_assist(a, assister);

            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("ASSISTEDCHANCE",
                    minute_str().c_str(),
                    team[a].name, team[a].player[assister].name,
                    team[a].player[shooter].name).c_str()));
            team[a].player[assister].keypasses++;
        }
        else
        {
            shooter = who_did_it(a, DID_SHOT);

            chance_assisted = 0;
            assister = 0;
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("CHANCE", minute_str().c_str(), team[a].name,
                    team[a].player[shooter].name).c_str()));
        }

        chance_tackled = (int) (4000.0*((team[!a].team_tackling*3.0)/(team[a].team_passing*2.0+team[a].team_shooting)));

        /* If the chance was tackled */
        if (randomp(chance_tackled))
        {
            tackler = who_did_it(!a, DID_TACKLE);
            team[!a].player[tackler].tackles++;

            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("TACKLE", team[!a].player[tackler].name).c_str()));
        }
        else /* Chance was not tackled, it will be a shot on goal */
        {
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("SHOT", team[a].player[shooter].name).c_str()));
            team[a].player[shooter].shots++;

            if (if_ontarget(a, shooter))
            {
                team[a].finalshots_on++;
                team[a].player[shooter].shots_on++;

                if (if_goal(a, shooter))
                {
                    commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("GOAL").c_str()));

                    if (!is_goal_cancelled(commentary))
                    {
                        team[a].score++;

                        // If the assister was the shooter, there was no
                        // assist, but a simple goal.
                        //
                        if (chance_assisted && (assister != shooter))
                            team[a].player[assister].assists++; /* For final stats */

                        team[a].player[shooter].goals++;
                        team[!a].player[team[!a].current_gk].conceded++;

                        sprintf(buf, "\n          ...  %s %d-%d %s ...",
                                team[0].name,
                                team[0].score,
                                team[1].score,
                                team[1].name);
                        commentary->Set(commentary->Length(), String::New(buf));

/*                        report_event* an_event = new report_event_goal(team[a].player[shooter].name,
                                                 team[a].name, formal_minute_str().c_str());

                        report_vec.push_back(an_event);*/
                    }
                }
                else
                {
                    commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("SAVE",
                            team[!a].player[team[!a].current_gk].name).c_str()));
                    team[!a].player[team[!a].current_gk].saves++;
                }
            }
            else
            {
                team[a].player[shooter].shots_off++;
                commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("OFFTARGET").c_str()));
                team[a].finalshots_off++;
            }
        }
    }
}


// Given a team and an event (eg. SHOT)
// picks one player at (weighted) random
// that performed this event.
//
// For example, for SHOT, pick a player
// at weighted random according to his
// shooting skill
//
int who_did_it(int a, DID_WHAT event)
{
    int k = 0;
    double total = 0, weight = 0;
    double* ar = new double[num_players + 1];

    // Employs the weighted random algorithm
    // A player's chance to DO_IT is his
    // contribution relative to the team's total
    // contribution
    //

    for (k = 1; k <= num_players; ++k)
    {
        switch(event)
        {
        case DID_SHOT:
            weight += team[a].player[k].sh_contrib * 100.0;
            total = team[a].team_shooting * 100.0;
            break;
        case DID_FOUL:
            weight += team[a].player[k].ag;
            total = team[a].aggression;
            break;
        case DID_TACKLE:
            weight += team[a].player[k].tk_contrib * 100.0;
            total = team[a].team_tackling * 100.0;
            break;
        case DID_ASSIST:
            weight += team[a].player[k].ps_contrib * 100.0;
            total = team[a].team_passing * 100.0;
            break;
        default:
            // TODO: handle internal error
            return 0;
        }

        ar[k] = weight;
    }

    unsigned rand_value = my_random((int) total);

    for (k = 2; ar[k] <= rand_value; ++k)
        if (k == num_players)
        {
            // TODO: handle internal error
            return 0;
        }

    delete[] ar;

    return k;
}


// When a chance was generated for the team and assisted by the
// assister, who got the assist ?
//
// This is almost like who_did_it, but it also takes
// into account the side of the assister - a player on his side
// has a higher chance to get the assist.
//
// How it's done: if the side of the shooter (picked by who_did_it)
// is different from the side of the asssiter, who_did_it is run
// once again - but this happens only once. This increases the
// chance of the player on the same side to be picked, but leaves
// a possibility for other sides as well.
//
int who_got_assist(int a, int assister)
{
    int shooter = assister;

    // Shooter and assister must be different, so re-run each time the same
    // one is generated
    //
    while (shooter == assister)
    {
        shooter = who_did_it(a, DID_SHOT);

        // if the side is different, re-run once
        //
        if (team[a].player[shooter].side != team[a].player[assister].side)
        {
            shooter = who_did_it(a, DID_SHOT);
        }
    }

    return shooter;
}


/* Whether the shot is on target. */
int if_ontarget(int a, int b)
{
    if (randomp((int) (5800.0*team[a].player[b].fatigue)))
        return 1;
    else
        return 0;
}


// Given a shot on target (team a shot on team b's goal),
// was it a goal ?
//
int if_goal(int a, int b)
{
    // Factors taken into account:
    // The shooter's Sh and fatigue against the GK's St
    //
    // The "median" is 0.35
    // Lower and upper bounds are 0.1 and 0.9 respectively
    //
    double temp = team[a].player[b].sh*team[a].player[b].fatigue*200 -
                  team[!a].player[team[!a].current_gk].st*200 + 3500;

    if (temp > 9000)
        temp = 9000;
    if (temp < 1000)
        temp = 1000;

    if (randomp((int)temp))
        return 1;
    else
        return 0;
}



void send_off(Handle<Array> commentary, int max_substitutions, int a, int b)
{
    team[a].player[b].yellowcards = 0;
    team[a].player[b].redcards++;
    team[a].player[b].active = 0;

    if (team[a].current_gk == b)  /* If a GK was sent off */
    {
        int i = 12, found = 0;

        if (team[a].substitutions < 3)
        {
            while (!found && i <= num_players)  /* Look for a keeper on the bench */
            {
                /* If found a keeper */
                if (!strcmp(team[a].player[i].pos, "GK") && team[a].player[i].active == 2)
                {
                    int n = 11;

                    found = 1;

                    while(team[a].player[n].active != 1)  /* Sub him for another player */
                        n--;
                    substitute_player(commentary, max_substitutions, a, n, i, "GK");
                    team[a].current_gk = i;
                }
                else
                {
                    found = 0;
                    i++;
                }
            }

            if (!found)         /*  If there was no keeper on the bench   */
            {                   /*  Change the position of another player */
                int n = 11;       /*  (who is on the field) to GK           */

                while(team[a].player[n].active != 1)
                    n--;

                change_position(commentary, a, n, string("GK"));
                team[a].current_gk = n;
            }
        }
        else      /* If substitutions >= 3 */
        {
            int n = 11;

            while(team[a].player[n].active != 1)
                n--;
            change_position(commentary, a, n, string("GK"));
            team[a].current_gk = n;
        }
    }
}


// Deals with yellow and red cards
//
void bookings(Handle<Array> commentary, int max_substitutions, int a, int b, int card_color)
{
    if (card_color == YELLOW)
    {
        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("YELLOWCARD").c_str()));
        team[a].player[b].yellowcards++;

        // A second yellow card is equal to a red card
        //
        if (team[a].player[b].yellowcards == 2)
        {
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("SECONDYELLOWCARD").c_str()));
            send_off(commentary, max_substitutions, a, b);

/*            report_event* an_event = new report_event_red_card(team[a].player[b].name,
                                     team[a].name, formal_minute_str().c_str());
            report_vec.push_back(an_event);*/

            red_carded[a] = b;
        }
        else
            yellow_carded[a] = b;
    }
    else if (card_color == RED)
    {
        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("REDCARD").c_str()));
        send_off(commentary, max_substitutions, a, b);

/*        report_event* an_event = new report_event_red_card(team[a].player[b].name,
                                 team[a].name, formal_minute_str().c_str());
        report_vec.push_back(an_event);*/

        red_carded[a] = b;
    }
}

// Handle fouls (called on each minute with for each team)
//
void if_foul(Handle<Array> commentary, int max_substitutions, int a)
{
    int fouler;

    if (randomp((int)team[a].aggression*3/4))
    {
        fouler = who_did_it(a, DID_FOUL);
        commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("FOUL", minute_str().c_str(), team[a].name,
                team[a].player[fouler].name).c_str()));

        team[a].finalfouls++;         /* For final stats */
        team[a].player[fouler].fouls++;

        /* The chance of the foul to result in a yellow or red card */
        if (randomp(6000))
            bookings(commentary, max_substitutions, a, fouler, YELLOW);
        else if (randomp(400))
            bookings(commentary, max_substitutions, a, fouler, RED);
        else
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("WARNED").c_str()));

        /* Condition for a penalty to occur (if GK fouled, or random) */
        if ((fouler == team[a].current_gk) || (randomp(500)))
        {
            // If the nominated PK taker isn't active, choose the
            // best shooter to take the PK
            //
            if (team[!a].player[team[!a].penalty_taker].active != 1 || team[!a].penalty_taker == -1)
            {
                double max = -1;
                int max_index = 1;

                for (int i = 1; i <= num_players; ++i)
                {
                    if (team[!a].player[i].active == 1 && team[!a].player[i].sh * team[!a].player[i].fatigue > max)
                    {
                        max = team[!a].player[i].sh * team[!a].player[i].fatigue;
                        max_index = i;
                    }
                }

                team[!a].penalty_taker = max_index;
            }

            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("PENALTY",
                    team[!a].player[team[!a].penalty_taker].name).c_str()));

            /* If Penalty... Goal ? */
            if (randomp(8000 + team[!a].player[team[!a].penalty_taker].sh*100 -
                        team[a].player[team[a].current_gk].st*100))
            {
                commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("GOAL").c_str()));
                team[!a].score++;
                team[!a].player[team[!a].penalty_taker].goals++;
                team[a].player[team[a].current_gk].conceded++;
                sprintf(buf, "\n          ...  %s %d-%d %s...", team[0].name, team[0].score,
                        team[1].score,  team[1].name);
                commentary->Set(commentary->Length(), String::New(buf));

/*                report_event* an_event = new report_event_penalty(team[!a].player[team[!a].penalty_taker].name,
                                         team[!a].name, formal_minute_str().c_str());
                report_vec.push_back(an_event);*/

            }
            else /* If the penalty taker didn't score */
            {
                // Either it was saved, or it went off-target
                //
                if (randomp(7500))
                    commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("SAVE",
                            team[a].player[team[a].current_gk].name).c_str()));
                else  /* Or it went off-target */
                    commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("OFFTARGET").c_str()));
            }
        }
    }
}



/* This function uses the constants contained in league.dat */
/* to calculate the ability change of each player.          */
void calc_ability(const ObjectV8 &ab)
{
    int i, j;

    // Initialization of ab bonuses
    //
    int ab_goal       = ab.get("AB_Goal", 0);
    int ab_assist     = ab.get("AB_Assist", 0);
    int ab_victory    = ab.get("AB_Victory_Random", 0);
    int ab_defeat     = ab.get("AB_Defeat_Random", 0);
    int ab_cleansheet = ab.get("AB_Clean_Sheet", 0);
    int ab_ktk        = ab.get("AB_KTK", 0);
    int ab_kps        = ab.get("AB_KPS", 0);
    int ab_sht_on     = ab.get("AB_Sht_On", 0);
    int ab_sht_off    = ab.get("AB_Sht_Off", 0);
    int ab_sav        = ab.get("AB_Sav", 0);
    int ab_concede    = ab.get("AB_Concede", 0);
    int ab_yellow     = ab.get("AB_Yellow", 0);
    int ab_red        = ab.get("AB_Red", 0);

    for (j = 0; j <= 1; ++j)
    {
        // Add simple bonuses
        //
        for (i = 1; i <= num_players; ++i)
        {
            team[j].player[i].sh_ab += ab_goal * team[j].player[i].goals;
            team[j].player[i].ps_ab += ab_assist * team[j].player[i].assists;
            team[j].player[i].tk_ab += ab_ktk * team[j].player[i].tackles;
            team[j].player[i].ps_ab += ab_kps * team[j].player[i].keypasses;
            team[j].player[i].sh_ab += ab_sht_on * team[j].player[i].shots_on;
            team[j].player[i].sh_ab += ab_sht_off * team[j].player[i].shots_off;
            team[j].player[i].st_ab += ab_sav * team[j].player[i].saves;
            team[j].player[i].st_ab += ab_concede * team[j].player[i].conceded;

            // For cards, all abilities are decreased (only St for a GK)
            //
            if (!strcmp(team[j].player[i].pos, "GK"))
            {
                team[j].player[i].st_ab += ab_yellow * team[j].player[i].yellowcards;
                team[j].player[i].st_ab += ab_red * team[j].player[i].redcards;
            }
            else
            {
                team[j].player[i].tk_ab += ab_yellow * team[j].player[i].yellowcards;
                team[j].player[i].ps_ab += ab_yellow * team[j].player[i].yellowcards;
                team[j].player[i].sh_ab += ab_yellow * team[j].player[i].yellowcards;

                team[j].player[i].tk_ab += ab_red * team[j].player[i].redcards;
                team[j].player[i].ps_ab += ab_red * team[j].player[i].redcards;
                team[j].player[i].sh_ab += ab_red * team[j].player[i].redcards;
            }
        }

        // Add random-victory bonuses
        //
        if (team[j].score > team[!j].score)
        {
            int num = 0, k, n;

            for (k = 1; k <= 2; k++)
            {
                //
                // Find a player to get the increase
                //
                do
                {
                    n = my_random(num_players) + 1;

                }
                while(!team[j].player[n].minutes || n == num);

                //
                // Decide the ability which gets the increase
                //
                if (!strcmp(team[j].player[n].pos, "GK"))
                    team[j].player[n].st_ab += ab_victory;
                else
                {
                    team[j].player[n].tk_ab += ab_victory;
                    team[j].player[n].ps_ab += ab_victory;
                    team[j].player[n].sh_ab += ab_victory;
                }

                num = n;
            }
        }

        //
        // Decrease random-defeat bonuses
        //
        if (team[j].score < team[!j].score)
        {
            int num = 0, k, n;

            for (k = 1; k <= 2; k++)
            {

                //
                // Decide the player to get the decrease
                //
                do
                {
                    n = my_random(num_players) + 1;

                }
                while(!team[j].player[n].minutes || n == num);

                //
                // Decide the ability which gets the decrease
                //
                if (!strcmp(team[j].player[n].pos, "GK"))
                    team[j].player[n].st_ab += ab_defeat;
                else
                {
                    team[j].player[n].tk_ab += ab_defeat;
                    team[j].player[n].ps_ab += ab_defeat;
                    team[j].player[n].sh_ab += ab_defeat;
                }

                num = n;
            }
        }

        //
        // Add clean sheet bonus
        //
        if (team[!j].score == 0)
        {
            int n = 0;

            do
            {
                n++;

                if (n >= num_players)
                    break;

            }
            while(team[j].player[n].minutes < 46 || (strcmp(team[j].player[n].pos,"GK")));

            if (n >= num_players)
                n = 1;

            team[j].player[n].st_ab += ab_cleansheet;

            do
            {
                n = my_random(num_players) + 1;

            }
            while(!team[j].player[n].minutes || (strcmp(team[j].player[n].pos,"DF")));

            team[j].player[n].tk_ab += ab_cleansheet;
        }
    }
}


// Prints after-game statistics into the commentary file. The shots on/off
// of each team, final score, stats (tackles, assists etc) for each player,
// etc.
//
// TODO: create finalStats instead of reuse commentary
Handle<Object> print_final_stats(Handle<Object> output)
{
    HandleScope scope;

    Handle<Array> teamInfos = Handle<Array>::Cast(output->Get(String::NewSymbol("teamInfo")));
    Handle<Object> teamInfo;
    Handle<Array> teamStatistics = Array::New();
    Handle<Array> teamTotals = Array::New();
    Handle<Array> teamProfiles = Array::New();
    Handle<String> keyShotsOffTarget = String::NewSymbol("shotsOffTarget");
    Handle<String> keyShotsOnTarget = String::NewSymbol("shotsOnTarget");
    Handle<String> keyScore = String::NewSymbol("score");

    // Print final stats for players
    // Name          Pos Prs St Tk Ps Sh Sm Min Sav Ktk Kps Ass Sht Gls Yel Red Inj KAb TAb PAb SAb Fit
    int
    i,j,
    t_saves, t_tackles, t_keypasses, t_assists, t_shots, t_goals, t_yellowcards, t_redcards, t_injured;
    Handle<Object> teamObj;
    Handle<Array> stats;
    struct teams *t;
    struct playerstruct *ps;

    for (j = 0; j <= 1; j++)
    {
        teamInfo = Handle<Object>::Cast(teamInfos->Get(j));

        // Totals
        t_saves = t_tackles = t_keypasses = t_assists = t_shots = t_goals = t_yellowcards = t_redcards = t_injured = 0;
        
        teamObj = Object::New();
        t = &team[j];

        // Print shots on/off target and final score
        teamInfo->Set(keyShotsOffTarget, Integer::New(t->finalshots_off));
        teamInfo->Set(keyShotsOnTarget, Integer::New(t->finalshots_on));
        teamInfo->Set(keyScore, Integer::New(t->score));

        // Print stats for each player and collect totals
        for (i = 1; i <= num_players; i++)
        {
            stats = Array::New();
            ps = &(t->player[i]);
            stats->Set(stats->Length(), String::New(pos_and_side2fullpos(ps->pos, ps->side).c_str())); //0
            stats->Set(stats->Length(), String::New(ps->pref_side)); //1
            stats->Set(stats->Length(), Integer::New(ps->st)); //2
            stats->Set(stats->Length(), Integer::New(ps->tk)); //3
            stats->Set(stats->Length(), Integer::New(ps->ps)); //4
            stats->Set(stats->Length(), Integer::New(ps->sh)); //5
            stats->Set(stats->Length(), Integer::New(ps->stamina)); //6
            stats->Set(stats->Length(), Integer::New(ps->minutes)); //7
            stats->Set(stats->Length(), Integer::New(ps->saves)); //8
            stats->Set(stats->Length(), Integer::New(ps->tackles)); //9
            stats->Set(stats->Length(), Integer::New(ps->keypasses)); //10
            stats->Set(stats->Length(), Integer::New(ps->assists)); //11
            stats->Set(stats->Length(), Integer::New(ps->shots)); //12
            stats->Set(stats->Length(), Integer::New(ps->goals)); //13
            stats->Set(stats->Length(), Integer::New(ps->yellowcards)); //14
            stats->Set(stats->Length(), Integer::New(ps->redcards)); //15
            stats->Set(stats->Length(), Integer::New(ps->injured)); //16
            stats->Set(stats->Length(), Integer::New(ps->st_ab)); //17
            stats->Set(stats->Length(), Integer::New(ps->tk_ab)); //18
            stats->Set(stats->Length(), Integer::New(ps->ps_ab)); //19
            stats->Set(stats->Length(), Integer::New(ps->sh_ab)); //21
            stats->Set(stats->Length(), Integer::New(int(ps->fatigue * 100.0))); //22

            teamObj->Set(String::New(ps->name), stats);

            t_saves += ps->saves;
            t_tackles += ps->tackles;
            t_keypasses += ps->keypasses;
            t_assists += ps->assists;
            t_shots += ps->shots;
            t_goals += ps->goals;
            t_injured += ps->injured;
            t_yellowcards += ps->yellowcards;
            t_redcards += ps->redcards;
        }
        teamStatistics->Set(j, teamObj);

        stats = Array::New();
        stats->Set(stats->Length(), Integer::New(t_saves));
        stats->Set(stats->Length(), Integer::New(t_tackles));
        stats->Set(stats->Length(), Integer::New(t_keypasses));
        stats->Set(stats->Length(), Integer::New(t_assists));
        stats->Set(stats->Length(), Integer::New(t_shots));
        stats->Set(stats->Length(), Integer::New(t_goals));
        stats->Set(stats->Length(), Integer::New(t_yellowcards));
        stats->Set(stats->Length(), Integer::New(t_redcards));
        stats->Set(stats->Length(), Integer::New(t_injured));
        teamTotals->Set(j, stats);
    }


    if (team_stats_total_enabled)
    {
        Handle<Array> profile;
        for (j = 0; j <= 1; j++){
            profile = Array::New();
            for (i = 0; i < 10; ++i)
            {
                // sprintf(buf, "\nTeam  Min        Tk       Ps       Sh");
                stats = Array::New();
                stats->Set(stats->Length(), Integer::New(i*10));
                stats->Set(stats->Length(), Number::New(teamStatsTotal[j][i][0]));
                stats->Set(stats->Length(), Number::New(teamStatsTotal[j][i][1]));
                stats->Set(stats->Length(), Number::New(teamStatsTotal[j][i][2]));
                profile->Set(i, stats);
            }
            teamProfiles->Set(j, profile);
        }
    }

    output->Set(String::New("teamStatistics"), teamStatistics);
    output->Set(String::New("teamTotals"), teamTotals);
    output->Set(String::New("teamProfiles"), teamProfiles);

    return scope.Close(output);
}

/*
// Updates the game reports file with info
// about the current game
//
void update_reports_file(string work_dir)
{
    FILE *reportsfile;

    string reports_filename = work_dir + "reports.txt";

    reportsfile = fopen(reports_filename.c_str(), "a");

    if (reportsfile == NULL)
        die("Can't open reports.txt: %s", strerror(errno));


    // Add the game score
    //
    fprintf(reportsfile, "\n%s %d - %d %s\n", team[0].fullname, team[0].score,
            team[1].score, team[1].fullname);

    // Add info about the goals scored
    //
    for (unsigned i = 0; i < report_vec.size(); i++)
    {
        string line = report_vec[i]->get_event();

        fprintf(reportsfile, "%s", line.c_str());
    }

    fprintf(reportsfile, "\n");

    fclose(reportsfile);
}


// Create stats.dir
//
void create_stats_file(string work_dir)
{
    FILE *statsdirfile;

    string stats_file_name_no_dir = string(team[0].name) + "_" + string(team[1].name) + ".txt";
    string stats_file_name = work_dir + stats_file_name_no_dir;
    string stats_dir_file_name = work_dir + "stats.dir";

    statsdirfile = fopen(stats_dir_file_name.c_str(), "a");
    fprintf(statsdirfile, "%s\n", stats_file_name_no_dir.c_str());

    fclose(statsdirfile);
}
*/

// Generate a random number up to 10000. If the given p is
// less than the generated number, return 1, otherwise return 0
//
// Used to "throw dice" and check if an event with some probability
// happened. p is 0..10000 - for example 2000 means probability 0.2
// So when 2000 is given, this function simulates an event with
// probability 0.2 and tells if it happened (naturally it has
// a prob. of 0.2 to happen)
//
int randomp(int p)
{
    int value = my_random(10000);

    if (value < p)
        return 1;
    else
        return 0;
}


// Returns a pseudo-random integer between 0 and N-1
//
unsigned my_random(int n)
{
    // genrand / (double) UINT_MAX - obtains a number in the range
    // [0,1], so once in ~4bln numbers we'll get 1, which is undesirable.
    // What we need is the [0, 1) interval, so a fix is supplied for the
    // special case. It only happens with probability  < 10e-9, so it
    // sholdn't affect the statistical quality of the generator.
    //
    double d = genrand() / (double) UINT_MAX;
    int u = (int) (d * n);

    return (u == n ? n-1 : u);
}


/// Adds one minute to the "minutes played" stats of all currently active
/// players in both teams.
///
void update_players_minute_count(void)
{
    int i, j;

    for (j = 0; j <= 1; j++)
        for (i = 1; i <= num_players; i++)
        {
            if (team[j].player[i].active == 1)
                team[j].player[i].minutes++;
        }
}


/// Checks whether the conds of a team should be activated.
///
/// Ran for each team on every minute by the main loop.
///
void check_conditionals(int team_num)
{
    vector<cond*>::iterator iter;

    for (iter = team[team_num].conds.begin(); iter != team[team_num].conds.end(); ++iter)
        (*iter)->test_and_execute();
}


/// Report an error in the conditionals of a team.
///
void cond_error(int team_num, int line, string msg)
{
    die("In conditionals of %s (line %d)\nReason: %s\n", team[team_num].name, line, msg.c_str());
}

// **********************************************************************
// ******************* Here the main program begins *********************
// **********************************************************************
//
// The main routine of ESMS, contains various initializations
// and the game running loop
//
Handle<Value> create(const Arguments &args)
{
    HandleScope scope;

    // Identify the names of teamsheet files
    //
    if (args.Length() != 9)
        return Undefined(); // Illegal usage of esms. Either supply no arguments, or the two teamsheet names

    // handling/parsing command line arguments
    //
    ObjectV8 opt(Handle<Object>::Cast(args[0]));

    unsigned timed_random_seed = opt.get("set_rnd_seed", 0);
    if (timed_random_seed)
    {
        sgenrand(timed_random_seed);
    }
    else
    {
        timed_random_seed = time(NULL);
        sgenrand(timed_random_seed);
    }
	
    // initialize configuration
    //
    ObjectV8 leagueDat(Handle<Object>::Cast(args[1]));
	
    // league ability configuration
    //
    ObjectV8 leagueAbilityDat(Handle<Object>::Cast(args[2]));

    // language dat
    //
    Handle<Object> languageDat = Handle<Object>::Cast(args[3]);

    Handle<Array> tacticDat = Handle<Array>::Cast(args[4]);

    // Read teams' names from the top of the teamsheets
    //
    Handle<Object> home_teamsheet = Handle<Object>::Cast(args[5]);
    Handle<Object> away_teamsheet = Handle<Object>::Cast(args[6]);

    // Read teams' rosters
    //
    Handle<Array> home_roster = Handle<Array>::Cast(args[7]); // team[0].roster_players
    Handle<Array> away_roster = Handle<Array>::Cast(args[8]); // team[1].roster_players

    tact_manager().init(tacticDat);

    team_stats_total_enabled = leagueDat.get("team_stats_total", 0) == 1 ? true : false;

    // find out how many players should be listed in a teamsheet
    //
    int num_subs = leagueDat.get("num_subs", 7);
    int max_substitutions = leagueDat.get("substitutions", 3);

    if (num_subs < 1 || num_subs > 13) return scope.Close(String::New("The number of subs specified in leagueDat must be between 1 and 13"));

    num_players = 11 + num_subs;

    the_commentary().init_commentary(languageDat);

    init_teams_data(home_teamsheet, away_teamsheet, home_roster, away_roster);

    home_bonus = leagueDat.get("home_bonus", 0);

    /* Creating commentary output */
    Handle<Object> output = Object::New();
    Handle<Array> commentary = Array::New();

    print_starting_tactics(output);

    commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("COMM_KICKOFF").c_str()));

    //--------------------------------------------
    //---------- The game running loop -----------
    //--------------------------------------------
    //
    // The timing logic is as follows:
    //
    // The game is divided to two structurally identical
    // halves. The difference between the halves is their
    // start times.
    //
    // For each half, an injury time is added. This time
    // goes into the minute counter, but not into the
    // formal_minute counter (that is needed for reports)
    //

    const int half_length = 45;
    double substitutions = 0;
    double injuries = 0;
    double fouls = 0;

    // For each half
    //
    for (int half_start = 1; half_start < 2*half_length; half_start += half_length)
    {
        int half = half_start == 1 ? 1 : 2;
        int last_minute_of_half = half_start + half_length - 1;
        bool in_inj_time = false;

        // Play the game minutes of this half
        //
        // last_minute_of_half will be increased by inj_time_length in
        // the end of the half
        //
        for (minute = formal_minute = half_start; minute <= last_minute_of_half; ++minute)
        {
            clean_inj_card_indicators();
            recalculate_teams_data();

            // For each team
            //
            for (int j = 0; j <= 1; j++)
            {
                // Calculate different events
                //
                if_shot(commentary, j);
                if_foul(commentary, max_substitutions, j);
                random_injury(commentary, max_substitutions, j);

                score_diff = team[j].score - team[!j].score;
                check_conditionals(j);
            }

            // fixme ?
            if (team_stats_total_enabled)
                if (minute == 1 || minute%10 == 0)
                    add_team_stats_total();

            if (!in_inj_time)
            {
                ++formal_minute;

                update_players_minute_count();
            }

            if (minute == last_minute_of_half && !in_inj_time)
            {
                in_inj_time = true;

                // shouldn't have been increased, but we only know about
                // this now
                --formal_minute;

                int inj_time_length = how_much_inj_time(substitutions, injuries, fouls);
                last_minute_of_half += inj_time_length;

                sprintf(buf, "%d", inj_time_length);
                commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("COMM_INJURYTIME", buf).c_str()));
            }
        }

        in_inj_time = false;

        if (half == 1)
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("COMM_HALFTIME").c_str()));
        else if (half == 2)
            commentary->Set(commentary->Length(), String::New(the_commentary().rand_comment("COMM_FULLTIME").c_str()));
    }

    calc_ability(leagueAbilityDat);

    // There are several options to specify how the user wants
    // to run penalty shootouts. Sorted by precendence:
    //
    // * penalty_score is given. The shootout runs if the game
    //   score matches the given score. (team0Score * 100 + team1score)
    //
    // * penalty_diff is given. The shootout runs if the game
    //   score difference matches the given difference
    //
    // * Check the CUP fpag in league.dat:
    //   CUP = 1     --> ask the user whether to run a shootout
    //   CUP = 2     --> run the shootout anyway
    //
    int penalty_score = opt.get("penalty_score", 101);
    int wanted_diff = opt.get("penalty_diff", 1);
    if (penalty_score)
    {
        int cur_score = team[0].score * 100 + team[1].score;

        if (penalty_score == cur_score)
            RunPenaltyShootout();
    }
    else if (wanted_diff)
    {
        if (wanted_diff == team[0].score - team[1].score)
            RunPenaltyShootout();
    }
    else
    {
        int cup_flag = leagueDat.get("cup", 0);

        if (cup_flag == 1)
        {
            if (opt.get("penalty_shootout", true))
                RunPenaltyShootout();
        }
        else if (cup_flag == 2)
            RunPenaltyShootout();
    }

    print_final_stats(output);
    //create_stats_file(output);
    //update_reports_file(output);

    output->Set(String::New("commentary"), commentary);

    // not reachable
    return scope.Close(output);
}


void add_team_stats_total()
{
    int index, i;

    /* Define the correct index for the array */
    if (minute == 1)
        index = 0;
    else
        index = minute/10;

    for (i = 0; i <= 1; ++i)
    {
        teamStatsTotal[i][index][0] = team[i].team_tackling;
        teamStatsTotal[i][index][1] = team[i].team_passing;
        teamStatsTotal[i][index][2] = team[i].team_shooting;
    }
}


/// Called in the beginning of every minute to clean the indicators
/// of injuries, yellow and red cards (that are used by conditionals).
///
void clean_inj_card_indicators(void)
{
    injured_ind[0] = injured_ind[1] = -1;
    yellow_carded[0] = yellow_carded[1] = red_carded[0] = red_carded[1] = -1;
}

void initialize(Handle<Object> exports, Handle<Object> module){
    module->Set(String::NewSymbol("exports"), FunctionTemplate::New(create)->GetFunction());
}

NODE_MODULE(esms, initialize)
