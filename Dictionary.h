#include <unordered_set>
#include <string>
using namespace std;

class Dictionary
{
private:
    unordered_set<string> keywords {"while","endwhile", "if", "fi", "else", "return", "put", "get", "int", "endif", "function", "int", "bool", "real", "true", "false"};
    
    unordered_set<string> operators  { "+", "-", "*", "/", "=", "<" , ">", "==", "!=", "<=", "=>"};

    unordered_set<string> separators {"(", ")", "{" , "}", "[", "]", ";", "#", ","};
    
    


public:
    bool isKeyword(string str) {
     return (keywords.find(str) != keywords.end());
    }
    
    bool isSep(string str) {
     return (separators.find(str) != separators.end());
    }
   
    bool isOpr(string str) {
     return (operators.find(str) != operators.end());
    }
};




