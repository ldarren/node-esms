var dat = require('./test_dat.js');

var
roster_creator = require('./build/Release/roster_creator'),
roster = roster_creator({N_GK:1,N_DF:2,N_DM:3,N_MF:4,N_AM:5,N_FW:6,TEST_BOOL:true,TEST_DBL:3.1425673,TEST_STR:"Hello"});

console.log(roster);

var
fixtures = require('./build/Release/fixtures'),
schedule = fixtures(['team1', 'team2', 'team3']);

console.log(schedule);

var
tsc = require('./build/Release/tsc'),
sheet = tsc('Apes United', "442N", testRoster, leagueDat, leagueAbilities, leagueAbbreviations);

console.log(sheet);
