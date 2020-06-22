#ifndef __Globals__
#define __Globals__

#include <string>
#include <vector>
#define Cyan_ "\033[1;36m"
#define _Cyan "\033[0m"   
#define Red_  "\033[0;31m"          
#define _Red  "\033[0m"          
#define Green_  "\033[0;32m"        
#define _Green  "\033[0m"       
#define Yellow_  "\033[0;33m"       
#define _Yellow  "\033[0m"       
#define Blue_  "\033[0;34m"         
#define _Blue  "\033[0m"         
#define Purple_  "\033[0;35m"       
#define _Purple  "\033[0m"      
#define White_  "\033[0;37m"        
#define _White  "\033[0m" 
using namespace std;

bool Same(const string& a, const string& b);
bool Same(const char* a, const string& b);
bool Same(const string& a, const char* b);
string GetColoredName(string crName);
int Roll(int min, int max);
void Tokenize(const string& line, vector<string>& arguments);

#endif //__Globals__