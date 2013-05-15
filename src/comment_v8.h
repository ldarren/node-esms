// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#ifndef COMMENT_V8_H
#define COMMENT_V8_H

#include <map>
#include <vector>
#include <string>

using namespace std;


class commentary
{
    public:
	void init_commentary(v8::Handle<v8::Object> languageDat);
	string rand_comment(const char* event, ...);

	friend commentary& the_commentary(void);

    private:
	commentary() {}
	commentary(const commentary& rhs);
	commentary& operator= (const commentary& rhs);

	map<string, vector<string> > comm_data;
};

commentary& the_commentary(void);



#endif // COMMENT_V8_H
