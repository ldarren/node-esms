var
leagueDat = {
    home_bonus: 200,
    dp_for_yellow: 4,
    dp_for_red: 10 ,
    suspension_margin: 10,
    max_injury_length: 9,
    num_subs: 5,
    substitutions: 3,
    updtr_fitness_gain: 20,
    updtr_fitness_after_injury: 80
},
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
},
leagueAbbreviations = {
    ape: "Apes_United",
    klm: "KLM_Royal_Club",
    uva: "Universidad_De_Valladolid",
    mac: "Machos_FC",
    bla: "Blade_Runners",
    esu: "ESMS_United",
    cra: "Crazy_Insomniacs"
},
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
    fitness: 100 } ];
