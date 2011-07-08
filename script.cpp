#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "setclass.h"
#include "slist.h"
#include "settings.h"
#include "classes.h"

#include "constants.h"
#include "functions.h"
using namespace std;

bool script_handler(Program_settings& settings, String_list& input, Page& setclass){
  
    String_list buffer, var_list;
    bool load_success;   
    string filename;
    input >> filename; 
    
    //load script into memory, return if file fails to open. 
    //load variable names into var_list
    load_success = load_script_file(filename, buffer, var_list);    
    if (load_success == 0) return 0;
    
    Variables var;    
    var.make(var_list);
    int vars = var.length(); 
   
    
    String_list global_list;
    global_list = settings.globals;
    
    //run the script
    int file_length = buffer.length();
   
    int length;
    
    string line;
    String_list proc_line;
    
    
    
    string name_in;
    string str_val_in;   
    
    string evaled_in;
    string var_in;
    int number_in;
    bool equation = 0;
    //iterate through each line in the buffer loaded from the script file
    int i, i2, i3;
    for(i = 0; i < file_length; i++){
        line = buffer.get(i);       //obtain the next line                
        proc_line.parse(line);      //parse the current line
        if (proc_line.get(0) == "end") break;   //exit if script command is "end" 
        
        //show line if echo on
        if (settings.get_echo() == 1) cout << line << '\n';      
        
        //replace variables and globals with their values (if not to the left of equals)        
        replace_vars(global_list, proc_line, setclass, var);
        
        //assign variables (x = var|global|int)
        equation = 0;
        equation = handle_equals(proc_line, var);   
               
        //reaching this point means the line contains a regular command.
        //execute that command    
        if (equation == 0){
            command_handler(settings, proc_line, setclass);
        }
    }
    
    return 1;
}
bool load_script_file(string filename, String_list& buffer, String_list& var_list){
    //convert filename to char* for opening the file
    char name [255] = "";
    stringstream scr_fn;
    scr_fn << filename;
    strcat (name, scr_fn.str().c_str());
    
    fstream fin;
    fin.open(name, ios_base::in);
        //attempt to open the file and report failure
        if (fin.fail()) {cout << "Failed to load script: " << filename << '\n'; return 0;}
               
       
        //load file into a buffer and count the number of variables
        int vars = 0;
        bool samename;
        String_list proc_line;
        string line, name_in;
        int i;
        while(fin.eof() != 1){
            getline(fin, line);     //obtain a line from the file
            buffer << line;         //store in buffer
            proc_line.parse(line);  //parse the line for processing
            
            //line added to buffer, now check for variable assignments
            if (proc_line.length() > 1){
                if (proc_line.get(1) == "="){
                    name_in = proc_line.get(0);
                    //check that the variable name is unique
                    samename = 0;
                    for(i = 0; i < vars; i++){                    
                        if (var_list.get(i) == name_in){
                            samename = 1;
                        }                          
                    }
                    //if the variable is unique, add the name to the list
                    if (samename == 0){
                        var_list << name_in;
                        vars++;
                    }
                }
            }
            if(line == "end") break;
        }
    fin.close();
    return 1;
}

//replace variables and globals with values
void replace_vars(String_list& globals, String_list& proc_line, 
    Page& setclass, Variables& var){
    
    string this_global;
    string global_str;
    int global_int;
    
    string str_val_in;
    int value_in;
    
    int global_total = globals.length();
    int length = proc_line.length();
    int vars = var.length();
    int i, i2;
    for(i = 1; i < length; i++){
        //find int and string equivalents to this argument
        str_val_in = proc_line.get(i);
        value_in = string_to_int(str_val_in);            
        
        //detect if argument is a number or a string
        if (value_in == 0 && str_val_in != "0"){
        
            //if variable is to the left of = skip this argument
            if(i < length - 1){
                if(proc_line.get(i + 1) == "=") continue;
            }
            //check for variables
            for(i2 = 0; i2 < vars; i2++){
                if(str_val_in == var.get_name(i2)){
                    proc_line.assign(i, int_to_string(var.get_value(i2)));                    
                    break;
                }
            }
            //check for globals
            for(i2 = 0; i2 < global_total; i2++){
                this_global = globals.get(i2);
                if(str_val_in == this_global){
                    global_int = find_global(this_global, setclass);
                    global_str = int_to_string(global_int);
                    proc_line.assign(i, global_str);
                    break;
                }
            }              
        }  
                 
    }
}
bool handle_equals(String_list& proc_line, Variables& var){
    string var_in, evaled_in;
    int number_in;
    
    int vars = var.length();
    int length = proc_line.length();
    bool isequation = 0;
    int i, i2;
    for(i = 0; i < length; i++){        
        if (proc_line.get(i) == "=" 
            && i > 0 && i + 1 < length){
        
            var_in = proc_line.get(i - 1);
            evaled_in = proc_line.get(i + 1);
            number_in = string_to_int(evaled_in);
            
            for(i2 = 0; i2 < vars; i2++){ 
            
                if (var_in == var.get_name(i2)){          
                    isequation = 1;          
                    var.set_value(i2, number_in);                    
                    break;
                }
            }
           
        } 
    }
    return isequation;    
}
string int_to_string(int int_in){
    stringstream val;
    string string_out;
    
    val << int_in;
    val >> string_out;
    return string_out;
}
int string_to_int(string str_in){
    stringstream val;
    int int_out;       
    
    val << str_in;
    val >> int_out;    
    return int_out;
}
int find_global(string global_name, Page& setclass){
    int value;    
    if(global_name == "TABS"){ value = setclass.get_tab_size(); return value;}
    if(global_name == "TAB"){ value = setclass.get_tab_number();return value;}
    if(global_name == "SETS"){ value = setclass.get_size(); return value;}
    if(global_name == "SET"){  value = setclass.get_iter(); return value;}
    
    
    if(global_name == "SETINT"){value =  setclass.get_integer(); return value;}       
    if(global_name == "SCINT") {value =  setclass.get_sc_integer(); return value;}        
    if(global_name == "SETBIN") {}
    if(global_name == "SCBIN") {}
    
    if(global_name == "INPUT"){value =  setclass.get_integer(); return value;}
    if(global_name == "SC") { value =  setclass.get_sc_integer(); return value;}
    
    //if(global_name == "FNAME"){  get_fname(); return value;}
    //if(global_name == "ICV") { get_icv(); return value;}

    if(global_name == "COMPLIMENT"){value = setclass.get_compliment(); return value;}
    if(global_name == "MPARTNER"){value = setclass.get_mpartner(); return value;}
    if(global_name == "ZPARTNER") {value = setclass.get_zpartner(); return value;}
    /*
    if(global_name == "INPUT")  list_string(get_integer()) << '\n'; return value;}
    if(global_name == "SC") {  list_string(get_sc_integer(), 2) << '\n'; return value;}
    if(global_name == "FNAME"){  get_fname() << '\n'; return value;}
    if(global_name == "ICV") { list_string(get_icv(),3) << '\n'; return value;}

    if(global_name == "COMPLIMENT"){list_string(get_compliment(),2) <<'\n'; return value;}
    if(global_name == "MPARTNER"){list_string(get_mpartner(),2) << '\n'; return value;}
    if(global_name == "ZPARTNER") {list_string(get_zpartner(),2); return value;}
    */     
    return 0;
}
