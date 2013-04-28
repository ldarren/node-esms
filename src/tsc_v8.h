// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#ifndef TSC_V8_H
#define TSC_V8_H

bool parse_formation(char* formation, int& dfs, int& mfs, int& fws, char* tactic);
bool verify_position_range(int n);

#endif /* TSC_V8_H */
