// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
////////////////////////////////////////////////////////////////////////////
//
// This program will create a fixtures list for 2 or more teams 
//
// Algorithm:
// 
// For example, with 8 teams the first week is:
// 
// 1 - 2
// 3 - 4
// 5 - 6
// 7 - 8 
//
// Then leave #1 in place and rotate the rest clockwise,
// 1 - 3
// 5 - 2
// 7 - 4
// 8 - 6
//
// and so on...
// 1 - 5
// 7 - 3
// 8 - 2
// 6 - 4
//
// If the amount of teams is odd, a dummy team will be added and all
// it's games will be deleted in the end, so some team will miss a 
// game each week.
//
////////////////////////////////////////////////////////////////////////////
#include <node.h> 

#include <string>
#include <vector>

using namespace std;
using namespace v8;


Handle<Value> create(const Arguments& args)
{
    // handling/parsing command line arguments
    //
    HandleScope scope;

    const Handle<String> DUMMY_TEAMNAME = String::New("DUMMY DUMMY DUMMY DUMMY HOPE IT WONT APPEAR !!!!");

    Handle<Array> teams = Handle<Array>::Cast(args[0]);
    unsigned num_teams = teams->Length();

    if (num_teams < 2)
        return Undefined(); // Two teams or more are needed for a league !

    // Insert a dummy team if there is an odd amount of teams
    // Note: from here on, the amount of teams in the league
    // is even.
    //
    if (num_teams % 2 == 1)
    {
        teams->Set(teams->Length(), DUMMY_TEAMNAME);
        ++num_teams;
    }

    // Initialize the games vector
    // 
    unsigned num_weeks_in_round = num_teams - 1;
    vector< Handle<String> > empty_round(num_teams);
    vector< vector< Handle<String> > > games(num_weeks_in_round, empty_round);

    // Initialize 1st week (1st vs. 2nd, 3rd vs. 4th, etc...)
    //
    for (unsigned i = 0; i < num_teams; ++i)
    {
        games[0][i] = Handle<String>::Cast(teams->Get(i));
    }

    // Create a round of games
    //
    if (num_teams > 2)
    {
        for (unsigned week_n = 0; week_n < num_weeks_in_round - 1; ++week_n)
        {
            // Each week is built from the previous week, using the 
            // algorithm described at the top of this file.
            //
            for (unsigned team_n = 1; team_n < num_teams - 1; ++team_n)
            {
                if (team_n % 2 == 1)
                    games[week_n + 1][team_n + 2] = games[week_n][team_n];
                else
                    games[week_n + 1][team_n - 2] = games[week_n][team_n];
            }

            // Special rotation around the first team (which doesn't move)
            //
            games[week_n + 1][0] = games[week_n][0];
            games[week_n + 1][1] = games[week_n][2];
            games[week_n + 1][num_teams - 2] = games[week_n][num_teams - 1];
        }
    }

    // Calibrate home/away so that every team playes home-away-home-away...
    // (very approximately: better for large leagues, worse for small ones).
    //
    // This is done by swapping all teams' home/away every other week.
    //
    for (unsigned week_n = 1; week_n < num_weeks_in_round; week_n += 2)
        for (unsigned team_n = 0; team_n < num_teams; team_n += 2)
            swap(games[week_n][team_n], games[week_n][team_n + 1]);

    Handle<Array> output = Array::New();
    Handle<Array> pairs, pair;
    Handle<String> home_team, away_team;

	// Now print the fixtures to fixtures.txt
	// First, "games" is printed as is. Then, it's printed again, with
	// home/away reversed (second round). The dummy team is removed.
	// 
	for (unsigned week_n = 0; week_n < num_weeks_in_round * 2; ++week_n)
	{
        pairs = Array::New();
	
	    for (unsigned team_n = 0; team_n < num_teams; team_n += 2)
	    {
            home_team = (week_n < num_weeks_in_round) ? games[week_n][team_n] : games[week_n - num_weeks_in_round][team_n + 1];
            away_team = (week_n < num_weeks_in_round) ? games[week_n][team_n + 1] : games[week_n - num_weeks_in_round][team_n];

            if (!home_team->StrictEquals(DUMMY_TEAMNAME) && !away_team->StrictEquals(DUMMY_TEAMNAME)){
                pair = Array::New(2);
                pair->Set(0, home_team);
                pair->Set(1, away_team);
                pairs->Set(pairs->Length(), pair);
            }
            
            output->Set(week_n, pairs);
        }
	}
	
    return scope.Close(output);
}

void initialize(Handle<Object> exports, Handle<Object> module){
    module->Set(String::NewSymbol("exports"), FunctionTemplate::New(create)->GetFunction());
}

NODE_MODULE(fixtures, initialize)
