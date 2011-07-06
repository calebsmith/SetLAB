#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "setclass.h"
#include "slist.h"
#include "settings.h"

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
    
    int i, i2;
    int vars;
    vars = var_list.length();
    
    //assign memory for variables
    int* var_value;
    string* var_name;
    var_value = new int[vars + 1];
    var_name = new string[vars + 1];
    
    //assign variable names obtained from var_list
    for(i = 0; i < vars; i++){
        var_list >> var_name[i]; 
    }
    //run the script
    int file_length = buffer.length();
    string line;
    String_list proc_line;
    string name_in;
    int value_in;
    for(i = 0; i < file_length; i++){
        line = buffer.get(i);       //obtain the next line
        if (line == "end") break;   //exit if script command is "end"         
        proc_line.parse(line);      //parse the current line
        
        //show line if echo on
        if (settings.get_echo() == 1) cout << line << '\n'; 
        
        //assign variables
        if (proc_line.get(1) == "="){
            name_in = proc_line.get(0);
            value_in = string_to_int(proc_line.get(2));
            for(i2 = 0; i2 < vars; i2++){
                if (name_in == var_name[i2]){
                    var_value[i2] = value_in;
                    cout << var_name[i2] << " is now = to " << var_value[i2] << '\n';
                    break;
                }
            }
            
            continue;
        }
        
        //reaching this point means the line contains a regular command.
        //execute that command    
        command_handler(settings, proc_line, setclass);
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
int string_to_int(string str_in){
    stringstream val;
    int int_out;       
    
    val << str_in;
    val >> int_out;    
    return int_out;
}
