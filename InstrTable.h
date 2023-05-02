#include <vector>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

class InstrTable {

private:

stack<int> jumpStack;
vector<vector<string>> table;

public:

int instrAddress = 1;

vector<vector<string>> GetTable() const {
    return table;
}

void GenInstr(string operation, string memoryAddress) {
    string instrAddressString = to_string(instrAddress);
    table.push_back({instrAddressString, operation, memoryAddress});
    instrAddress++;
}


void backPatch() {
    string jumpAddrString = to_string(instrAddress);
    int addr = popJumpStack() - 1;
    table[addr][2] = jumpAddrString;
}

void pushJumpStack() {
    jumpStack.push(instrAddress); 
}

int popJumpStack() {
    if (jumpStack.empty()) {
        return 0;
    }      
    int jmpzAddress = jumpStack.top();
    jumpStack.pop();
    return jmpzAddress;
}


};