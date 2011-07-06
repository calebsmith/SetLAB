#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "setclass.h"
#include "slist.h"
#include "settings.h"

#include "functions.h"
#include "constants.h"

void command_handler(Program_settings& settings, String_list& input, Page& setclass){
    bool valid_in, valid_out;
    
    valid_in = input_commands(settings, input, setclass);
    valid_out = output_commands(settings, input, setclass);
   
    if (valid_in == 0 && valid_out == 0){
        cout << "Invalid command" << '\n';
        return;
    }
    if (settings.get_autolook() == 1) setclass.show();
    return;
}

bool input_commands(Program_settings& settings, String_list& input, Page& setclass){
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
    
    int i, i2;
    int allsets, alltabs;
    int holdset, holdtab;
    string remove; //to remove a command prefix
    
         
    if (args == 0){
        //pc manipulation
        if (command == "invert" || command == "i"){setclass.invert(); return 1;}
        if (command == "multiply" || command == "m"){setclass.multiply(); return 1;}
        
        if (command == "clear"){ setclass.set_clear(); return 1;}
        if (command == "c" || command == "compliment"){ setclass.set_from_c(); return 1;}
        if (command == "z" || command == "z-partner"){ setclass.set_from_z(); return 1;}
        if (command == "forcez"){setclass.zpartner(); return 1;}
        //set manipulation
        if (command == "new" || command == "newset" || command == "n"){
            setclass.add_set();
            return 1;
        }
        if (command == "next" || command == "nextset"){setclass.next_set(); return 1;}
        if (command == "prev" || command == "prevset"){setclass.prev_set(); return 1;}
        //tab manipulation
        if (command == "newtab"){setclass.add_tab(); return 1;}
        if (command == "left" || command == "prevtab"){setclass.prev_tab(); return 1;}
        if (command == "right" || command == "nexttab"){setclass.next_tab(); return 1;}
        //file manipulation
        if (command == "save"){setclass.save(settings.get_filename()); return 1;}
        if (command == "load"){setclass.load(settings.get_filename()); return 1;}
    }
    if (args > 0){
        if (command == "add"){            
            for(i = 0; i < args; i++){
                setclass.add(arg[i]);
            }            
            return 1;
        }
        if (command == "remove" || command == "rm"){
            for(i = 0; i < args; i++){
                setclass.remove(arg[i]);
            }           
            return 1;
        }
        if (command == "insert" && args > 1) {
            if (arg[args - 1] < 0) arg[args - 1] += setclass.get_length() + 1;
            for(i = 0; i < args - 1; i++){     
                setclass.insert(arg[i], arg[args - 1] + i);
            }
            return 1;
        }
        //modify all sets in group with all
        if (command == "all"){
            remove = input.lpop();
            allsets = setclass.get_size();
            holdset = setclass.get_iter();
            for(i = 0; i < allsets; i++){
                setclass.select_set(i);
                command_handler(settings, input, setclass);
            }
            setclass.select_set(holdset);
            return 1;
        }
        if (command == "global"){
            remove = input.lpop();
            alltabs = setclass.get_tab_size();
            holdtab = setclass.get_tab_number();//keep the current focus for later
            holdset = setclass.get_iter();
            for(i = 0; i < alltabs; i++){
                setclass.select_tab(i);
                setclass.select_set(0);
                allsets = setclass.get_size();
                for(i2 = 0; i2 < allsets; i2++){
                    setclass.select_set(i2);
                    command_handler(settings, input, setclass);
                }
            }
            setclass.select_tab(holdtab);//return the focus to the current tab and set
            setclass.select_set(holdset);
            return 1;
        }
    }
    if (args == 1){
        //scripts
        if (command == "script" || command == "run") {
            script_handler(settings, input, setclass);
	        return 1;
        }
        //pc manipulation
        if (command == "transpose" || command == "t"){
            setclass.transpose(arg[0]);
            return 1;
        }
        //set manipulation  
        if (command == "set" || command == "changeset"){
            setclass.select_set(arg[0]);
            return 1;
        }     
        if (command == "equals" || command == "equal"){
            setclass.equals_set(setclass.get_iter(),arg[0]);
            return 1;
        }
        if (command == "rmset" || command == "removeset"){
            setclass.remove_set(arg[0]);
            return 1;
        }
        if (command == "insertset"){
            setclass.insert_set(arg[0]);
            return 1;
        }
        if (command == "mod" || command == "modulus"){
            setclass.mod(arg[0]);
            return 1;
        } 
        if (command == "canon"){
            setclass.canon(arg[0]);
            return 1;
        }
        if (command == "mvalue"){
            setclass.m_value(arg[0]);
            return 1;
        }
        if (command == "ordered"){
            if (arg[0] == 0 || str_arg[0] == "false" || str_arg[0] == "off"){
                setclass.order(0);
                return 1;
            }
            if (arg[0] == 1 || str_arg[0] == "true" || str_arg[0] == "on"){
                setclass.order(1);
                return 1;
            }
        }
        if (command == "multiset" || command == "multi" || command == "multisets"){
            if (arg[0] == 0 || str_arg[0] == "false" || str_arg[0] == "off"){
                setclass.multi(0);
                return 1;
            }
            if (arg[0] == 1 || str_arg[0] == "true" || str_arg[0] == "on"){
                setclass.multi(1);
                return 1;
            }
        }
		if (command == "base"){setclass.base(arg[0]); return 1;}		
        
        if (command == "fname" || command == "forte" || command == "fortename"){
            setclass.set_from_fname(str_arg[0]);
            return 1;
        }
        if (command == "decimal" || command == "dec"){ 
            setclass.set_from_int(arg[0]); 
            return 1;
        }       
        //tab manipulation
        if (command == "tab"){
            setclass.select_tab(arg[0]);
            return 1;
        }
        if (command == "rmtab" || command == "removetab"){
            setclass.remove_tab(arg[0]);
            return 1;
        }
        if (command == "inserttab"){ setclass.insert_tab(arg[0]); return 1;}
        
        //file manipulation
        if (command == "save"){
            string name = str_arg[0];
            settings.set_filename(name);
            setclass.save(name); 
            return 1;
        }
        if (command == "load"){
            string name = str_arg[0];
            settings.set_filename(name);
            setclass.load(name); 
            return 1;
        }
        
        //settings manipulation
        if (command == "prompt"){
            if (str_arg[0] == "off" || str_arg[0] == "false" || str_arg[0] == "0") settings.set_prompt(0);
            if (str_arg[0] == "on" || str_arg[0] == "true"  || str_arg[0] == "1") settings.set_prompt(1);
            return 1;
        }
        if (command == "autolook"){
            if (str_arg[0] == "off" || str_arg[0] == "false" || str_arg[0] == "0") settings.set_autolook(0);
            if (str_arg[0] == "on" || str_arg[0] == "true" || str_arg[0] == "1") settings.set_autolook(1);
            return 1;
        }
        if (command == "echo"){
            if (str_arg[0] == "off" || str_arg[0] == "false" || str_arg[0] == "0") settings.set_echo(0);
            if (str_arg[0] == "on" || str_arg[0] == "true" || str_arg[0] == "1") settings.set_echo(1);
            return 1;
        }
        
    }
    if (args == 2){  
        //allow for negative entries that indicate (length - argument) 
        for (i = 0; i < args; i++){
            if (arg[i] < 0) arg[i] += setclass.get_length();   
        }
        
        //pc manipulation
        if (command == "swap"){                 
            setclass.swap_p(arg[0], arg[1]);  
            return 1;
        }  
        //set manipulation
        if (command == "swapset"){        
            setclass.swap_set(arg[0], arg[1]); 
            return 1; 
        }    
        //tab manipulation
        if (command == "swaptab"){
            setclass.swap_tab(arg[0], arg[1]);
            return 1;
        }
    }    
    return 0;
}
