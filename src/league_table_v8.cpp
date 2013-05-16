// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#include <node.h>

#include "league_table_v8.h"


bool team_data_predicate(league_table::team_data data1, league_table::team_data data2)
{
    if (data1.points != data2.points)
        return data1.points > data2.points;
    else
    {
        if (data1.goal_difference != data2.goal_difference)
            return data1.goal_difference > data2.goal_difference;
        else
            return data1.goals_for > data2.goals_for;
    }
}


bool league_table::team_exists(string name)
{
    return (teams.find(name) != teams.end());
}


void league_table::read_league_table_file(Handle<Array> leagueDat)
{
    // The file doesn't have to exist (if it doesn't, a new table is
    // created). But if it exists, it must be in correct format
    //
    if (leagueDate->Length())
    {
        HandleScope scope;
        Handle<Array> tokens;
        for(int i=0, l=leagueDat->Length(); i<l; ++i)
        {
            tokens = Handle<Array>::Cast(leagueDat->Get(i));

            // The structure of a line must be:
            //
            // PLACE TEAMNAME+ PL W D L GF GA GD PTS
            //
            // TEAMNAME may be multiple tokens, so we count from the
            // end ! The first token is PLACE, the last 8 tokens are
            // as specified, and everything between the first and
            // the last 8 is the team name.
            //
            // Note: when the team name is restructured from the
            // tokens, each token is separated by one space
            //
            unsigned num_tokens = tokens->Length();

            if (num_tokens < 10)
                die("The following line in leaguedat has too few tokens%s");

            int points = tokens->Get(9)->IntegerValue();
            int goal_difference = tokens->Get(8)->IntegerValue();
            int goals_against = tokens->Get(7)->IntegerValue();
            int goals_for = tokens->Get(6)->IntegerValue();
            int lost = tokens->Get(5)->IntegerValue();
            int drawn = tokens->Get(4)->IntegerValue();
            int won = tokens->Get(3)->IntegerValue();
            int played = tokens->Get(2)->IntegerValue();
            char name[64];
            toAscii(tokens->Get(1)->ToString(), name);

            add_new_team(string(name), played, won, drawn, lost, goals_for, goals_against,
                         goal_difference, points);
        }
    }
}


void league_table::read_results_file(const char *teamName1, int score1, const char *teamName2, int score2)
{
    add_team_result(string(teamName1), score1, score2);
    add_team_result(string(teamName2), score2, score1);
}


// adds a new team to the league
// if such team already exists - it's replaced
//
void league_table::add_new_team(string name, int played, int won,
                                int drawn, int lost, int goals_for,
                                int goals_against, int goal_difference,
                                int points)
{
    team_data new_team_data(name, played, won, drawn, lost, goals_for,
                            goals_against, goal_difference, points);

    teams[name] = new_team_data;
}


// adds a new result for a team in terms of how many goals
// it scored and conceded. recalculates all team data.
// if such team doesn't exist - it's created
//
void league_table::add_team_result(string name, int scored, int conceded)
{
    team_data data(name);

    if (team_exists(name))
        data = teams[name];

    data.played++;

    if (scored > conceded)
    {
        data.won++;
        data.points += 3;
    }
    else if (scored < conceded)
    {
        data.lost++;
    }
    else
    {
        data.drawn++;
        data.points++;
    }

    data.goals_for += scored;
    data.goals_against += conceded;
    data.goal_difference = data.goals_for - data.goals_against;

    teams[name] = data;
}


Handle<Array> league_table::dump_league_table(void)
{
    HandleScope scope;

    Handle<Array> leagueTable = Array::New();
    Handle<Array> line;
    vector<team_data> sorted_teams;

    for (map<string, team_data>::const_iterator i = teams.begin();
            i != teams.end(); ++i)
    {
        sorted_teams.push_back(i->second);
    }

    sort(sorted_teams.begin(), sorted_teams.end(), team_data_predicate);

    // print teams
    //
    for (vector<team_data>::const_iterator i = sorted_teams.begin(); i != sorted_teams.end(); ++i)
    {
        line = Array::New();
        line->Set(0, Integer::New((i - sorted_teams.begin()) + 1));
        line->Set(1, String::New(i->name.c_str());
        line->Set(2, Integer::New(i->played);
        line->Set(3, Integer::New(i->won);
        line->Set(4, Integer::New(i->drawn);
        line->Set(5, Integer::New(i->lost);
        line->Set(6, Integer::New(i->goals_for);
        line->Set(7, Integer::New(i->goals_against);
        line->Set(8, Integer::New(i->goal_difference);
        line->Set(9, Integer::New(i->points);

        leagueTable->Set(leagueTable->Length(), line);
    }

    return scope.Close(leagueTable);
}
