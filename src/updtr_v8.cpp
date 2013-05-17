// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
//
#include <node.h>

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "util.h"
#include "object_v8.h"
#include "league_table_v8.h"
#include "updtr_v8.h"

using namespace v8;
using namespace std;


// whether there is a wait on exit
//
bool waitflag = true;


// These reports are filled in by the various updating functions,
// and printed to one file in the end
//
//vector<string> skill_change_report;
//vector<string> injury_report;
//vector<string> suspension_report;
//vector<string> stats_report;
vector<string> leaders_report;
//vector<string> table_report;


Handle<Value> create(const Arguments &args)
{
    HandleScope scope;

    srand(time(NULL));

    // handling/parsing command line arguments
    //
    ObjectV8 opt(Handle<Object>::Cast(args[0]));
    int option = opt.get("choice", 8);
	
    // initialize configuration
    //
    ObjectV8 leagueDat(Handle<Object>::Cast(args[1]));
	
    // teamInfoDat configuration
    //
    Handle<Array> teamInfoDat = Handle<Array>::Cast(args[2]);

    // statistic dat
    //
    Handle<Array> statsDat = Handle<Array>::Cast(args[3]);

    // league table dat
    //
    Handle<Array> leagueTable = Handle<Array>::Cast(args[4]);

    // home roster
    //
    Handle<Array> homeRoster = Handle<Array>::Cast(args[5]);

    // away roster
    //
    Handle<Array> awayRoster = Handle<Array>::Cast(args[6]);

	int fitnessAfterInjury = leagueDat.get("updtr_fitness_after_injury", 80);
	int fitnessGain = leagueDat.get("updtr_fitness_gain", 20);

    Handle<Object> output = Object::New();

    // Now do the job...
    //
    switch (option)
    {
    case 1:
        update_rosters(leagueDat, statsDat, homeRoster, awayRoster, output);
        recover_fitness(homeRoster, awayRoster, false, fitnessGain);
		generate_leaders();
        break;
    case 2:
        update_rosters(leagueDat, statsDat, homeRoster, awayRoster, output);
	    recover_fitness(homeRoster, awayRoster, true, fitnessGain);
		generate_leaders();
        break;
    case 3:
        decrease_suspensions_injuries(homeRoster, awayRoster, fitnessAfterInjury, INJURIES);
        break;
    case 4:
        decrease_suspensions_injuries(homeRoster, awayRoster, fitnessAfterInjury, SUSPENSIONS);
        break;
    case 5:
        update_league_table(teamInfoDat, leagueTable, output);
        break;
    case 6:
        decrease_suspensions_injuries(homeRoster, awayRoster, fitnessAfterInjury, SUSPENSIONS);
        update_rosters(leagueDat, statsDat, homeRoster, awayRoster, output);
		recover_fitness(homeRoster, awayRoster, true, fitnessGain);
        generate_leaders();
        break;
    case 7:
        decrease_suspensions_injuries(homeRoster, awayRoster, fitnessAfterInjury, SUSPENSIONS | INJURIES);
        update_rosters(leagueDat, statsDat, homeRoster, awayRoster, output);
		recover_fitness(homeRoster, awayRoster, true, fitnessGain);
        generate_leaders();
        update_league_table(teamInfoDat, leagueTable, output);
        break;
    case 8:
        decrease_suspensions_injuries(homeRoster, awayRoster, fitnessAfterInjury, SUSPENSIONS | INJURIES);
        update_rosters(leagueDat, statsDat, homeRoster, awayRoster, output);
		recover_fitness(homeRoster, awayRoster, false, fitnessGain);
        generate_leaders();
        update_league_table(teamInfoDat, leagueTable, output);
        break;
	case 9:
		increase_ages(homeRoster, awayRoster);
		break;
	case 10:
		reset_stats(homeRoster, awayRoster);
		break;
	case 11:
		reset_stats(homeRoster, awayRoster, INJURIES);
		break;
	case 12:
		reset_stats(homeRoster, awayRoster, INJURIES | SUSPENSIONS);
		break;
    default:
        return scope.Close(String::New("Illegal option"));
    }

    output->Set(String::New("homeRoster"), homeRoster);
    output->Set(String::New("awayRoster"), awayRoster);

    return scope.Close(output);
}


bool is_stats_header_line(string line)
{
    vector<string> toks = tokenize(line, " \t");

    if (toks.size() >= 3 && toks[0] == "<<<" && toks[toks.size() - 1] == ">>>")
        return true;
    else
        return false;
}


// Fills in the vectors with stats from filename
//
void get_players_game_stats(const ObjectV8 &leagueDat, string stats_filename,
                            vector<player_game_stats>& home_team,
                            vector<player_game_stats>& away_team)
{
    ifstream file(stats_filename.c_str());

    if (!file)
        die("Failed to open file %s\n", stats_filename.c_str());

    int dp_for_yellow = leagueDat.get("dp_for_yellow", 4);
    int dp_for_red = leagueDat.get("dp_for_red", 10);
    int num_subs = leagueDat.get("num_subs", 7);
    int num_players = 11 + num_subs;
    string line;
    int team_count = 0;

    while (getline(file, line))
    {
        if (is_stats_header_line(line))
        {
            ++team_count;
            vector<player_game_stats> team;

            // get the following empty line and lines with column headers
            //
            getline(file, line);
            getline(file, line);
            getline(file, line);

            for (int i = 0; i < num_players; ++i)
            {
                getline(file, line);
                vector<string> tokens = tokenize(line);

                if (tokens.size() != 24)
                    die("Illegal stats line in file %s:\n%s\n", stats_filename.c_str(), line.c_str());

                player_game_stats player;
                player.name = tokens[0];
                player.pos = tokens[1];
                player.minutes = str_atoi(tokens[9]);
                player.games = (player.minutes > 0) ? 1 : 0;
                player.saves = str_atoi(tokens[10]);
                player.tackles = str_atoi(tokens[11]);
                player.keypasses = str_atoi(tokens[12]);
                player.assists = str_atoi(tokens[13]);
                player.shots = str_atoi(tokens[14]);
                player.goals = str_atoi(tokens[15]);
                player.yellow = str_atoi(tokens[16]);
                player.red = str_atoi(tokens[17]);
                player.injured = str_atoi(tokens[18]);
                player.st_ab = str_atoi(tokens[19]);
                player.tk_ab = str_atoi(tokens[20]);
                player.ps_ab = str_atoi(tokens[21]);
                player.sh_ab = str_atoi(tokens[22]);
                player.fitness = str_atoi(tokens[23]);
                player.dp = dp_for_red * player.red + dp_for_yellow * player.yellow;

                team.push_back(player);
            }


            if (team_count == 1)
                home_team = team;
            else
                away_team = team;
        }
    }
}


// Finds a player by name in a roster
//
Handle<Value> get_player_by_name_from_roster(const Handle<String> &name, const Handle<Array> &players)
{
    HandleScope scope;
    Handle<String> keyName = String::NewSymbol("name");
    Handle<Object> player;
    for (int i=0, l=players->Length(); i<l; ++i)
    {
        player = Handle<Object>::Cast(players->Get(i));
		if (player->Get(keyName)->ToString()->StrictEquals(name))
			return scope.Close(player);
    }

    return Undefined();
}


// Handles a skill change as a result of the ability crossing a threshold
//
// Given:
//   - player name, team name, skill name (for printing to report)
//   - ab_points, skill - the ability and skill affected. In case of a skill change,
//     these values are modified by the function
//
int handle_skill_change(int& ab_points, Handle<String> name, Handle<Array> incList, Handle<Array> decList)
{
    int skill = 0;
    // Increase ?
    if (ab_points >= 1000)
    {
        ab_points -= 700;
        skill++;
        incList->Set(incList->Length(), name);
    }
    // Decrease ?
    else if (ab_points < 0)
    {
        ab_points += 300;
        skill--;
        decList->Set(decList->Length(), name);
    }

    return skill;
}


bool perf_predicate(const pair<string, int>& left, const pair<string, int>& right)
{
    return left.second > right.second;
}


int calc_perf_points(int goals, int shots, int tackles, int saves, int assists, int keypasses, int dp)
{
    return goals * 9 + shots + tackles * 6 + saves * 3 + assists * 7 + keypasses * 4 - dp * 2;
}


string make_header(string header_name)
{
    string line(header_name.length() + 4, '-');
    string ret = line + "\n  " + header_name + "\n" + line + "\n";

    return ret;
}


// Goes over stats.dir and updates the rosters of all teams with
// stats from the listed games.
//
void update_rosters(const ObjectV8 &leagueDat, Handle<Array> statsDat, Handle<Array> homeRoster, Handle<Array> awayRoster, Handle<Object> output)
{
    // fetch some configs
    //
    int max_inj = leagueDat.get("max_injury_length", 9);
    int suspension_margin = leagueDat.get("suspension_margin", 10);
    int dp_for_yellow = leagueDat.get("dp_for_yellow", 4);
    int dp_for_red = leagueDat.get("dp_for_red", 10);
    int num_subs = leagueDat.get("num_subs", 7);
    int num_players = 11 + num_subs;

    map<string, int> weekly_stats;
    weekly_stats["goals"] = weekly_stats["goals_DF"] = weekly_stats["goals_DM"] = weekly_stats["goals_MF"] =
		weekly_stats["goals_AM"] = weekly_stats["goals_FW"] = weekly_stats["assists"] =
		weekly_stats["assists_DF"] = weekly_stats["assists_DM"] = weekly_stats["assists_MF"] =
		weekly_stats["assists_DM"] = weekly_stats["assists_FW"] = weekly_stats["yellows"] =
		weekly_stats["reds"] = weekly_stats["injuries"] = 0;

    vector<pair<string, int> > weekly_performers;

    // For each line in the stats.dir file (that is, for each game stats to
    // update), open the relevant rosters and update the relevant players.
    //
    if (homeRoster->Length() < unsigned(num_players))
        die("Expected %d players of %s in stats file %s\n");

    if (awayRoster->Length() < unsigned(num_players))
        die("Expected %d players of %s in stats file %s\n");

    Handle<Array> roster;
    Handle<Object> stats;
    Handle<Array> statsKey;
    Handle<String> name;
    Handle<Array> playerStat;
    Handle<Value> playerEle;
    Handle<Object> player;
    Handle<Array> suspensionReport = Array::New();
    Handle<Array> injuryReport = Array::New();
    Handle<Array> stIncReport = Array::New();
    Handle<Array> stDecReport = Array::New();
    Handle<Array> tkIncReport = Array::New();
    Handle<Array> tkDecReport = Array::New();
    Handle<Array> psIncReport = Array::New();
    Handle<Array> psDecReport = Array::New();
    Handle<Array> shIncReport = Array::New();
    Handle<Array> shDecReport = Array::New();
    Handle<Object> suspensionTeam;
    Handle<Object> injuryTeam;
    Handle<Array> stIncTeam;
    Handle<Array> stDecTeam;
    Handle<Array> tkIncTeam;
    Handle<Array> tkDecTeam;
    Handle<Array> psIncTeam;
    Handle<Array> psDecTeam;
    Handle<Array> shIncTeam;
    Handle<Array> shDecTeam;
    Handle<String> keyGames = String::NewSymbol("games");
    Handle<String> keyTackles = String::NewSymbol("tackles");
    Handle<String> keyKeyPasses = String::NewSymbol("keypasses");
    Handle<String> keyShots = String::NewSymbol("shots");
    Handle<String> keyGoals = String::NewSymbol("goals");
    Handle<String> keyAssists = String::NewSymbol("assists");
    Handle<String> keySuspension = String::NewSymbol("suspension");
    Handle<String> keyInjury = String::NewSymbol("injury");
    Handle<String> keyFitness = String::NewSymbol("fitness");
    Handle<String> keyStAb = String::NewSymbol("st_ab");
    Handle<String> keyTkAb = String::NewSymbol("tk_ab");
    Handle<String> keyPsAb = String::NewSymbol("ps_ab");
    Handle<String> keyShAb = String::NewSymbol("sh_ab");
    Handle<String> keySt = String::NewSymbol("st");
    Handle<String> keyTk = String::NewSymbol("tk");
    Handle<String> keyPs = String::NewSymbol("ps");
    Handle<String> keySh = String::NewSymbol("sh");
    Handle<String> keyDp = String::NewSymbol("dp");
    int team_n, player_n;
    int stAb, tkAb, psAb, shAb;
    int dp, dp_before_update, dp_after_update, suspension, injury;

    for (team_n = 0; team_n <= 1; ++team_n)
    {
        if (0 == team_n) roster = homeRoster;
        else roster = awayRoster;
        suspensionTeam = Object::New();
        injuryTeam = Object::New();
        stIncTeam = Array::New();
        stDecTeam = Array::New();
        tkIncTeam = Array::New();
        tkDecTeam = Array::New();
        psIncTeam = Array::New();
        psDecTeam = Array::New();
        shIncTeam = Array::New();
        shDecTeam = Array::New();

        stats = Handle<Object>::Cast(statsDat->Get(team_n));
        statsKey = stats->GetOwnPropertyNames();

        // For each player in the stats: look it up in the roster, and
        // update everything
        //
        for (player_n = 0; player_n < num_players; ++player_n)
        {
            name = statsKey->Get(player_n)->ToString();
            playerStat = Handle<Array>::Cast(stats->Get(name));
            playerEle = get_player_by_name_from_roster(name, roster);

            if (playerEle->IsUndefined())
                die("Player %s (from %s) not found in roster %s\n");

            player = Handle<Object>::Cast(playerEle);

            // Add all simple stats
            //
            player->Set(keyGames, Integer::New(player->Get(keyGames)->IntegerValue() + (playerStat->Get(7)->IntegerValue() > 0 ? 1 : 0)));
            player->Set(keyGames, Integer::New(player->Get(keyGames)->IntegerValue() + playerStat->Get(8)->IntegerValue()));
            player->Set(keyTackles, Integer::New(player->Get(keyTackles)->IntegerValue() + playerStat->Get(9)->IntegerValue()));
            player->Set(keyKeyPasses, Integer::New(player->Get(keyKeyPasses)->IntegerValue() + playerStat->Get(10)->IntegerValue()));
            player->Set(keyAssists, Integer::New(player->Get(keyAssists)->IntegerValue() + playerStat->Get(11)->IntegerValue()));
            player->Set(keyShots, Integer::New(player->Get(keyShots)->IntegerValue() + playerStat->Get(12)->IntegerValue()));
            player->Set(keyGoals, Integer::New(player->Get(keyGoals)->IntegerValue() + playerStat->Get(13)->IntegerValue()));

            // Take care of skill increases and decreases
            //
            stAb = player->Get(keyStAb)->IntegerValue() + playerStat->Get(17)->IntegerValue();
            player->Set(keySt, Integer::New(player->Get(keySt)->IntegerValue() + handle_skill_change(stAb, name, stIncTeam, stDecTeam)));
            player->Set(keyStAb, Integer::New(stAb));

            tkAb = player->Get(keyTkAb)->IntegerValue() + playerStat->Get(18)->IntegerValue();
            player->Set(keyTk, Integer::New(player->Get(keyTk)->IntegerValue() + handle_skill_change(tkAb, name, tkIncTeam, stDecTeam)));
            player->Set(keyTkAb, Integer::New(tkAb));

            psAb = player->Get(keyPsAb)->IntegerValue() + playerStat->Get(19)->IntegerValue();
            player->Set(keyPs, Integer::New(player->Get(keyPs)->IntegerValue() + handle_skill_change(psAb, name, psIncTeam, psIncTeam)));
            player->Set(keyPsAb, Integer::New(psAb));

            shAb = player->Get(keyShAb)->IntegerValue() + playerStat->Get(21)->IntegerValue();
            player->Set(keySh, Integer::New(player->Get(keySh)->IntegerValue() + handle_skill_change(shAb, name, shIncTeam, shIncTeam)));
            player->Set(keyShAb, Integer::New(shAb));

            // Take care of DP and suspensions
            //
            // A suspension takes place if after the update, a player's
            // DP crossed some factor of suspension_margin. Then, the length of
            // the suspension is this factor.
            //
            // For example:
            //
            // A player's DP before the game was 18, and he got 3 DP during
            // the game, and suspension_margin = 10. His total DP now is 21, so
            // he crossed a factor (crossed = was below it prior to the update,
            // and is above it after the update). Then, his suspension period
            // is 2 (since it's int(DP/suspension_margin).
            //
            dp_before_update = player->Get(keyDp)->IntegerValue();
            dp = playerStat->Get(14)->IntegerValue() * dp_for_yellow + playerStat->Get(15)->IntegerValue() * dp_for_red;
            dp_after_update = dp_before_update + dp;

            // Note: relying on C++'s division of integers --> integral part
            //
            if ((dp_before_update / suspension_margin) < (dp_after_update / suspension_margin))
            {
                suspension = dp_after_update / suspension_margin;
                suspensionTeam->Set(name, Integer::New(suspension));
                player->Set(keySuspension, Integer::New(suspension));
            }
            player->Set(keyDp, Integer::New( dp_after_update));

            // Take care of injuries
            //
            if (playerStat->Get(16)->IntegerValue())
            {
                injury = my_random(my_random(max_inj + 1) + 1);
                injuryTeam->Set(name, Integer::New(injury));
                player->Set(keyInjury, Integer::New(injury));
            }

            // Take care of fitness
            //
            player->Set(keyFitness, playerStat->Get(22));
/*
            // Generate weekly statistics
            //
            weekly_stats["goals"] += playerStat->Get(13)->IntegerValue();
            weekly_stats["assists"] += playerStat->Get(11)->IntegerValue();
            weekly_stats["yellows"] += playerStat->Get(14)->IntegerValue();
            weekly_stats["reds"] += playerStat->Get(15)->IntegerValue();
            weekly_stats["injuries"] += playerStat->Get(16)->IntegerValue();

            if (stats_teams[team_n][player_n].pos != "GK")
            {
                string only_position = stats_teams[team_n][player_n].pos.substr(0, 2);

                weekly_stats["goals_" + only_position] += stats_teams[team_n][player_n].goals;
                weekly_stats["assists_" + only_position] += stats_teams[team_n][player_n].assists;
            }

            string name_and_team = stats_teams[team_n][player_n].name + " (" + team_name[team_n] + ")";

            perf_points = calc_perf_points(stats_teams[team_n][player_n].goals,
                                               stats_teams[team_n][player_n].shots,
                                               stats_teams[team_n][player_n].tackles,
                                               stats_teams[team_n][player_n].saves,
                                               stats_teams[team_n][player_n].assists,
                                               stats_teams[team_n][player_n].keypasses,
                                               stats_teams[team_n][player_n].dp);

            weekly_performers.push_back(make_pair(name_and_team, perf_points));
*/
        }
        suspensionReport->Set(team_n, suspensionTeam);
        injuryReport->Set(team_n, injuryTeam);
        stIncReport->Set(team_n, stIncTeam);
        stDecReport->Set(team_n, stDecTeam);
        tkIncReport->Set(team_n, tkIncTeam);
        tkDecReport->Set(team_n, tkDecTeam);
        psIncReport->Set(team_n, psIncTeam);
        psDecReport->Set(team_n, psDecTeam);
        shIncReport->Set(team_n, shIncTeam);
        shDecReport->Set(team_n, shDecTeam);
    }
    output->Set(String::New("suspension"), suspensionReport);
    output->Set(String::New("injury"), injuryReport);
    output->Set(String::New("stInc"), stIncReport);
    output->Set(String::New("stDec"), stDecReport);
    output->Set(String::New("tkInc"), tkIncReport);
    output->Set(String::New("tkDec"), tkDecReport);
    output->Set(String::New("psInc"), psIncReport);
    output->Set(String::New("psDec"), psDecReport);
    output->Set(String::New("shInc"), shIncReport);
    output->Set(String::New("shDec"), shDecReport);
/*
    stats_report.push_back(make_header("Round summary"));
    stats_report.push_back(format_str("Goals:        %3d  (DFs - %d, DMs - %d, MFs - %d, AMs - %d, FWs - %d)",
                                      weekly_stats["goals"], weekly_stats["goals_DF"],
                                      weekly_stats["goals_DM"], weekly_stats["goals_MF"],
                                      weekly_stats["goals_AM"], weekly_stats["goals_FW"]));
    stats_report.push_back(format_str("Assists:      %3d  (DFs - %d, DMs - %d, MFs - %d, AMs - %d, FWs - %d)",
                                      weekly_stats["assists"], weekly_stats["assists_DF"],
                                      weekly_stats["assists_DM"], weekly_stats["assists_MF"],
                                      weekly_stats["assists_AM"], weekly_stats["assists_FW"]));
    stats_report.push_back(format_str("Yellow cards: %3d", weekly_stats["yellows"]));
    stats_report.push_back(format_str("Red cards:    %3d", weekly_stats["reds"]));
    stats_report.push_back(format_str("Injuries:     %3d", weekly_stats["injuries"]));

    sort(weekly_performers.begin(), weekly_performers.end(), perf_predicate);

    stats_report.push_back("\nTop performers:\n");

    for (vector<pair<string, int> >::const_iterator it = weekly_performers.begin();
            it != weekly_performers.end(); ++it)
    {
        if (it - weekly_performers.begin() > 10)
            break;

        stats_report.push_back(format_str("%-20s  %d", it->first.c_str(), it->second));
    }
*/
}


// Goes over all players in all teams listed in teams.dir and "transforms" them using
// the provided transformer_proc
//
void transform_all_players(void (*transformer_proc)(Handle<Object>, void*, int), Handle<Array> roster, void* arg, int opt)
{
    HandleScope scope;
    for (int i=0, l=roster->Length(); i<l; ++i)
    {
        transformer_proc(Handle<Object>::Cast(roster->Get(i)), arg, opt);
    }
}


void transformer_recover_fitness(Handle<Object> player, void* arg, int gain)
{
    HandleScope scope;
	bool* half = (bool*) arg;
	
	if (*half) gain /= 2;
	Handle<String> keyFitness = String::NewSymbol("fitness");
	int fitness = player->Get(keyFitness)->IntegerValue() + gain;
	
	if (fitness > 100) fitness = 100;
    player->Set(keyFitness, Integer::New(fitness));
}


void recover_fitness(Handle<Array> homeRoster, Handle<Array> awayRoster, bool half, int fitnessGain)
{
	transform_all_players(transformer_recover_fitness, homeRoster, &half, fitnessGain);
	transform_all_players(transformer_recover_fitness, awayRoster, &half, fitnessGain);
}


void transformer_increase_ages(Handle<Object> player, void*, int)
{
    HandleScope scope;
	Handle<String> keyAge = String::NewSymbol("age");

	player->Set(keyAge, Integer::New(player->Get(keyAge)->IntegerValue() + 1));
}


void increase_ages(Handle<Array> homeRoster, Handle<Array> awayRoster)
{
	transform_all_players(transformer_increase_ages, homeRoster, 0, 0);
	transform_all_players(transformer_increase_ages, awayRoster, 0, 0);
}


void transformer_reset_stats(Handle<Object> player, void* arg, int)
{
    /*
	player->games = player->saves = player->tackles = player->keypasses = player->shots = player->goals = player->assists = player->dp = 0;
	player->fitness = 100;
	
	unsigned* inj_sus_flag = (unsigned*) arg;
	
	if (*inj_sus_flag & INJURIES)
		player->injury = 0;
	
	if (*inj_sus_flag & SUSPENSIONS)
		player->suspension = 0;
    */
}


void reset_stats(Handle<Array> homeRoster, Handle<Array> awayRoster, unsigned inj_sus_flag)
{
	transform_all_players(transformer_reset_stats, homeRoster, &inj_sus_flag, 0);
	transform_all_players(transformer_reset_stats, awayRoster, &inj_sus_flag, 0);
}


void transformer_decrease_sus_inj(Handle<Object> player, void* arg, int)
{
    /*
	unsigned* inj_sus_flag = (unsigned*) arg;
	
	if (*inj_sus_flag & SUSPENSIONS)
	{
		// those with 0 will be decreased to -1, hence they will generate
		// no report on "coming back".
		//
		player->suspension--;

		if (player->suspension == 0)
			suspension_report.push_back(the_commentary().rand_comment("UPDTR_END_SUSPENSION",
										player->name.c_str(),
										team_name.c_str()));
		else if (player->suspension < 0)
			player->suspension = 0;
	}

	if (*inj_sus_flag & INJURIES)
	{
		player->injury--;

		if (player->injury == 0)
		{
			injury_report.push_back(the_commentary().rand_comment("UPDTR_END_INJURY",
									player->name.c_str(),
									team_name.c_str()));

			player->fitness = leagueDat.get("UPDTR_FITNESS_AFTER_INJURY", 80);
		}
		else if (player->injury < 0)
			player->injury = 0;
	}
    */
}


void decrease_suspensions_injuries(Handle<Array> homeRoster, Handle<Array> awayRoster, int fitnessAfterInjury, unsigned inj_sus_flag)
{
	transform_all_players(transformer_decrease_sus_inj, homeRoster, &inj_sus_flag, fitnessAfterInjury);
	transform_all_players(transformer_decrease_sus_inj, awayRoster, &inj_sus_flag, fitnessAfterInjury);
}


void make_leaders_report(const vector<player_stat>& list, string heading, string stat)
{
    leaders_report.push_back("\n\n" + heading + ":");
    leaders_report.push_back(format_str("\nName                 Games    %s\n"
                                        "---------------------------------", stat.c_str()));

    for (int i = 0; i < min<int>(15, list.size()); ++i)
    {
        string name_and_team = list[i].name + " (" + list[i].team_name + ")";
        int the_stat =
            (stat == "Gls") ? list[i].goals :
            (stat == "Ass") ? list[i].assists :
            (stat == "DPs") ? list[i].dp : list[i].perf_points;

        leaders_report.push_back(format_str("%-20s %5d  %5d", name_and_team.c_str(),
                                            list[i].games, the_stat));
    }
}


bool leaders_predicate_goals(const player_stat& p1, const player_stat& p2)
{
    return (p1.goals > p2.goals) || (p1.goals == p2.goals && p1.games < p2.games);
}


bool leaders_predicate_assists(const player_stat& p1, const player_stat& p2)
{
    return (p1.assists > p2.assists) || (p1.assists == p2.assists && p1.games < p2.games);
}


bool leaders_predicate_dps(const player_stat& p1, const player_stat& p2)
{
    return (p1.dp > p2.dp) || (p1.dp == p2.dp && p1.games < p2.games);
}


bool leaders_predicate_perf(const player_stat& p1, const player_stat& p2)
{
    return (p1.perf_points > p2.perf_points) || (p1.perf_points == p2.perf_points && p1.games < p2.games);
}


void generate_leaders(void)
{
    /*
    ifstream dir_file("teams.dir");

    if (!dir_file)
        die("Failed to open file teams.dir\n");

    vector<player_stat> stat_players;

    string line;

    while (getline(dir_file, line))
    {
        // delete spaces
        line.erase(remove
                   (line.begin(), line.end(), ' '), line.end());
        string roster_name = line;
        string team_name = roster_name.substr(0, roster_name.find_first_of("."));

		RosterPlayerArray players;
		string msg = read_roster_players(roster_name, players);
	
		if (msg != "")
		{
			cerr << "Error in leaders generation: " << msg << endl;
			continue;
		}

        for (RosterPlayerIterator player = players.begin(); player != players.end(); ++player)
        {
            int perf_points = calc_perf_points(player->goals,
                                               player->shots,
                                               player->tackles,
                                               player->saves,
                                               player->assists,
                                               player->keypasses,
                                               player->dp);

            stat_players.push_back(player_stat(player->name,
                                               team_name,
                                               player->games,
                                               player->goals,
                                               player->assists,
                                               player->dp,
                                               perf_points));

            if (is_only_whitespace(player->name))
                cout << "ALARM";
        }
    }

    sort(stat_players.begin(), stat_players.end(), leaders_predicate_goals);
    make_leaders_report(stat_players, "Scorers", "Gls");
    sort(stat_players.begin(), stat_players.end(), leaders_predicate_perf);
    make_leaders_report(stat_players, "Performers", "Pts");
    sort(stat_players.begin(), stat_players.end(), leaders_predicate_assists);
    make_leaders_report(stat_players, "Assisters", "Ass");
    sort(stat_players.begin(), stat_players.end(), leaders_predicate_dps);
    make_leaders_report(stat_players, "Disciplinary points", "DPs");

    cout << "Leaders generated\n";
    */
}


void update_league_table(Handle<Array> teamInfo, Handle<Array> leagueTable, Handle<Object> output)
{
    HandleScope scope;
    league_table table;
    char strTeam1[64];
    char strTeam2[64];
    Handle<String> keyName = String::NewSymbol("name");
    Handle<String> keyScore = String::NewSymbol("score");
    Handle<Object> team1 = Handle<Object>::Cast(teamInfo->Get(0));
    Handle<Object> team2 = Handle<Object>::Cast(teamInfo->Get(1));

    toAscii(team1->Get(keyName)->ToString(), strTeam1);
    toAscii(team2->Get(keyName)->ToString(), strTeam2);

    table.read_league_table_file(leagueTable);
    table.read_results_file(strTeam1, team1->Get(keyScore)->IntegerValue(), strTeam2, team1->Get(keyScore)->IntegerValue());

    Handle<Array> newLeagueDat = table.dump_league_table();
    output->Set(String::New("leagueTable"), newLeagueDat);
}


// suckz, but the needs of updtr are trivial.
//
int my_random(int n)
{
    return rand() % n;
}

void initialize(Handle<Object> exports, Handle<Object> module){
    module->Set(String::NewSymbol("exports"), FunctionTemplate::New(create)->GetFunction());
}

NODE_MODULE(updtr, initialize)
