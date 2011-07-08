#include <new>
#include <string>

class Variables{
    private:
       int* value;
       string* name;
       int vars;
        
    public:
        Variables(){            
            vars = 0;
            value = new int[1];
            name = new string[1];
        }
        ~Variables(){        
            delete [] value;
            delete [] name;
        }       
        void make(String_list& var_list){
            //assign variable names obtained from var_list            
            delete [] name;
            delete [] value;
            
            vars = var_list.length();            
            name = new string[vars];
            value = new int[vars];
            
            int i;
            for(i = 0; i < vars; i++){
                var_list >> name[i]; 
                value[i] = 0;
            }            
        }    
            
        void assign(string label, int val){
            int i;
            for(i = 0; i < vars; i++){
                if (label == name[i]){
                    set_value(i, val);    
                    break;
                }
            }
            return;
        }
        void set_value(int i, int val){
            if (i < 0 || i >= vars) return;
            value[i] = val;
            return;   
        }
        
        string get_name(int i){string a = ""; if (i >= 0 && i < vars) a = name[i]; return a;}
        int get_value(int i){int a = 0; if (i >= 0 && i < vars) a = value[i]; return a;}
        int length(){int a = vars; return a;}
        
        void list(){
            int i;
            for(i = 0; i < vars; i++){
                cout << "Variable name: " << name[i] << " has value " << value[i] << '\n';
            }
        }
};

