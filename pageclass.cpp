#include <fstream>
#include <sstream>
#include <string>

#include "setclass.h"
#include "slist.h"

#include "constants.h"

void Page::save(){save("default"); return;};
void Page::save(string filename){
	int i, i2, i3;
	//convert filename to char* for opening the file
	char name [100] = "";
	stringstream input;
	input << filename;
	strcat (name, input.str().c_str());

	fstream fout;
	fout.open(name, ios_base::out);
		if (fout.fail()){cout << "Failed to open file for saving. Aborting..." << '\n'; return;}
		fout << "2010" << '\n';			//magic number
		fout << "4" << '\n';			//big version number
		fout << "0" << '\n';			//small version number
		fout << tab_size << '\n';		//how many tabs
		
		//group settings
		for (i = 0; i < tab_size; i++)
		{
			fout << tab[i].get_modulus() << '\n';
			fout << tab[i].get_canon() << '\n';
			fout << tab[i].get_default_m() << '\n';
			fout << tab[i].get_ordered() << '\n';
			fout << tab[i].get_multiset() << '\n';
			fout << tab[i].get_base() << '\n';
		}

		for (i = 0; i < tab_size; i++)
		{
			fout << tab[i].get_size() << '\n';					//how many sets in each tab
			for (i2 = 0; i2 < tab[i].get_size(); i2++)
			{
				fout << tab[i].current[i2].get_length() << '\n';//length of each set
				for (i3 = 0; i3 < tab[i].current[i2].get_length(); i3++)
				{
					fout << tab[i].current[i2].get_p(i3) << '\n';				//list of pitches in the set
				}
			}
		}
		
	fout.close();
	cout << "Save successful." << '\n';
	return;
};
void Page::load(){load("default"); return;};
void Page::load(string filename){
	
	int i, i2, i3;
	//convert filename to char* for opening the file
	char name [100] = "";
	stringstream input;
	input << filename;
	strcat (name, input.str().c_str());
	
    //variables for loading
    int magic, bigversion, smallversion;
    int value;
    bool bvalue;

	fstream fin;
	fin.open(name, ios_base::in);
		if (fin.fail()) {cout << "Failed to load file: " << filename << '\n'; return;}
		//load file headers and do some preliminary checking
		fin >> magic;			if (magic != 2010) {cout << "Wrong file type." << '\n'; return;}

		fin >> bigversion;		
		fin >> smallversion;			
		if (bigversion != 4 || smallversion != 0) {cout << "Not a compatible version." << '\n'; return;}
		//load the tabs
		fin >> value;  		if (value < 1) value = 1;//be sure we at least have one
		tab_size = value;
		if (DEBUG == 1) cout << "Number of tabs: " << value << '\n';
		make(tab_size);//create each tab (group of sets)
		//load the settings for each group
		for (i = 0; i < tab_size; i++)
		{
			
			fin >> value;	tab[i].mod(value);
			if (DEBUG == 1) cout << "Mod: " << value << '\n';
			fin >> value;	tab[i].canon(value);
			if (DEBUG == 1) cout << "Canon: " << value << '\n';
			fin >> value;	tab[i].m_value(value);
			if (DEBUG == 1) cout << "M value: " << value << '\n';
			fin >> bvalue;	tab[i].order(bvalue);
			if (DEBUG == 1) cout << "Ordered?: " << bvalue << '\n';
			fin >> bvalue;	tab[i].multi(bvalue);
			if (DEBUG == 1) cout << "Multiset?: " << bvalue << '\n';
			fin >> value;	tab[i].base_value(value);
			if (DEBUG == 1) cout << "Base: " << value << '\n';
		}
		for (i = 0; i < tab_size; i++)
		{
			fin >> value; //how many sets in group
			if (DEBUG == 1) cout << "Making " << value << " sets in group " << i << '\n';
			tab[i].make(value);
			if (DEBUG == 1) cout << "Group make successful" << '\n';
			for (i2 = 0; i2 < tab[i].get_size(); i2++)
			{
				fin >> value; // how many pitches in this set?
				if (DEBUG == 1) cout << "Making " << value << " pitches in set number " << i2 ;
				if (DEBUG == 1) cout << " in group " << i << '\n';
				tab[i].current[i2].make(value);
				if (DEBUG == 1) cout << "Set make successful" << '\n';
				for (i3 = 0; i3 < tab[i].current[i2].get_length(); i3++)
				{
					fin >> value;
					if (DEBUG == 1) cout << "Assigning " << value << " to position " << i3 ;
					if (DEBUG == 1) cout << " of set " << i2 << " in group " << i << '\n';
					tab[i].current[i2].assign(i3,value);
					
				}
				if (DEBUG == 1) cout <<"Assignment succesful. Refreshing..." << '\n';
				tab[i].current[i2].refresh();
			}
			if (DEBUG == 1) cout << "All set data is loaded for this set." << "Refreshing group lists." << '\n';
			tab[i].refresh_list();
		}
		
	fin.close();
	cout << "File loaded successfully." << '\n';
	return;
};
void Page::show(int var){
    int i;
    switch(var){
        
        case TABS: { cout << "Total tabs:        " << get_tab_size() << '\n'; break;}
        case TAB:  { cout << "Tab #:             " << get_tab_number() << '\n'; break;}
        case SETS: { cout << "Total sets in tab: " << get_size() << '\n'; break;}
        case SET:  { cout << "Current set #:     " << get_iter() << '\n'; break;}
        
        case PITCHES: {
            cout << get_length() << " pitches: ";
            for (i = 0; i < get_length(); i++){ 
                cout << get_p(i) << " ";
            }
            cout << '\n';
            break;
        }
        case PCS: {
            cout << get_card() << " PC's: ";
            for (i = 0; i < get_card(); i++){ cout << get_pc(i) << " ";}
            cout << '\n';
            break;
        }
        
        case SETINT: break;       
        case SCINT: break;        
        case SETBIN: break;
        case SCBIN: break;

        case INPUT: {cout << "Input:      " << list_string(get_integer()) << '\n'; break;}
        case SC: { cout << "SC:         " << list_string(get_sc_integer(), 2) << '\n'; break;}
        case FNAME:{ cout << "Forte name: " << get_fname() << '\n'; break;}
        case ICV: {cout << "ICV:        " << list_string(get_icv(),3) << '\n'; break;}

        case COMPLIMENT: {cout << "Compliment: " << list_string(get_compliment(),2) <<'\n'; break;}
        case MPARTNER:{cout << "M-partner:  " << list_string(get_mpartner(),2) << '\n'; break;}
        case ZPARTNER: {
            if (get_modulus() <= 12 || get_zpartner() > -1) {
                cout << "Z-partner:  " << list_string(get_zpartner(),2) << '\n';
            }
            else{
                cout << "Z-partner: " << " disabled for moduli > 12. Use forcez (can be very slow)"<< '\n';   
            }
            break;
        }
    }
};

void Page::stat(){
    cout << '\n';
    show(TABS);
    show(TAB);
    show(SETS);
    show(SET);
    
};
void Page::list(){

    cout << '\n';
    show(PITCHES);
    show(PCS);
};
void Page::set(){

    cout << '\n';
    show(INPUT);
    show(SC);
    show(FNAME);
    show(ICV);
    show(COMPLIMENT);
    show(MPARTNER);
    show(ZPARTNER);    
}; 

