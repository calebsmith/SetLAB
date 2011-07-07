#ifndef SETTINGS_H
#define SETTINGS_H
#include <iostream>
#include <string>
class Program_settings{
    private:
        bool prompt, autolook, echo, showinvalid;
        string filename;

        
    public:
        String_list globals;
        Program_settings(){
            prompt = 1;
            autolook = 1;
            showinvalid = 1;
            echo = 0;
            filename = "unnamed";
            globals.parse("NONE TABS TAB SETS SET PITCHES PCS SETINT SCINT SETBIN SCBIN INPUT SC FNAME ICV COMPLIMENT MPARTNER ZPARTNER");
        }
        void set_prompt(int in){if (in < 0 || in > 1) return; prompt = in;}
        void set_autolook(int in){if (in < 0 || in > 1) return; autolook = in;}
        void set_echo(int in){if (in < 0 || in > 1) return; echo = in;}
        void set_showinvalid(int in){if (in < 0 || in > 1) return; showinvalid = in;}
        void set_filename(string in){filename = in; return;}

        
        bool get_autolook(){bool out; out = autolook; return out;}
        bool get_prompt(){bool out; out = prompt; return out;}
        bool get_echo(){bool out; out = echo; return out;}
        bool get_showinvalid(){bool out; out = showinvalid; return out;}
        string get_filename(){string out; out = filename; return out;}
};

#endif
