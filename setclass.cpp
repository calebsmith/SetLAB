#include <math.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "setclass.h"
#include "slist.h"

int Set::Modulus = 12;
int Set::Canon = 1;
int Set::Default_M = 5;
int Set::Ordered = true;
int Set::Multi = false;
int Set::Base = 2;

//find the decimal value of the bit array sent to the function
long long int Set::to_decimal (int set[], int size)
{
    int i;
    long long int i2;
    int tc;
    long long int answer = 0;
    
    for (i = 0; i < Modulus; i++)
    {
        if (i < size) 
        {
            tc = set[i];
            i2 = pow(Base, i);
            if (tc > 0) answer = answer + (tc * i2);
        }
    }
    
    return answer;
}

void Set::to_binary (long long int set_decimal)
{
    long long int temp = set_decimal;
    int i;
    int i2;
	delete [] temp_array;
	temp_array = new int[Modulus];
   
    for (i = 0; i < Modulus; i++) {temp_array[i] = 0;}
    for (i = Modulus - 1; i >= 0; i--)
    {
        i2 = pow(Base, i);
        if (temp >= i2)
        {
            temp -= i2;
            temp_array[i] = 1;
        }
       
	}
    return;
}

long long int Set::prime(long long int intin)
{
   // if (Base != 2) return 0;
    long long int answer;
    int last_rot;
    all_tto(intin);
    switch(Canon)
    {
        case 0: last_rot = Modulus; break;
        case 1: last_rot = Modulus * 2; break;
        case 2: last_rot = Modulus * 3; break;
        case 3: last_rot = Modulus * 4; break;
    }
	
    answer = lowest(tto.list,last_rot);
    return answer;
};

void Set::all_tto(long long int intin)
{

    int rot_length = Modulus * (Canon + 1);
    delete [] tto.list;
    tto.list = new long long int[rot_length];
    long long int prime = intin;
    long long int inv = 0;
	long long int m = 0;
	long long int mi = 0;
	if (Canon > 0) inv = inversion_int(intin);
    if (Canon > 1) m = m_int(intin);
	if (Canon > 2) mi = inversion_int(m_int(intin));
   
    long long int current;
    
    int i,i2;
    int operation;
    for (operation = 0; operation < Canon + 1; operation++)
    {
        switch(operation)
        {
            case 0: current = prime; break;
            case 1: current = inv;   break;
            case 2: current = m;     break;
            case 3: current = mi;    break;
        }
	
        for (i = 0; i < Modulus; i++)
        {
            i2 = (operation * Modulus) + i;
            
            tto.list[i2] = transpose_int(current,i);
        }
    }
    tto.size = rot_length;
    return;
};
long long int Set::lowest(long long int* rot_set, int size)
{
    long long int answer;
    long long int low_num = pow(Base, Modulus) - 1;//initialize as the biggest possible number in this system
    int i;
    for (i = 0; i < size; i++)
    {
        if (low_num > rot_set[i] && Canon != 2) low_num = rot_set[i];
        if (low_num > rot_set[i] && Canon == 2)//if using TnM, skip TnI rotations
        {
            if (i < Modulus || i >= Modulus * 2)
            {
                low_num = rot_set[i];
            }
        }
    }
    answer = low_num;
    return answer;
};

void Set::findicv(long long int intin){
	int icv_length = (Modulus / 2) + 1;
	
	int* set;
	set = new int[Modulus];
	int i, i2;
	to_binary(intin);
	for (i = 0; i < Modulus; i++){set[i] = temp_array[i];}
	for (i = 0; i < icv_length; i++){temp_icv[i] = 0;}
	int difference;
	for(i = 0; i < Modulus; i++)
	{
		if (set[i] >= 1)
		{
			for(i2 = i; i2 < Modulus; i2++)
			{
				if (set[i2] >= 1)
				{
					difference = i2 - i;
					if (difference > icv_length - 1) difference = Modulus - difference;
					temp_icv[difference]++;
				}
			}

		}
	}
	delete [] set;	
	
	return;
};

long long int Set::zpartner(long long int intin)
{
	if (intin == 0) return -1;
	long long int answer = -1;			//default to answering none in case no zpartner is found.
	int i, i2;
	long long int scin = prime(intin);  //for the sc of the set that came into the function.
	long long int thissc;				//for the sc of each set

	int incard = findcardinality(intin);//cardinality for set that came in
	int thiscard;						//cardinality for each set

	long long int begin = pow(Base, incard) - 1;//begin with the first set of the cardinality of the incoming set

	int icv_length = (Modulus / 2) + 1;

	int* icvin;							//icv of sc that came into the function
	icvin = new int [icv_length];
	findicv(scin);
	for (i = 0; i < icv_length; i++) icvin[i] = temp_icv[i];

	int* icvcheck;						//icv of each sc checked
	icvcheck = new int [icv_length];
	
	int sum;
	long long int max_i = pow(Base, Modulus) / 2;	//maximum set number of the modulus (divide by 2 because last half 
											//are redundant except the universal set, which never has a z-partner)
	for (i = begin; i < max_i; i++)
	{
		sum = 0; i2 = 0;
		thiscard = findcardinality(i);
		if (thiscard == incard)//only sets of the same cardinality can have the same ICV
		{
			findicv(i);
			while(sum >= 0 && i2 < icv_length)
			{				
				if (temp_icv[i2] != icvin[i2]) sum = -1;//exit if an entry is different
				if (temp_icv[i2] == icvin[i2]) sum++;
				i2++;
			}
	
			if (sum == icv_length)//if all icv entries are the same
			{			
				thissc = prime(i);
				if (thissc != scin) //check to be sure they are not the same SC
				{
					answer = thissc;
					delete [] icvin; delete [] icvcheck;

					return answer;
				}
			}
		}
	
	}

	delete [] icvin; delete [] icvcheck;
	return answer; //return with no partner found.
};


long long int Set::fname_to_scint(string fnamein)
{
    long long int answer = -1; //default to failure report (0 is reserved for empty set)
    if (fnamein == "{}"){  answer = 0; return answer;}
    if (fnamein == "1-1"){  answer = 1; return answer;}
    if (fnamein == "2-1"){  answer = 3; return answer;}
    if (fnamein == "2-2"){  answer = 5; return answer;}
    if (fnamein == "3-1"){  answer = 7; return answer;}
    if (fnamein == "2-3"){  answer = 9; return answer;}
    if (fnamein == "3-2"){  answer = 11; return answer;}
    if (fnamein == "4-1"){  answer = 15; return answer;}
    if (fnamein == "2-4"){  answer = 17; return answer;}
    if (fnamein == "3-3"){  answer = 19; return answer;}
    if (fnamein == "3-6"){  answer = 21; return answer;}
    if (fnamein == "4-2"){  answer = 23; return answer;}
    if (fnamein == "4-3"){  answer = 27; return answer;}
    if (fnamein == "5-1"){  answer = 31; return answer;}
    if (fnamein == "2-5"){  answer = 33; return answer;}
    if (fnamein == "3-4"){  answer = 35; return answer;}
    if (fnamein == "3-7"){  answer = 37; return answer;}
    if (fnamein == "4-4"){  answer = 39; return answer;}
    if (fnamein == "4-11"){  answer = 43; return answer;}
    if (fnamein == "4-10"){  answer = 45; return answer;}
    if (fnamein == "5-2"){  answer = 47; return answer;}
    if (fnamein == "4-7"){  answer = 51; return answer;}
    if (fnamein == "5-3"){  answer = 55; return answer;}
    if (fnamein == "6-1"){  answer = 63; return answer;}
    if (fnamein == "2-6"){  answer = 65; return answer;}
    if (fnamein == "3-5"){  answer = 67; return answer;}
    if (fnamein == "3-8"){  answer = 69; return answer;}
    if (fnamein == "4-5"){  answer = 71; return answer;}
    if (fnamein == "3-10"){  answer = 73; return answer;}
    if (fnamein == "4-13"){  answer = 75; return answer;}
    if (fnamein == "4-12"){  answer = 77; return answer;}
    if (fnamein == "5-4"){  answer = 79; return answer;}
    if (fnamein == "4-Z15"){  answer = 83; return answer;}
    if (fnamein == "4-21"){  answer = 85; return answer;}
    if (fnamein == "5-9"){  answer = 87; return answer;}
    if (fnamein == "5-10"){  answer = 91; return answer;}
    if (fnamein == "5-8"){  answer = 93; return answer;}
    if (fnamein == "6-2"){  answer = 95; return answer;}
    if (fnamein == "4-8"){  answer = 99; return answer;}
    if (fnamein == "5-6"){  answer = 103; return answer;}
    if (fnamein == "5-Z12"){  answer = 107; return answer;}
    if (fnamein == "6-Z3"){  answer = 111; return answer;}
    if (fnamein == "6-Z4"){  answer = 119; return answer;}
    if (fnamein == "7-1"){  answer = 127; return answer;}
    if (fnamein == "3-9"){  answer = 133; return answer;}
    if (fnamein == "4-6"){  answer = 135; return answer;}
    if (fnamein == "3-11"){  answer = 137; return answer;}
    if (fnamein == "4-Z29"){  answer = 139; return answer;}
    if (fnamein == "4-14"){  answer = 141; return answer;}
    if (fnamein == "5-5"){  answer = 143; return answer;}
    if (fnamein == "4-18"){  answer = 147; return answer;}
    if (fnamein == "4-22"){  answer = 149; return answer;}
    if (fnamein == "5-Z36"){  answer = 151; return answer;}
    if (fnamein == "4-17"){  answer = 153; return answer;}
    if (fnamein == "5-16"){  answer = 155; return answer;}
    if (fnamein == "5-11"){  answer = 157; return answer;}
    if (fnamein == "6-Z36"){  answer = 159; return answer;}
    if (fnamein == "4-16"){  answer = 163; return answer;}
    if (fnamein == "4-23"){  answer = 165; return answer;}
    if (fnamein == "5-14"){  answer = 167; return answer;}
    if (fnamein == "5-24"){  answer = 171; return answer;}
    if (fnamein == "5-23"){  answer = 173; return answer;}
    if (fnamein == "6-9"){  answer = 175; return answer;}
    if (fnamein == "5-Z18"){  answer = 179; return answer;}
    if (fnamein == "6-Z11"){  answer = 183; return answer;}
    if (fnamein == "6-Z10"){  answer = 187; return answer;}
    if (fnamein == "6-8"){  answer = 189; return answer;}
    if (fnamein == "7-2"){  answer = 191; return answer;}
    if (fnamein == "4-9"){  answer = 195; return answer;}
    if (fnamein == "5-7"){  answer = 199; return answer;}
    if (fnamein == "5-19"){  answer = 203; return answer;}
    if (fnamein == "6-5"){  answer = 207; return answer;}
    if (fnamein == "6-Z12"){  answer = 215; return answer;}
    if (fnamein == "6-Z13"){  answer = 219; return answer;}
    if (fnamein == "7-4"){  answer = 223; return answer;}
    if (fnamein == "6-Z6"){  answer = 231; return answer;}
    if (fnamein == "7-5"){  answer = 239; return answer;}
    if (fnamein == "8-1"){  answer = 255; return answer;}
    if (fnamein == "3-12"){  answer = 273; return answer;}
    if (fnamein == "4-19"){  answer = 275; return answer;}
    if (fnamein == "4-24"){  answer = 277; return answer;}
    if (fnamein == "5-13"){  answer = 279; return answer;}
    if (fnamein == "5-Z17"){  answer = 283; return answer;}
    if (fnamein == "6-Z37"){  answer = 287; return answer;}
    if (fnamein == "4-20"){  answer = 291; return answer;}
    if (fnamein == "4-27"){  answer = 293; return answer;}
    if (fnamein == "5-Z38"){  answer = 295; return answer;}
    if (fnamein == "4-26"){  answer = 297; return answer;}
    if (fnamein == "5-27"){  answer = 299; return answer;}
    if (fnamein == "5-25"){  answer = 301; return answer;}
    if (fnamein == "6-Z40"){  answer = 303; return answer;}
    if (fnamein == "5-21"){  answer = 307; return answer;}
    if (fnamein == "5-26"){  answer = 309; return answer;}
    if (fnamein == "6-15"){  answer = 311; return answer;}
    if (fnamein == "5-Z37"){  answer = 313; return answer;}
    if (fnamein == "6-14"){  answer = 315; return answer;}
    if (fnamein == "6-Z39"){  answer = 317; return answer;}
    if (fnamein == "7-3"){  answer = 319; return answer;}
    if (fnamein == "4-25"){  answer = 325; return answer;}
    if (fnamein == "5-15"){  answer = 327; return answer;}
    if (fnamein == "5-29"){  answer = 331; return answer;}
    if (fnamein == "5-28"){  answer = 333; return answer;}
    if (fnamein == "6-Z41"){  answer = 335; return answer;}
    if (fnamein == "5-30"){  answer = 339; return answer;}
    if (fnamein == "5-33"){  answer = 341; return answer;}
    if (fnamein == "6-22"){  answer = 343; return answer;}
    if (fnamein == "6-Z24"){  answer = 347; return answer;}
    if (fnamein == "6-21"){  answer = 349; return answer;}
    if (fnamein == "7-9"){  answer = 351; return answer;}
    if (fnamein == "5-20"){  answer = 355; return answer;}
    if (fnamein == "6-Z43"){  answer = 359; return answer;}
    if (fnamein == "6-Z25"){  answer = 363; return answer;}
    if (fnamein == "6-Z23"){  answer = 365; return answer;}
    if (fnamein == "7-Z36"){  answer = 367; return answer;}
    if (fnamein == "6-16"){  answer = 371; return answer;}
    if (fnamein == "7-13"){  answer = 375; return answer;}
    if (fnamein == "7-11"){  answer = 379; return answer;}
    if (fnamein == "7-8"){  answer = 381; return answer;}
    if (fnamein == "8-2"){  answer = 383; return answer;}
    if (fnamein == "6-Z38"){  answer = 399; return answer;}
    if (fnamein == "5-22"){  answer = 403; return answer;}
    if (fnamein == "6-Z17"){  answer = 407; return answer;}
    if (fnamein == "6-Z19"){  answer = 411; return answer;}
    if (fnamein == "7-6"){  answer = 415; return answer;}
    if (fnamein == "6-18"){  answer = 423; return answer;}
    if (fnamein == "6-Z26"){  answer = 427; return answer;}
    if (fnamein == "7-14"){  answer = 431; return answer;}
    if (fnamein == "7-Z38"){  answer = 439; return answer;}
    if (fnamein == "7-Z37"){  answer = 443; return answer;}
    if (fnamein == "8-4"){  answer = 447; return answer;}
    if (fnamein == "6-7"){  answer = 455; return answer;}
    if (fnamein == "7-7"){  answer = 463; return answer;}
    if (fnamein == "7-15"){  answer = 471; return answer;}
    if (fnamein == "8-5"){  answer = 479; return answer;}
    if (fnamein == "8-6"){  answer = 495; return answer;}
    if (fnamein == "9-1"){  answer = 511; return answer;}
    if (fnamein == "4-28"){  answer = 585; return answer;}
    if (fnamein == "5-31"){  answer = 587; return answer;}
    if (fnamein == "6-Z42"){  answer = 591; return answer;}
    if (fnamein == "5-32"){  answer = 595; return answer;}
    if (fnamein == "5-34"){  answer = 597; return answer;}
    if (fnamein == "6-Z46"){  answer = 599; return answer;}
    if (fnamein == "6-27"){  answer = 603; return answer;}
    if (fnamein == "6-Z45"){  answer = 605; return answer;}
    if (fnamein == "7-10"){  answer = 607; return answer;}
    if (fnamein == "6-Z44"){  answer = 615; return answer;}
    if (fnamein == "6-Z28"){  answer = 619; return answer;}
    if (fnamein == "7-16"){  answer = 623; return answer;}
    if (fnamein == "7-Z17"){  answer = 631; return answer;}
    if (fnamein == "8-3"){  answer = 639; return answer;}
    if (fnamein == "5-35"){  answer = 661; return answer;}
    if (fnamein == "6-Z47"){  answer = 663; return answer;}
    if (fnamein == "6-Z49"){  answer = 667; return answer;}
    if (fnamein == "7-Z12"){  answer = 671; return answer;}
    if (fnamein == "6-Z48"){  answer = 679; return answer;}
    if (fnamein == "6-34"){  answer = 683; return answer;}
    if (fnamein == "6-33"){  answer = 685; return answer;}
    if (fnamein == "7-24"){  answer = 687; return answer;}
    if (fnamein == "6-31"){  answer = 691; return answer;}
    if (fnamein == "6-32"){  answer = 693; return answer;}
    if (fnamein == "7-27"){  answer = 695; return answer;}
    if (fnamein == "7-26"){  answer = 699; return answer;}
    if (fnamein == "7-23"){  answer = 701; return answer;}
    if (fnamein == "8-11"){  answer = 703; return answer;}
    if (fnamein == "6-30"){  answer = 715; return answer;}
    if (fnamein == "6-Z29"){  answer = 717; return answer;}
    if (fnamein == "7-19"){  answer = 719; return answer;}
    if (fnamein == "6-Z50"){  answer = 723; return answer;}
    if (fnamein == "7-29"){  answer = 727; return answer;}
    if (fnamein == "7-31"){  answer = 731; return answer;}
    if (fnamein == "7-25"){  answer = 733; return answer;}
    if (fnamein == "8-13"){  answer = 735; return answer;}
    if (fnamein == "7-20"){  answer = 743; return answer;}
    if (fnamein == "7-28"){  answer = 747; return answer;}
    if (fnamein == "8-Z29"){  answer = 751; return answer;}
    if (fnamein == "7-Z18"){  answer = 755; return answer;}
    if (fnamein == "8-14"){  answer = 759; return answer;}
    if (fnamein == "8-12"){  answer = 763; return answer;}
    if (fnamein == "8-10"){  answer = 765; return answer;}
    if (fnamein == "9-2"){  answer = 767; return answer;}
    if (fnamein == "6-20"){  answer = 819; return answer;}
    if (fnamein == "7-21"){  answer = 823; return answer;}
    if (fnamein == "8-7"){  answer = 831; return answer;}
    if (fnamein == "7-30"){  answer = 855; return answer;}
    if (fnamein == "7-32"){  answer = 859; return answer;}
    if (fnamein == "8-Z15"){  answer = 863; return answer;}
    if (fnamein == "7-22"){  answer = 871; return answer;}
    if (fnamein == "8-18"){  answer = 879; return answer;}
    if (fnamein == "8-19"){  answer = 887; return answer;}
    if (fnamein == "8-17"){  answer = 891; return answer;}
    if (fnamein == "9-3"){  answer = 895; return answer;}
    if (fnamein == "8-8"){  answer = 927; return answer;}
    if (fnamein == "8-16"){  answer = 943; return answer;}
    if (fnamein == "8-20"){  answer = 951; return answer;}
    if (fnamein == "9-4"){  answer = 959; return answer;}
    if (fnamein == "8-9"){  answer = 975; return answer;}
    if (fnamein == "9-5"){  answer = 991; return answer;}
    if (fnamein == "10-1"){  answer = 1023; return answer;}
    if (fnamein == "6-35"){  answer = 1365; return answer;}
    if (fnamein == "7-33"){  answer = 1367; return answer;}
    if (fnamein == "7-34"){  answer = 1371; return answer;}
    if (fnamein == "8-21"){  answer = 1375; return answer;}
    if (fnamein == "7-35"){  answer = 1387; return answer;}
    if (fnamein == "8-22"){  answer = 1391; return answer;}
    if (fnamein == "8-24"){  answer = 1399; return answer;}
    if (fnamein == "9-6"){  answer = 1407; return answer;}
    if (fnamein == "8-23"){  answer = 1455; return answer;}
    if (fnamein == "8-27"){  answer = 1463; return answer;}
    if (fnamein == "8-26"){  answer = 1467; return answer;}
    if (fnamein == "9-7"){  answer = 1471; return answer;}
    if (fnamein == "8-25"){  answer = 1495; return answer;}
    if (fnamein == "9-8"){  answer = 1503; return answer;}
    if (fnamein == "9-9"){ answer = 1519; return answer;}
    if (fnamein == "10-2"){ answer = 1535; return answer;}
    if (fnamein == "8-28"){ answer = 1755; return answer;}
    if (fnamein == "9-10"){ answer = 1759; return answer;}
    if (fnamein == "9-11"){ answer = 1775; return answer;}
    if (fnamein == "10-3"){ answer = 1791; return answer;}
    if (fnamein == "9-12"){ answer = 1911; return answer;}
    if (fnamein == "10-4"){ answer = 1919; return answer;}
    if (fnamein == "10-5"){ answer = 1983; return answer;}
    if (fnamein == "10-6"){ answer = 2015; return answer;}
    if (fnamein == "11-1"){ answer = 2047; return answer;}
    if (fnamein == "12-1"){ answer = 4095; return answer;}
    return answer;
}


string Set::fortename(long long int sc)
{
string name;
int sci;
sci = (int) sc;
if (Modulus != 12) {name = "Not for this Modulus or Set Class space"; return name;}
name = "none found";
	switch (sci)
	{
		case 0: name = "{}"; break;
		case 1: name = "1-1"; break;
		case 3: name = "2-1"; break;
		case 5: name = "2-2"; break;
		case 7: name = "3-1"; break;
		case 9: name = "2-3"; break;
		case 11: name = "3-2"; break;
		case 15: name = "4-1"; break;
		case 17: name = "2-4"; break;
		case 19: name = "3-3"; break;
		case 21: name = "3-6"; break;
		case 23: name = "4-2"; break;
		case 27: name = "4-3"; break;
		case 31: name = "5-1"; break;
		case 33: name = "2-5"; break;
		case 35: name = "3-4"; break;
		case 37: name = "3-7"; break;
		case 39: name = "4-4"; break;
		case 43: name = "4-11"; break;
		case 45: name = "4-10"; break;
		case 47: name = "5-2"; break;
		case 51: name = "4-7"; break;
		case 55: name = "5-3"; break;
		case 63: name = "6-1"; break;
		case 65: name = "2-6"; break;
		case 67: name = "3-5"; break;
		case 69: name = "3-8"; break;
		case 71: name = "4-5"; break;
		case 73: name = "3-10"; break;
		case 75: name = "4-13"; break;
		case 77: name = "4-12"; break;
		case 79: name = "5-4"; break;
		case 83: name = "4-Z15"; break;
		case 85: name = "4-21"; break;
		case 87: name = "5-9"; break;
		case 91: name = "5-10"; break;
		case 93: name = "5-8"; break;
		case 95: name = "6-2"; break;
		case 99: name = "4-8"; break;
		case 103: name = "5-6"; break;
		case 107: name = "5-Z12"; break;
		case 111: name = "6-Z3"; break;
		case 119: name = "6-Z4"; break;
		case 127: name = "7-1"; break;
		case 133: name = "3-9"; break;
		case 135: name = "4-6"; break;
		case 137: name = "3-11"; break;
		case 139: name = "4-Z29"; break;
		case 141: name = "4-14"; break;
		case 143: name = "5-5"; break;
		case 147: name = "4-18"; break;
		case 149: name = "4-22"; break;
		case 151: name = "5-Z36"; break;
		case 153: name = "4-17"; break;
		case 155: name = "5-16"; break;
		case 157: name = "5-11"; break;
		case 159: name = "6-Z36"; break;
		case 163: name = "4-16"; break;
		case 165: name = "4-23"; break;
		case 167: name = "5-14"; break;
		case 171: name = "5-24"; break;
		case 173: name = "5-23"; break;
		case 175: name = "6-9"; break;
		case 179: name = "5-Z18"; break;
		case 183: name = "6-Z11"; break;
		case 187: name = "6-Z10"; break;
		case 189: name = "6-8"; break;
		case 191: name = "7-2"; break;
		case 195: name = "4-9"; break;
		case 199: name = "5-7"; break;
		case 203: name = "5-19"; break;
		case 207: name = "6-5"; break;
		case 215: name = "6-Z12"; break;
		case 219: name = "6-Z13"; break;
		case 223: name = "7-4"; break;
		case 231: name = "6-Z6"; break;
		case 239: name = "7-5"; break;
		case 255: name = "8-1"; break;
		case 273: name = "3-12"; break;
		case 275: name = "4-19"; break;
		case 277: name = "4-24"; break;
		case 279: name = "5-13"; break;
		case 283: name = "5-Z17"; break;
		case 287: name = "6-Z37"; break;
		case 291: name = "4-20"; break;
		case 293: name = "4-27"; break;
		case 295: name = "5-Z38"; break;
		case 297: name = "4-26"; break;
		case 299: name = "5-27"; break;
		case 301: name = "5-25"; break;
		case 303: name = "6-Z40"; break;
		case 307: name = "5-21"; break;
		case 309: name = "5-26"; break;
		case 311: name = "6-15"; break;
		case 313: name = "5-Z37"; break;
		case 315: name = "6-14"; break;
		case 317: name = "6-Z39"; break;
		case 319: name = "7-3"; break;
		case 325: name = "4-25"; break;
		case 327: name = "5-15"; break;
		case 331: name = "5-29"; break;
		case 333: name = "5-28"; break;
		case 335: name = "6-Z41"; break;
		case 339: name = "5-30"; break;
		case 341: name = "5-33"; break;
		case 343: name = "6-22"; break;
		case 347: name = "6-Z24"; break;
		case 349: name = "6-21"; break;
		case 351: name = "7-9"; break;
		case 355: name = "5-20"; break;
		case 359: name = "6-Z43"; break;
		case 363: name = "6-Z25"; break;
		case 365: name = "6-Z23"; break;
		case 367: name = "7-Z36"; break;
		case 371: name = "6-16"; break;
		case 375: name = "7-13"; break;
		case 379: name = "7-11"; break;
		case 381: name = "7-8"; break;
		case 383: name = "8-2"; break;
		case 399: name = "6-Z38"; break;
		case 403: name = "5-22"; break;
		case 407: name = "6-Z17"; break;
		case 411: name = "6-Z19"; break;
		case 415: name = "7-6"; break;
		case 423: name = "6-18"; break;
		case 427: name = "6-Z26"; break;
		case 431: name = "7-14"; break;
		case 439: name = "7-Z38"; break;
		case 443: name = "7-Z37"; break;
		case 447: name = "8-4"; break;
		case 455: name = "6-7"; break;
		case 463: name = "7-7"; break;
		case 471: name = "7-15"; break;
		case 479: name = "8-5"; break;
		case 495: name = "8-6"; break;
		case 511: name = "9-1"; break;
		case 585: name = "4-28"; break;
		case 587: name = "5-31"; break;
		case 591: name = "6-Z42"; break;
		case 595: name = "5-32"; break;
		case 597: name = "5-34"; break;
		case 599: name = "6-Z46"; break;
		case 603: name = "6-27"; break;
		case 605: name = "6-Z45"; break;
		case 607: name = "7-10"; break;
		case 615: name = "6-Z44"; break;
		case 619: name = "6-Z28"; break;
		case 623: name = "7-16"; break;
		case 631: name = "7-Z17"; break;
		case 639: name = "8-3"; break;
		case 661: name = "5-35"; break;
		case 663: name = "6-Z47"; break;
		case 667: name = "6-Z49"; break;
		case 671: name = "7-Z12"; break;
		case 679: name = "6-Z48"; break;
		case 683: name = "6-34"; break;
		case 685: name = "6-33"; break;
		case 687: name = "7-24"; break;
		case 691: name = "6-31"; break;
		case 693: name = "6-32"; break;
		case 695: name = "7-27"; break;
		case 699: name = "7-26"; break;
		case 701: name = "7-23"; break;
		case 703: name = "8-11"; break;
		case 715: name = "6-30"; break;
		case 717: name = "6-Z29"; break;
		case 719: name = "7-19"; break;
		case 723: name = "6-Z50"; break;
		case 727: name = "7-29"; break;
		case 731: name = "7-31"; break;
		case 733: name = "7-25"; break;
		case 735: name = "8-13"; break;
		case 743: name = "7-20"; break;
		case 747: name = "7-28"; break;
		case 751: name = "8-Z29"; break;
		case 755: name = "7-Z18"; break;
		case 759: name = "8-14"; break;
		case 763: name = "8-12"; break;
		case 765: name = "8-10"; break;
		case 767: name = "9-2"; break;
		case 819: name = "6-20"; break;
		case 823: name = "7-21"; break;
		case 831: name = "8-7"; break;
		case 855: name = "7-30"; break;
		case 859: name = "7-32"; break;
		case 863: name = "8-Z15"; break;
		case 871: name = "7-22"; break;
		case 879: name = "8-18"; break;
		case 887: name = "8-19"; break;
		case 891: name = "8-17"; break;
		case 895: name = "9-3"; break;
		case 927: name = "8-8"; break;
		case 943: name = "8-16"; break;
		case 951: name = "8-20"; break;
		case 959: name = "9-4"; break;
		case 975: name = "8-9"; break;
		case 991: name = "9-5"; break;
		case 1023: name = "10-1"; break;
		case 1365: name = "6-35"; break;
		case 1367: name = "7-33"; break;
		case 1371: name = "7-34"; break;
		case 1375: name = "8-21"; break;
		case 1387: name = "7-35"; break;
		case 1391: name = "8-22"; break;
		case 1399: name = "8-24"; break;
		case 1407: name = "9-6"; break;
		case 1455: name = "8-23"; break;
		case 1463: name = "8-27"; break;
		case 1467: name = "8-26"; break;
		case 1471: name = "9-7"; break;
		case 1495: name = "8-25"; break;
		case 1503: name = "9-8"; break;
		case 1519: name = "9-9"; break;
		case 1535: name = "10-2"; break;
		case 1755: name = "8-28"; break;
		case 1759: name = "9-10"; break;
		case 1775: name = "9-11"; break;
		case 1791: name = "10-3"; break;
		case 1911: name = "9-12"; break;
		case 1919: name = "10-4"; break;
		case 1983: name = "10-5"; break;
		case 2015: name = "10-6"; break;
		case 2047: name = "11-1"; break;
		case 4095: name = "12-1"; break;
	}
return name;
};


