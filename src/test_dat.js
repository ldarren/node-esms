Object.freeze(
leagueDat = {
    home_bonus: 200,
    dp_for_yellow: 4,
    dp_for_red: 10 ,
    suspension_margin: 10,
    max_injury_length: 9,
    num_subs: 5,
    substitutions: 3,
    updtr_fitness_gain: 20,
    updtr_fitness_after_injury: 80,
    team_stats_total: 1,
    cup: 0
});
Object.freeze(
leagueAbilities = {
    AB_Goal: 50,
    AB_Assist: 35,
    AB_Victory_Random: 60,
    AB_Clean_Sheet: 50,
    AB_Ktk: 18,
    AB_Kps: 12,
    AB_Sht_On: 2,
    AB_Sht_Off: 0,
    AB_Sav: 12,
    AB_Defeat_Random: -60,
    AB_Concede: -8,
    AB_Yellow: -8,
    AB_Red: -20
});
Object.freeze(
leagueAbbreviations = {
    ape: "Apes_United",
    klm: "KLM_Royal_Club",
    uva: "Universidad_De_Valladolid",
    mac: "Machos_FC",
    bla: "Blade_Runners",
    esu: "ESMS_United",
    cra: "Crazy_Insomniacs"
});
Object.freeze(
tacticsDat = [
["TACTIC","N","Normal"],
["TACTIC","D","Defensive"],
["TACTIC","A","Attacking"],
["TACTIC","C","Counter_Attack"],
["TACTIC","L","Long_Ball"],
["TACTIC","P","Passing"],
"MULT N DF TK 1.0",
"MULT N DF PS 0.5",
"MULT N DF SH 0.3",
"MULT N DM TK 0.85",
"MULT N DM PS 0.75",
"MULT N DM SH 0.3",
"MULT N MF TK 0.3",
"MULT N MF PS 1.0",
"MULT N MF SH 0.3",
"MULT N AM TK 0.3",
"MULT N AM PS 0.85",
"MULT N AM SH 0.85",
"MULT N FW TK 0.3",
"MULT N FW PS 0.3",
"MULT N FW SH 1.0",
"MULT D DF TK 1.25",
"MULT D DF PS 0.5",
"MULT D DF SH 0.25",
"MULT D DM TK 1.13",
"MULT D DM PS 0.68",
"MULT D DM SH 0.25",
"MULT D MF TK 1.0",
"MULT D MF PS 0.75",
"MULT D MF SH 0.25",
"MULT D AM TK 0.75",
"MULT D AM PS 0.65",
"MULT D AM SH 0.5",
"MULT D FW TK 0.5",
"MULT D FW PS 0.25",
"MULT D FW SH 0.75",
"BONUS D L DF TK 0.25",
"MULT A DF TK 1.0",
"MULT A DF PS 0.5",
"MULT A DF SH 0.5",
"MULT A DM TK 0.5",
"MULT A DM PS 0.75",
"MULT A DM SH 0.68",
"MULT A MF TK 0.0",
"MULT A MF PS 1.0",
"MULT A MF SH 0.75",
"MULT A AM TK 0.0",
"MULT A AM PS 0.87",
"MULT A AM SH 1.13",
"MULT A FW TK 0.0",
"MULT A FW PS 0.75",
"MULT A FW SH 1.5",
"MULT C DF TK 1.0",
"MULT C DF PS 0.5",
"MULT C DF SH 0.25",
"MULT C DM TK 0.85",
"MULT C DM PS 0.85",
"MULT C DM SH 0.25",
"MULT C MF TK 0.5",
"MULT C MF PS 1.0",
"MULT C MF SH 0.25",
"MULT C AM TK 0.5",
"MULT C AM PS 0.85",
"MULT C AM SH 0.65",
"MULT C FW TK 0.5",
"MULT C FW PS 0.5 ",
"MULT C FW SH 1.0",
"BONUS C A MF SH 0.5",
"BONUS C A DF PS 0.25",
"BONUS C A DF SH 0.25",
"BONUS C P MF SH 0.5",
"BONUS C P DF PS 0.25",
"BONUS C P DF SH 0.25",
"MULT L DF TK 1.0",
"MULT L DF PS 0.25",
"MULT L DF SH 0.25",
"MULT L DM TK 0.75",
"MULT L DM PS 0.85",
"MULT L DM SH 0.38",
"MULT L MF TK 0.5",
"MULT L MF PS 1.0",
"MULT L MF SH 0.5 ",
"MULT L AM TK 0.45",
"MULT L AM PS 0.85",
"MULT L AM SH 0.9",
"MULT L FW TK 0.25",
"MULT L FW PS 0.5 ",
"MULT L FW SH 1.3",
"BONUS L C DF TK 0.25",
"BONUS L C DF PS 0.5",
"MULT P DF TK 1.0",
"MULT P DF PS 0.75",
"MULT P DF SH 0.3 ",
"MULT P DM TK 0.87",
"MULT P DM PS 0.87",
"MULT P DM SH 0.28",
"MULT P MF TK 0.25",
"MULT P MF PS 1.0",
"MULT P MF SH 0.25",
"MULT P AM TK 0.25",
"MULT P AM PS 0.87",
"MULT P AM SH 0.68",
"MULT P FW TK 0.25",
"MULT P FW PS 0.75",
"MULT P FW SH 1.0",
"BONUS P L MF SH 0.5",
"BONUS P L MF TK 0.5",
"BONUS P L FW SH 0.25"
]);
Object.freeze(
languageDat = [
// A scoring chance
// 1st %s - game minute
// 2nd %s - team abbrevation
// 3rd %s - player name
["CHANCE", "\nMin. %s :(%s) %s with the dribble"],
["CHANCE", "\nMin. %s :(%s) %s takes possesion"],
["CHANCE", "\nMin. %s :(%s) %s cuts through the defense"],
["CHANCE", "\nMin. %s :(%s) %s finds a hole in the defense"],
["CHANCE", "\nMin. %s :(%s) %s takes advantage of a defensive mistake"],
["CHANCE", "\nMin. %s :(%s) %s finds his way through"],
["CHANCE", "\nMin. %s :(%s) %s sidesteps his marker"],
["CHANCE", "\nMin. %s :(%s) %s with a flashy move"],
["CHANCE", "\nMin. %s :(%s) %s beats his marker"],
["CHANCE", "\nMin. %s :(%s) %s with a real burst of pace"],
["CHANCE", "\nMin. %s :(%s) %s bursts forward"],
["CHANCE", "\nMin. %s :(%s) %s finds himself in a good position"],
// An assisted scoring chance
// 1st %s - game minute
// 2nd %s - team abbrevation
// 3rd %s - player making the pass
// 4th %s - player receiving the pass
["ASSISTEDCHANCE", "\nMin. %2s :(%s) %s passes the ball to %s"],
["ASSISTEDCHANCE", "\nMin. %2s :(%s) %s with a smart pass to %s"],
["ASSISTEDCHANCE", "\nMin. %2s :(%s) %s finds %s in the box"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s with a precise pass to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s heads the ball down to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s slides the ball across to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s cuts the ball back to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s with the heel pass to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s plays a long ball to %s"],
["ASSISTEDCHANCE", "\nMin. %s :(%s) %s with a glorious long pass to %s"],
// A key tackle (scoring chance prevented)
// 1st %s - player name
["TACKLE", "\n          ...  Cleared by %s"],
["TACKLE", "\n          ...  Blocked by %s"],
["TACKLE", "\n          ...  %s wins the ball with a clear tackle"],
["TACKLE", "\n          ...  Intercepted by %s"],
["TACKLE", "\n          ...  %s gets in the way and wins the ball"],
["TACKLE", "\n          ...  But %s clears the danger"],
["TACKLE", "\n          ...  But %s clears the ball to safety"],
["TACKLE", "\n          ...  But %s wins the ball with a sliding challenge"],
["TACKLE", "\n          ...  But %s wins the tackle"],
["TACKLE", "\n          ...  But %s reads the situation well and wins the ball"],
// A shot to goal
// 1st %s - player name
["SHOT", "\n          ...  A powerful shot by %s !"],
["SHOT", "\n          ...  %s tries to beat the keeper !"],
["SHOT", "\n          ...  %s with the strike !"],
["SHOT", "\n          ...  %s shoots towards goal !"],
["SHOT", "\n          ...  %s tries to chip the ball over the keeper !"],
["SHOT", "\n          ...  %s with the shot !"],
["SHOT", "\n          ...  %s chases it through, he must score !"],
["SHOT", "\n          ...  %s is one of one with the keeper, he shoots !"],
["SHOT", "\n          ...  %s goes for goal !"],
["SHOT", "\n          ...  A vicious shot by %s !"],
// A save (by the goalkeeper)
// 1st %s - goalkeeper name
["SAVE", "\n          ...  Saved by %s"],
["SAVE", "\n          ...  %s gathers it comfortably"],
["SAVE", "\n          ...  %s makes a comfortable save"],
["SAVE", "\n          ...  But %s makes a fine save"],
["SAVE", "\n          ...  %s makes a good save"],
["SAVE", "\n          ...  %s parries it"],
["SAVE", "\n          ...  %s makes a difficult save"],
["SAVE", "\n          ...  But %s with the difficult save"],
["SAVE", "\n          ...  But %s reaches the ball. Good save"],
["SAVE", "\n          ...  But %s punches it away"],
// Shot going off target
["OFFTARGET", "\n          ...  But it goes wide"],
["OFFTARGET", "\n          ...  But it goes wide of the post"],
["OFFTARGET", "\n          ...  Over the bar"],
["OFFTARGET", "\n          ...  It goes wide for a goal kick"],
["OFFTARGET", "\n          ...  But it clips the post and goes wide"],
["OFFTARGET", "\n          ...  But it whistles just past the post"],
["OFFTARGET", "\n          ...  But it goes just over"],
["OFFTARGET", "\n          ...  But he puts it wide"],
["OFFTARGET", "\n          ...  But he puts it into the crowd"],
["OFFTARGET", "\n          ...  Wide of goal"],
// The shot resulted in a goal
["GOAL", "\n          ...  GOAL !!"],
// The goal was cancelled
["GOALCANCELLED", "\n          ...  But it's been disallowed. The linesman raised his flag"],
["GOALCANCELLED", "\n          ...  But it's been disallowed. The referee spotted something"],
["GOALCANCELLED", "\n          ...  But it's been disallowed. The linesman flags for offside"],
// An injury
// 1st %s - minute
// 2nd %s - team abbrevation
// 3rd %s - the injured player
["INJURY", "\nMin. %s :(%s) %s injured. He's unable to continue"],
["INJURY", "\nMin. %s :(%s) %s injured. He's been stretchered off"],
["INJURY", "\nMin. %s :(%s) %s falls badly. He's unable to continue"],
// An injury
// 1st %s - minute
// 2nd %s - team abbrevation
// 3rd %s - player name
// 4th %s - new position
["CHANGEPOSITION", "\nMin. %s :(%s) %s will now play as %s"],
// A substitution
// 1st %s - minute
// 2nd %s - team abbrevation
// 3rd %s - player-in name
// 4th %s - player-out name
// 5th %s - the position he'll play
["SUB", "\nMin. %s :(%s) %s will come on for %s and play %s"],
// The team has no substitutions left
["NOSUBSLEFT", "\n          ...  They have no substitutions left. Will play a man down"],
// Tactic change
// 1st %s - minute
// 2nd %s - team abbrevation
// 3rd %s - team abbrevation
// 4th %s - new tactic
["CHANGETACTIC", "\nMin. %s :(%s) %s will now play %s"],
// A foul
// 1st %s - minute
// 2nd %s - team abbrevation
// 3rd %s - fouling player
["FOUL", "\nMin. %s :(%s) %s with the foul"],
["FOUL", "\nMin. %s :(%s) %s with a nasty foul"],
["FOUL", "\nMin. %s :(%s) %s fouls badly"],
["FOUL", "\nMin. %s :(%s) %s with a bad challenge"],
["FOUL", "\nMin. %s :(%s) %s with an ugly foul"],
// A penalty was a awarded
// 1st %s - player taking the penalty
["PENALTY", "\n          ...  PENALTY !! \n          ...  %s will take it"],
["PENALTY", "\n          ...  PENALTY !! \n          ...  %s takes it"],
["PENALTY", "\n          ...  PENALTY !! \n          ...  %s with the spot kick"],
// The fouling player has been warned by the referee
["WARNED", "\n          ...  He is warned by the ref"],
["WARNED", "\n          ...  The referee calls him for a talk"],
["WARNED", "\n          ...  The ref warns him"],
// The fouling player has been given a yellow card
["YELLOWCARD", "\n          ...  He gets a yellow card"],
["YELLOWCARD", "\n          ...  Shown a yellow card"],
["YELLOWCARD", "\n          ...  He is booked for that one"],
// The fouling player has been given a 2nd yellow card (and was sent off)
["SECONDYELLOWCARD", "\n          ...  It's his second ! Sent off !"],
["SECONDYELLOWCARD", "\n          ...  His 2nd yellow this game ! Sent off !"],
["SECONDYELLOWCARD", "\n          ...  His 2nd yellow this game ! Sent off !"],
// The fouling player has been given a red card
["REDCARD", "\n          ...  The ref sends him off the pitch !!"],
["REDCARD", "\n          ...  It's a red card !! End of the game for him"],
["REDCARD", "\n          ...  Shown a red card !!"],
// Various statistic and informational lines printed in the
// commentary file
["COMM_KICKOFF",  "\n*************  KICK OFF  *****************"],
["COMM_HALFTIME", "\n*************  HALF TIME  ****************"],
["COMM_FULLTIME", "\n*************  FULL TIME  ****************"],
["COMM_SHOTSOFFTARGET", "\nShots off target"],
["COMM_SHOTSONTARGET", "\nShots on target"],
["PENALTYSHOOTOUT", "\n***********  PENALTY SHOOTOUT  ***********"],
["WONPENALTYSHOOTOUT", "\n\n          ...  %s wins the penalty shootout"],
["COMM_SCORE", "\nScore"],
["COMM_INJURYTIME", "\nThe ref adds %s min. of injury time"],
["COMM_STATISTICS", "Player statistics for: %s"],
// For updtr
["UPDTR_SKILL_INCREASE", "%s (%s) %s increases"],
["UPDTR_SKILL_DECREASE", "%s (%s) %s decreases"],
["UPDTR_SUSPENDED_1", "%s (%s) is suspended for one game"],
["UPDTR_SUSPENDED_N", "%s (%s) is suspended for %d games"],
["UPDTR_INJURY_NONE", "%s (%s) shakes off a minor injury"],
["UPDTR_INJURY_NONE", "%s (%s) got a doctor's permission to play"],
["UPDTR_INJURY_NONE", "%s (%s) passed a fitness test"],
["UPDTR_INJURY_1", "%s (%s) is injured for one week with a bruised knee"],
["UPDTR_INJURY_1", "%s (%s) is injured for one week with a bruised hand"],
["UPDTR_INJURY_1", "%s (%s) is injured for one week with a gashed hip"],
["UPDTR_INJURY_LIGHT", "%s (%s) is injured for %d weeks with a groin strain"],
["UPDTR_INJURY_LIGHT", "%s (%s) is injured for %d weeks with a sprained knee"],
["UPDTR_INJURY_LIGHT", "%s (%s) is injured for %d weeks with a sprained ankle"],
["UPDTR_INJURY_LIGHT", "%s (%s) is injured for %d weeks with a twisted ankle"],
["UPDTR_INJURY_LIGHT", "%s (%s) is injured for %d weeks with a knee inflammation"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a torn hamstring"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a broken arm"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a ruptured ligament"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a torn achilles tendon"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a broken ankle"],
["UPDTR_INJURY_HARD", "%s (%s) is injured for %d weeks with a torn knee cartilage"],
["UPDTR_END_SUSPENSION", "%s (%s) is coming back from suspension"],
["UPDTR_END_INJURY", "%s (%s) is coming back from injury"]
]);
Object.freeze(
testRoster = [ 
  { name: 'U_Avrovroo',
    nationality: 'arg',
    age: 19,
    pref_side: 'RLC',
    st: 11,
    tk: 3,
    ps: 3,
    sh: 3,
    stamina: 39,
    ag: 38,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'U_Cledsedroe',
    nationality: 'eng',
    age: 28,
    pref_side: 'C',
    tk: 14,
    st: 3,
    ps: 5,
    sh: 7,
    stamina: 56,
    ag: 38,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'V_Iblakro',
    nationality: 'bra',
    age: 24,
    pref_side: 'LC',
    tk: 15,
    st: 3,
    ps: 8,
    sh: 7,
    stamina: 82,
    ag: 28,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'N_Prutri',
    nationality: 'hol',
    age: 19,
    pref_side: 'R',
    ps: 13,
    tk: 10,
    st: 3,
    sh: 7,
    stamina: 60,
    ag: 34,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'D_Bribs',
    nationality: 'ire',
    age: 20,
    pref_side: 'C',
    ps: 10,
    tk: 11,
    st: 3,
    sh: 5,
    stamina: 58,
    ag: 37,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'C_Zasa',
    nationality: 'den',
    age: 21,
    pref_side: 'R',
    ps: 9,
    tk: 11,
    st: 3,
    sh: 7,
    stamina: 43,
    ag: 23,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'Y_Stednuvr',
    nationality: 'spa',
    age: 19,
    pref_side: 'R',
    ps: 15,
    st: 3,
    tk: 9,
    sh: 7,
    stamina: 79,
    ag: 33,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'N_Ovruartoa',
    nationality: 'den',
    age: 20,
    pref_side: 'C',
    ps: 17,
    st: 3,
    tk: 6,
    sh: 7,
    stamina: 39,
    ag: 24,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'T_Ednam',
    nationality: 'saf',
    age: 23,
    pref_side: 'LC',
    ps: 17,
    st: 3,
    tk: 8,
    sh: 6,
    stamina: 39,
    ag: 25,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'V_Uvre',
    nationality: 'ire',
    age: 29,
    pref_side: 'R',
    ps: 11,
    st: 3,
    tk: 7,
    sh: 7,
    stamina: 75,
    ag: 37,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'J_Kavowpri',
    nationality: 'nor',
    age: 22,
    pref_side: 'L',
    ps: 13,
    sh: 10,
    tk: 8,
    st: 3,
    stamina: 81,
    ag: 28,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'M_Inya',
    nationality: 'bul',
    age: 21,
    pref_side: 'R',
    ps: 13,
    sh: 10,
    tk: 7,
    st: 3,
    stamina: 60,
    ag: 31,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'I_Orsecli',
    nationality: 'ger',
    age: 26,
    pref_side: 'R',
    ps: 13,
    sh: 9,
    tk: 7,
    st: 3,
    stamina: 49,
    ag: 23,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'S_Ogra',
    nationality: 'fra',
    age: 19,
    pref_side: 'R',
    ps: 11,
    sh: 11,
    tk: 9,
    st: 3,
    stamina: 69,
    ag: 35,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'Z_Shoz',
    nationality: 'bra',
    age: 26,
    pref_side: 'C',
    ps: 11,
    sh: 11,
    tk: 8,
    st: 3,
    stamina: 46,
    ag: 38,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'N_Ojhuryoeg',
    nationality: 'isr',
    age: 24,
    pref_side: 'RLC',
    sh: 17,
    st: 3,
    tk: 8,
    ps: 7,
    stamina: 70,
    ag: 22,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'K_Ovroodo',
    nationality: 'ger',
    age: 25,
    pref_side: 'RC',
    sh: 14,
    st: 3,
    tk: 5,
    ps: 8,
    stamina: 61,
    ag: 37,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'H_Kruvro',
    nationality: 'arg',
    age: 26,
    pref_side: 'C',
    sh: 14,
    st: 3,
    tk: 7,
    ps: 7,
    stamina: 47,
    ag: 26,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'P_Eblo',
    nationality: 'ger',
    age: 19,
    pref_side: 'R',
    sh: 12,
    st: 3,
    tk: 5,
    ps: 7,
    stamina: 43,
    ag: 32,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'H_Vepriks',
    nationality: 'ger',
    age: 24,
    pref_side: 'R',
    sh: 14,
    st: 3,
    tk: 7,
    ps: 7,
    stamina: 45,
    ag: 30,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 },
  { name: 'S_Omno',
    nationality: 'bul',
    age: 24,
    pref_side: 'L',
    sh: 15,
    st: 3,
    tk: 6,
    ps: 9,
    stamina: 38,
    ag: 30,
    st_ab: 300,
    tk_ab: 300,
    ps_ab: 300,
    sh_ab: 300,
    games: 0,
    saves: 0,
    tackles: 0,
    keypasses: 0,
    shots: 0,
    goals: 0,
    assists: 0,
    dp: 0,
    injury: 0,
    suspension: 0,
    fitness: 100 } ]);
Object.freeze(teamsheet = {
    teamName: 'Apes United',
    tactic: 'N',
    PK: 'N_Ojhuryoeg',
    Selection: 
    [   ,
        { pos: 'GK', name: 'U_Avrovroo' },
        { pos: 'DFC', name: 'V_Iblakro' },
        { pos: 'DFC', name: 'U_Cledsedroe' },
        { pos: 'DFC', name: 'D_Bribs' },
        { pos: 'DFC', name: 'C_Zasa' },
        { pos: 'MFC', name: 'N_Ovruartoa' },
        { pos: 'MFC', name: 'T_Ednam' },
        { pos: 'MFC', name: 'Y_Stednuvr' },
        { pos: 'MFC', name: 'N_Prutri' },
        { pos: 'FWC', name: 'N_Ojhuryoeg' },
        { pos: 'FWC', name: 'S_Omno' },
        { name: 'V_Uvre', pos: 'GK' },
        { pos: 'DFC', name: 'S_Ogra' },
        { pos: 'MFC', name: 'J_Kavowpri' },
        { pos: 'DFC', name: 'Z_Shoz' },
        { pos: 'FWC', name: 'K_Ovroodo' } ]
        // optional conditions
});
Object.freeze(statsDat = {
    "shotsOffTarget": [ 6, 5 ],
    "shotsOnTarget": [ 11, 7 ],
    "scores": [ 6, 2 ],
    "teamStatistics": [
    {
    "U_Avrovroo": [ "GK", "RLC", 11, 3, 3, 3, 39, 90, 4, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 100 ],
    "V_Iblakro": [ "DFC", "LC", 3, 15, 8, 7, 82, 90, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82 ],
    "U_Cledsedroe": [ "DFC", "C", 3, 14, 5, 7, 56, 90, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 73 ],
    "D_Bribs": [ "DFC", "C", 3, 11, 10, 5, 58, 90, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 72 ],
    "C_Zasa": [ "DFC", "R", 3, 11, 9, 7, 43, 90, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69 ],
    "N_Ovruartoa": [ "MFC", "C", 3, 6, 17, 7, 39, 88, 0, 0, 3, 2, 0, 0, 0, 0, 1, 0, 60, 130, 60, 68 ],
    "T_Ednam": [ "MFC", "LC", 3, 8, 17, 6, 39, 90, 0, 1, 6, 2, 0, 0, 0, 0, 0, 0, 0, 70, 0, 69 ],
    "Y_Stednuvr": [ "MFC", "R", 3, 9, 15, 7, 79, 90, 0, 1, 6, 1, 0, 0, 0, 0, 0, 0, 60, 95, 60, 79 ],
    "N_Prutri": [ "MFC", "R", 3, 10, 13, 7, 60, 90, 0, 0, 4, 0, 1, 0, 1, 0, 0, 0, -8, -8, -8, 73 ],
    "N_Ojhuryoeg": [ "FWC", "RLC", 3, 8, 7, 17, 70, 90, 0, 0, 0, 0, 6, 2, 0, 0, 0, 0, 0, 0, 108, 79 ],
    "S_Omno": [ "FWC", "L", 3, 6, 9, 15, 38, 90, 0, 0, 2, 0, 8, 4, 0, 0, 0, 0, 0, 0, 214, 66 ],
    "V_Uvre": [ "GK", "R", 3, 7, 11, 7, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "S_Ogra": [ "DFC", "R", 3, 9, 11, 11, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "J_Kavowpri": [ "MFC", "L", 3, 8, 13, 10, 81, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 98 ],
    "Z_Shoz": [ "DFC", "C", 3, 8, 11, 11, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "K_Ovroodo": [ "FWC", "RC", 3, 5, 8, 14, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ]
    },
    {
    "U_Avrovroo": [ "GK", "RLC", 11, 3, 3, 3, 39, 90, 5, 0, 0, 0, 0, 0, 0, 0, 0, -48, 0, 0, 0, 100 ],
    "V_Iblakro": [ "DFC", "LC", 3, 15, 8, 7, 82, 45, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -20, -20, -20, 93 ],
    "U_Cledsedroe": [ "DFC", "C", 3, 14, 5, 7, 56, 90, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75 ],
    "D_Bribs": [ "DFC", "C", 3, 11, 10, 5, 58, 90, 0, 3, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 75 ],
    "C_Zasa": [ "DFC", "R", 3, 11, 9, 7, 43, 90, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68 ],
    "N_Ovruartoa": [ "MFC", "C", 3, 6, 17, 7, 39, 90, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65 ],
    "T_Ednam": [ "MFC", "LC", 3, 8, 17, 6, 39, 90, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66 ],
    "Y_Stednuvr": [ "MFC", "R", 3, 9, 15, 7, 79, 90, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 82 ],
    "N_Prutri": [ "MFC", "R", 3, 10, 13, 7, 60, 90, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74 ],
    "N_Ojhuryoeg": [ "FWC", "RLC", 3, 8, 7, 17, 70, 90, 0, 0, 0, 0, 7, 1, 1, 0, 0, 0, -68, -68, -8, 82 ],
    "S_Omno": [ "FWC", "L", 3, 6, 9, 15, 38, 90, 0, 0, 0, 0, 3, 1, 1, 0, 0, 0, -8, -8, 44, 67 ],
    "V_Uvre": [ "GK", "R", 3, 7, 11, 7, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "S_Ogra": [ "DFC", "R", 3, 9, 11, 11, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "J_Kavowpri": [ "MFC", "L", 3, 8, 13, 10, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "Z_Shoz": [ "DFC", "C", 3, 8, 11, 11, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ],
    "K_Ovroodo": [ "FWC", "RC", 3, 5, 8, 14, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 ]
    }
    ],
    "teamTotals": [
    [ 4, 4, 22, 5, 17, 6, 1, 0, 1 ],
    [ 5, 8, 11, 0, 12, 2, 2, 1, 0 ]
    ],
    "teamProfiles": [
        [
        [ 0, 52.96137487199999, 64.72878468, 40.60699583899999 ],
        [ 10, 51.70652302499999, 62.9848608, 39.595624504999996 ],
        [ 20, 50.389022895000004, 61.095497325, 38.44149008 ],
        [ 30, 48.839371904999986, 59.099908649999996, 37.346240390000006 ],
        [ 40, 47.51671684499999, 57.01900964999999, 36.148376585 ],
        [ 50, 45.779013189, 55.07855780999999, 35.010173168 ],
        [ 60, 44.330694503999986, 53.05378910999998, 33.749356553 ],
        [ 70, 42.83605605899999, 51.19269125999998, 32.670719917999996 ],
        [ 80, 41.57932748399999, 49.27251470999999, 31.708759598 ],
        [ 90, 40.83480162599999, 45.963092759999995, 31.262698873999998 ]],
        [
        [ 0, 52.92528280199999, 64.657302405, 40.586943088999995 ],
        [ 10, 51.617289330000006, 62.8330614, 39.708945379999996 ],
        [ 20, 50.382682845, 61.02675735000001, 38.80664723 ],
        [ 30, 49.316078235, 59.244753075, 37.914349385 ],
        [ 40, 48.117424499999984, 57.53179905, 36.839535245 ],
        [ 50, 38.375922749, 53.31764966000001, 34.475614604 ],
        [ 60, 36.965720813999994, 51.26540145999999, 33.422649794 ],
        [ 70, 35.751592684, 49.33118690999999, 32.32988564899999 ],
        [ 80, 34.49249135899999, 47.488819310000004, 31.363398778999997 ],
        [ 90, 33.204422498999996, 45.65689111, 30.29729375899999 ]
        ]],
    "commentary": [
        "Home                           Away\n",
        "----                           ----\n\n",
        "                                                             \n\n",
        "GK  U_Avrovroo                 GK  U_Avrovroo                \n",
        "DFC V_Iblakro                  DFC V_Iblakro                 \n",
        "DFC U_Cledsedroe               DFC U_Cledsedroe              \n",
        "DFC D_Bribs                    DFC D_Bribs                   \n",
        "DFC C_Zasa                     DFC C_Zasa                    \n",
        "MFC N_Ovruartoa                MFC N_Ovruartoa               \n",
        "MFC T_Ednam                    MFC T_Ednam                   \n",
        "MFC Y_Stednuvr                 MFC Y_Stednuvr                \n",
        "MFC N_Prutri                   MFC N_Prutri                  \n",
        "FWC N_Ojhuryoeg                FWC N_Ojhuryoeg               \n",
        "FWC S_Omno                     FWC S_Omno                    \n",
        "4-4-2 Normal                   ",
        "4-4-2 Normal                   ",
        "\n*************  KICK OFF  *****************",
        "\nMin.  6 :() N_Ovruartoa with a smart pass to S_Omno",
        "\n          ...  S_Omno tries to chip the ball over the keeper !",
        "\n          ...  GOAL !!",
        "\n          ...   1-0  ...",
        "\nMin.  9 :() N_Ojhuryoeg takes advantage of a defensive mistake",
        "\n          ...  N_Ojhuryoeg chases it through, he must score !",
        "\n          ...  GOAL !!",
        "\n          ...   2-0  ...",
        "\nMin. 10 :() S_Omno finds N_Ojhuryoeg in the box",
        "\n          ...  Intercepted by D_Bribs",
        "\nMin. 10 :() Y_Stednuvr cuts the ball back to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg with the shot !",
        "\n          ...  GOAL !!",
        "\n          ...  But it's been disallowed. The linesman flags for offside",
        "\nMin. 11 :() N_Ojhuryoeg cuts through the defense",
        "\n          ...  But T_Ednam clears the danger",
        "\nMin. 12 :() Y_Stednuvr finds N_Ojhuryoeg in the box",
        "\n          ...  But N_Ovruartoa reads the situation well and wins the ball",
        "\nMin. 12 :() N_Ojhuryoeg with the foul",
        "\n          ...  He is booked for that one",
        "\nMin. 14 :() T_Ednam heads the ball down to N_Ojhuryoeg",
        "\n          ...  Intercepted by U_Cledsedroe",
        "\nMin. 15 :() N_Ojhuryoeg takes possesion",
        "\n          ...  N_Ojhuryoeg tries to chip the ball over the keeper !",
        "\n          ...  GOAL !!",
        "\n          ...   2-1  ...",
        "\nMin. 16 :() N_Ovruartoa with a smart pass to S_Omno",
        "\n          ...  A vicious shot by S_Omno !",
        "\n          ...  GOAL !!",
        "\n          ...   3-1  ...",
        "\nMin. 17 :() U_Cledsedroe with the dribble",
        "\n          ...  A vicious shot by U_Cledsedroe !",
        "\n          ...  But it goes wide of the post",
        "\nMin. 19 :() T_Ednam heads the ball down to S_Omno",
        "\n          ...  S_Omno shoots towards goal !",
        "\n          ...  Over the bar",
        "\nMin. 20 :() U_Cledsedroe plays a long ball to S_Omno",
        "\n          ...  S_Omno tries to beat the keeper !",
        "\n          ...  Wide of goal",
        "\nMin. 21 :() V_Iblakro heads the ball down to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg chases it through, he must score !",
        "\n          ...  But U_Avrovroo makes a fine save",
        "\nMin. 26 :() Y_Stednuvr with a glorious long pass to N_Ojhuryoeg",
        "\n          ...  C_Zasa gets in the way and wins the ball",
        "\nMin. 29 :() Y_Stednuvr passes the ball to S_Omno",
        "\n          ...  A vicious shot by S_Omno !",
        "\n          ...  GOAL !!",
        "\n          ...   4-1  ...",
        "\nMin. 31 :() T_Ednam with a smart pass to S_Omno",
        "\n          ...  S_Omno tries to chip the ball over the keeper !",
        "\n          ...  U_Avrovroo gathers it comfortably",
        "\nMin. 31 :() D_Bribs passes the ball to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg with the strike !",
        "\n          ...  U_Avrovroo makes a good save",
        "\nMin. 32 :() T_Ednam with the heel pass to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg with the shot !",
        "\n          ...  GOAL !!",
        "\n          ...   5-1  ...",
        "\nMin. 33 :() Y_Stednuvr with a smart pass to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg goes for goal !",
        "\n          ...  But U_Avrovroo with the difficult save",
        "\nMin. 37 :() Y_Stednuvr passes the ball to N_Ojhuryoeg",
        "\n          ...  Blocked by D_Bribs",
        "\nMin. 37 :() V_Iblakro fouls badly",
        "\n          ...  Shown a yellow card",
        "\nMin. 39 :() T_Ednam finds S_Omno in the box",
        "\n          ...  Intercepted by V_Iblakro",
        "\nMin. 40 :() C_Zasa with an ugly foul",
        "\n          ...  The ref warns him",
        "\nMin. 41 :() N_Prutri with a smart pass to N_Ojhuryoeg",
        "\n          ...  But C_Zasa clears the ball to safety",
        "\nMin. 43 :() S_Omno plays a long ball to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg with the shot !",
        "\n          ...  U_Avrovroo makes a good save",
        "\nThe ref adds 2 min. of injury time",
        "\nMin. 46 :() V_Iblakro with the foul",
        "\n          ...  Shown a yellow card",
        "\n          ...  His 2nd yellow this game ! Sent off !",
        "\nMin. 47 :() T_Ednam heads the ball down to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg is one of one with the keeper, he shoots !",
        "\n          ...  U_Avrovroo makes a good save",
        "\n*************  HALF TIME  ****************",
        "\nMin. 47 :() T_Ednam plays a long ball to S_Omno",
        "\n          ...  S_Omno is one of one with the keeper, he shoots !",
        "\n          ...  U_Avrovroo makes a comfortable save",
        "\nMin. 47 :() N_Ovruartoa finds Y_Stednuvr in the box",
        "\n          ...  Y_Stednuvr tries to chip the ball over the keeper !",
        "\n          ...  But U_Avrovroo with the difficult save",
        "\nMin. 52 :() T_Ednam plays a long ball to S_Omno",
        "\n          ...  S_Omno with the shot !",
        "\n          ...  GOAL !!",
        "\n          ...   6-1  ...",
        "\nMin. 53 :() Y_Stednuvr slides the ball across to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg is one of one with the keeper, he shoots !",
        "\n          ...  Wide of goal",
        "\nMin. 55 :() N_Prutri heads the ball down to S_Omno",
        "\n          ...  But D_Bribs wins the tackle",
        "\nMin. 55 :() D_Bribs finds N_Ojhuryoeg in the box",
        "\n          ...  N_Ojhuryoeg goes for goal !",
        "\n          ...  Over the bar",
        "\nMin. 61 :() N_Prutri finds a hole in the defense",
        "\n          ...  But C_Zasa reads the situation well and wins the ball",
        "\nMin. 62 :() Y_Stednuvr plays a long ball to D_Bribs",
        "\n          ...  A vicious shot by D_Bribs !",
        "\n          ...  But he puts it wide",
        "\nMin. 65 :() N_Ovruartoa with a precise pass to N_Prutri",
        "\n          ...  Cleared by C_Zasa",
        "\nMin. 66 :() N_Prutri beats his marker",
        "\n          ...  N_Prutri tries to chip the ball over the keeper !",
        "\n          ...  But it whistles just past the post",
        "\nMin. 67 :() N_Ovruartoa passes the ball to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg shoots towards goal !",
        "\n          ...  But it goes wide",
        "\nMin. 75 :() N_Prutri passes the ball to D_Bribs",
        "\n          ...  D_Bribs with the strike !",
        "\n          ...  Wide of goal",
        "\nMin. 75 :() S_Omno finds himself in a good position",
        "\n          ...  S_Omno chases it through, he must score !",
        "\n          ...  GOAL !!",
        "\n          ...   6-2  ...",
        "\nMin. 77 :() N_Prutri fouls badly",
        "\n          ...  Shown a yellow card",
        "\nMin. 81 :() N_Prutri passes the ball to N_Ojhuryoeg",
        "\n          ...  N_Ojhuryoeg with the strike !",
        "\n          ...  But it goes wide of the post",
        "\nMin. 82 :() S_Omno fouls badly",
        "\n          ...  He gets a yellow card",
        "\nMin. 86 :() U_Cledsedroe with a nasty foul",
        "\n          ...  The referee calls him for a talk",
        "\nMin. 88 :() N_Prutri cuts the ball back to S_Omno",
        "\n          ...  Y_Stednuvr gets in the way and wins the ball",
        "\nMin. 89 :() N_Ovruartoa falls badly. He's unable to continue",
        "\nMin. 89 :() J_Kavowpri will come on for N_Ovruartoa and play MFC",
        "\nThe ref adds 3 min. of injury time",
        "\nMin. 92 :() D_Bribs plays a long ball to S_Omno",
        "\n          ...  S_Omno chases it through, he must score !",
        "\n          ...  Saved by U_Avrovroo",
        "\nMin. 92 :() S_Omno with a real burst of pace",
        "\n          ...  S_Omno goes for goal !",
        "\n          ...  But it goes just over",
        "\n*************  FULL TIME  ****************"
    ]
});
