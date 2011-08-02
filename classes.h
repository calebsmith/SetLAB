#include <new>
#include <string>

class Loops{
    private:
        int* begin_line;
        int* end_line;
        int* iter;
        int* limit;
        int loops;
        
        void del(){
            delete [] begin_line;
            delete [] end_line;
            delete [] iter;
            delete [] limit;
        }
         void make(int num){
            del();
            loops = num;
            begin_line = new int[loops];
            end_line = new int[loops];
            iter = new int[loops];
            limit = new int[loops];
        }
        void assign(int num, const Loops& a){
            int i;
            for(i = 0; i < num; i++){
                begin_line[i] = a.begin_line[i];
                end_line[i] = a.end_line[i];
                iter[i] = a.iter[i];
                limit[i] = a.limit[i];
            }
        }
        void init(int i){            
            begin_line[i] = 0;
            end_line[i] = 0;
            iter[i] = 0;
            limit[i] = 0;
        }
    public:
        Loops(){
            loops = 0;
            begin_line = new int[1];
            end_line = new int[1];
            iter = new int[1];
            limit = new int[1];
        }
        ~Loops(){
            del();            
        }        
        Loops& operator= (const Loops &a){
			if(this == &a) return *this;	
			make(loops);	
			int i;
			for (i = 0; i < loops; i++){
			    begin_line[i] = a.begin_line[i];
			    end_line[i] = a.end_line[i];
			    iter[i] = a.iter[i];
			    limit[i] = a.limit[i];
			}			
			assign(loops, a);
			return *this;
		}
        void add_loop(){           
            loops++;            
            make(loops);
            int i;
            for (i = 0; i < loops; i++){
			    begin_line[i] = a.begin_line[i];
			    end_line[i] = a.end_line[i];
			    iter[i] = a.iter[i];
			    limit[i] = a.limit[i];
			}	
            init(loops); 
            return;      
        }   
        
        bool check_val(int val){if (val < 0 || val >= loops) return 1; return 0;}
        
        bool set_begin(int i, int val){if (check_val(i)) return 0; begin_line[i] = val; return 1;}
        bool set_end(int i, int val){if (check_val(i)) return 0; end_line[i] = val; return 1;}
        bool set_iter(int i, int val){if (check_val(i)) return 0; iter[i] = val; return 1;}
        bool set_limit(int i, int val){if (check_val(i)) return 0; limit[i] = val; return 1;}
        bool set_loops(int val){loops = val; return 1;}
        
        bool add_iter(int i){if (check_val(i)) return 0; iter[i]++; return 1;}
        
        
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

