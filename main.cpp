#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "setclass.h"
#include "slist.h"
#include "settings.h"

#include "constants.h"
#include "variables.h"
#include "functions.h"

using namespace std;


int main(int argc, char* argv[]){
    cout << '\n' << "SETLAB - Console" << '\n';
    
    if (argc == 2) setclass.load(argv[1]);
    while(1){
        if (settings.get_prompt() == 1) cout << "(SETLAB)>> ";
        
        getline(cin, rawinput);
        if (rawinput == "quit" || rawinput == "exit") return 0;
        input.parse(rawinput);
        
        command_handler(settings, input, setclass);     
    }
}
