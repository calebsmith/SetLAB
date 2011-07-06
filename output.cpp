#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "setclass.h"
#include "slist.h"
#include "settings.h"

#include "constants.h"
#include "functions.h"

bool output_commands(Program_settings& settings, String_list& input, Page& setclass){
    //format input into a command and arguments
    int length = input.length();
    if (length < 1) return 1;
    
    //obtain command and arguments. Arguments are taken as a list of integers
    int args = length - 1;
    int* arg;
    arg = new int[args];
    string* str_arg;
    str_arg = new string[args];
    
    string command = input.get(0);
    arg = input.get_int_args();
    str_arg = input.get_str_args();
    
    //general status commands
    if (args == 0){
        if (command == "status" || command == "stat"){ setclass.stat(); return 1;}
        if (command == "list" || command == "lists"){ setclass.list(); return 1;}
        if (command == "set"){ setclass.set(); return 1;}
        if (command == "show" || command == "ls" || command == "look"){
            setclass.show();
            return 1;
        }
    }
    
    int varvalue;
    string varname;
    int i;
    //to see one attribute
    if (args > 0){
        if(command == "print" || command == "show"){
            for (i = 0; i < args; i++){
                varname = str_arg[i];
                if (varname == "tabs") varvalue = TABS;
                if (varname == "tab") varvalue = TAB;
                if (varname == "sets") varvalue = SETS;
                if (varname == "set") varvalue = SET;
                if (varname == "pitches") varvalue = PITCHES;
                if (varname == "pcs") varvalue = PCS;
                if (varname == "setint") varvalue = SETINT;
                if (varname == "scint") varvalue = SCINT;
                if (varname == "setbin") varvalue = SETBIN;
                if (varname == "scbin") varvalue = SCBIN;
                if (varname == "input") varvalue = INPUT;
                if (varname == "sc") varvalue = SC;
                if (varname == "fname") varvalue = FNAME;
                if (varname == "icv") varvalue = ICV;
                if (varname == "compliment") varvalue = COMPLIMENT;
                if (varname == "mpartner") varvalue = MPARTNER;
                if (varname == "zpartner") varvalue = ZPARTNER;
                
                if (varvalue > NONE) setclass.show(varvalue);
            }
            return 1;
        }
        //output from scripts can send any message using echo
        if (command == "echo"){
            for(i = 0; i < args; i++){
                if (i > 0 || str_arg[0] != "on") cout << str_arg[i] << " ";
            }
            cout << '\n';
            return 1;
        }
    }
    return 0;
}   
