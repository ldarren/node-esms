// ESMS - Electronic Soccer Management Simulator
// Copyright (C) <1998-2005>  Eli Bendersky
//
// This program is free software, licensed with the GPL (www.fsf.org)
// 
#include <v8.h>

#include <map>
#include <vector>
#include <string>
#include <cstdarg>
#include <algorithm>

#include "comment_v8.h"
#include "object_v8.h"
#include "util.h"

using namespace std;
using namespace v8;


// get a reference to a static commentary (a singleton)
//
commentary& the_commentary(void)
{
    static commentary tcfg;
    return tcfg;
}


// Initializes commentary data, reading it from the language.dat file
//
void commentary::init_commentary(Handle<Object> languageDat)
{
    HandleScope scope;
    // Read languageDat line by line, updating the
    // commentary database
    //
    const Local<Array> props = languageDat->GetOwnPropertyNames();
    const uint32_t length = props->Length();
    Local<String> key;
    Local<Array> value;
    Local<String> event, comment;
    char strKey[64];
    char strValue[128];

    for (uint32_t i=0 ; i<length ; ++i) {
        key = props->Get(i)->ToString();
        value = Local<Array>::Cast(languageDat->Get(key));

        event = value->Get(0)->ToString();

        comment = value->Get(1)->ToString();

        // Add line to the commentary database 
        //
        comm_data[toAscii(event, strKey)].push_back(toAscii(comment, strValue));
    }
}


string commentary::rand_comment(const char* event, ...)
{
    va_list arglist;
    va_start(arglist, event);

    // How many commentary lines of this type are available ?
    //
    string str_event = event;
    int num_of_choices = comm_data[str_event].size();

    if (num_of_choices < 1)
        die("No commentary choices found for event %s", event);

    // Pick one of the possible commentaries randomly
    //
    int choice_num = rand() % num_of_choices;
    string comm_format = comm_data[str_event][choice_num];

    char* buf = new char[4096];
    vsprintf(buf, comm_format.c_str(), arglist);

    string ret;

    // Convert the '\n's in the string into "real" newlines
    // which are recognized by the printing functions
    //
    char* s1 = buf;

    for (; *s1; ++s1)
    {
        if ((*s1 == '\\') && (*(s1+1) == 'n'))
        {
            ret += '\n';
            ++s1;
        }
        else
            ret += *s1;
    }

    delete [] buf;
    return ret;
}


