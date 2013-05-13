require('./test_dat.js');
/*
var
roster_creator = require('./build/Release/roster_creator'),
roster = roster_creator({N_GK:1,N_DF:2,N_DM:3,N_MF:4,N_AM:5,N_FW:6,TEST_BOOL:true,TEST_DBL:3.1425673,TEST_STR:"Hello"});

console.log(roster);

var
fixtures = require('./build/Release/fixtures'),
schedule = fixtures(['team1', 'team2', 'team3']);

console.log(schedule);
*/
/*
var
tsc = require('./build/Release/tsc'),
sheet = tsc('Apes United', "442N", testRoster, leagueDat);

console.log(sheet);
*/

var
esms = require('./build/Release/esms'),
option = {
    set_rnd_seed: 56473,
    penalty_diff: 1,
    penalty_score: 1,
    penalty_shootout: false,
    team_stats_total: 0
},
report = esms(option, leagueDat, leagueAbilities, languageDat, tacticsDat, teamsheet, teamsheet, testRoster, testRoster);

console.log(report);
/*
var
updtr = require('./build/Release/updtr'),
//    "Weekly updates:\n\n"
//    "1)  Update rosters (100% fitness recovery)\n"
//    "2)  Update rosters (50% fitness recovery)\n"
//    "3)  Decrease injuries\n"
//    "4)  Decrease suspensions\n"
//    "5)  Update league table\n"
//    "6)  Decrease suspensions + update rosters (50% of fitness gain)\n"
//    "7)  Decrease suspensions, injuries + update rosters (50% fitness recovery), league table\n"
//    "8)  Decrease suspensions, injuries + update rosters (100% fitness recovery), league table\n\n"
//    "End-of-season updates:\n\n"
//    "9)  Increase all players' age by one\n"
//    "10) Reset all player stats\n"
//    "11) Reset all player stats, including injuries\n"
//    "12) Reset all player stats, including injuries and suspensions\n\n"
//    "Enter your choice -> ";
option = {
    choice: 8
},
result = updtr(option, leagueDat, languageDat, statsDat, testRoster, testRoster);

console.log(result);
*/
