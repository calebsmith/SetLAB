#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void command_handler(Program_settings& settings, String_list& input, Page& setclass);
    bool input_commands(Program_settings& settings, String_list& input, Page& setclass);
    bool output_commands(Program_settings& settings, String_list& input, Page& setclass);
        string capitalize(string word);    
        int find_global(string, Page& setclass);

    bool script_handler(Program_settings& settings, String_list& input, Page& setclass);
        bool load_script_file(string filename, String_list& buffer, String_list& var_list);
        void replace_vars(String_list& globals, String_list& proc_line, Page& setclass, Variables& var);
        int string_to_int(string str_in);
        string int_to_string(int int_in);
#endif
