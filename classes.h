#include <new>
#include <string>

class Loops{
    private:
        int* begin_line;
        int* end_line;
        int* iter;
        int* limit;
        int loops;
    public:
        Loops(){
            loops = 0;
            begin_line = new int[1];
            end_line = new int[1];
            iter = new int[1];
            limit = new int[1];
        }
        ~Loops(){
            delete [] begin_line;
            delete [] end_line;
            delete [] iter;
            delete [] limit;
        }
        
        bool check_val(int val){if (val < 0 || val >= loops) return 1; return 0;}
        
        bool set_begin_line(int i, int val){if (check_val(i)) return 0; begin_line[i] = val; return 1;}
        bool set_end_line(int i, int val){if (check_val(i)) return 0; end_line[i] = val; return 1;}
        bool set_iter(int i, int val){if (check_val(i)) return 0; iter[i] = val; return 1;}
        bool set_limit(int i, int val){if (check_val(i)) return 0; limit[i] = val; return 1;}
        bool set_loops(int val){loops = val; return 1;}
        
        bool add_iter(int i){if (check_val(i)) return 0; iter[i]++; return 1;}
        void add_loops(){loops++;}
        
        int get_loops(){int a = loops; return a;}
        int get_begin(int i){if (check_val(i)) return 0; int a = begin_line[i]; return a;}
        int get_end(int i){if (check_val(i)) return 0; int a = end_line[i]; return a;}
        int get_iter(int i){if (check_val(i)) return 0; int a = iter[i]; return a;}
        int get_limit(int i){if (check_val(i)) return 0; int a = limit[i]; return a;}
};

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

