#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void command_handler(Program_settings& settings, String_list& input, Page& setclass);
bool input_commands(Program_settings& settings, String_list& input, Page& setclass);
bool output_commands(Program_settings& settings, String_list& input, Page& setclass);

bool script_handler(Program_settings& settings, String_list& input, Page& setclass);
    bool load_script_file(string filename, String_list& buffer, String_list& var_list);
    int string_to_int(string str_in);
#endif
