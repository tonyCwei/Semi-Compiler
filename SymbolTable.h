#include <vector>
#include <string>
#include <cctype>
using namespace std;

class SymbolTable {

private:

string type_;
bool isDeclaring = false;
vector<vector<string>> table;
int memoryAddress = 5000;

public:

vector<vector<string>> GetTable() const {
    return table;
}

void SetType(string type) {
    type_ = type;
}

void SetIsDeclaring(bool target) {
    isDeclaring = target;
}

void Insert(string identifier) {
    if (!isDeclaring){
        return;
    }

    if (!IdExist(identifier)) {
        string memoryAddressString = to_string(memoryAddress);
        table.push_back({identifier, memoryAddressString, type_});
        memoryAddress++;
    } else {
       table.push_back({identifier, "Error", "Identifier already declared"}); 
    }
    
}

bool IdExist(string identifier) {
    for (vector<string> entry : table) {
        if (entry[0] == identifier) {
            return true;
        }
    }
    return false;
}

string GetAddress(string identifier) {
    for (vector<string> entry : table) {
        if (entry[0] == identifier) {
            return entry[1];
        }
    }
    return "Error, identifier not declared";
}





}; // End of Class