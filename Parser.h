#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include "SymbolTable.h"
#include "InstrTable.h"
using namespace std;

class Parser {

private:

int n;
string relopSign;
SymbolTable mySymbolTable;
InstrTable myInstrTable;

public:
    void Analyzer(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules) {
        n = LexicalAnalysis.size();

        for (int i = 0; i < n; i++) {
            R1(LexicalAnalysis, OutProductionRules, i);
        }
    }


    void R1(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ){
        OutProductionRules.at(i) += "<Rat23S> ::= <Opt Function Definitions> # <Opt Declaration List> # <Statement List>\n";
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        // call R2
        if (lexeme == "function") {
            R2(LexicalAnalysis, OutProductionRules, i);
            lexeme = LexicalAnalysis.at(i).second;
            if (lexeme != "#") {
            OutProductionRules.at(i) += "R1 Error, # expected\n";
            }
            i++;
        } else {
            OutProductionRules.at(i) += "<Opt Function Definitions> ::= <Empty>\n";
            OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
            if (lexeme != "#") {
            OutProductionRules.at(i) += "R1 Error, # expected\n";
            }
            i++;
        }

        
        lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "int" || lexeme == "bool" || lexeme == "real") {
            R10(LexicalAnalysis, OutProductionRules, i);
            lexeme = i < n ? LexicalAnalysis[i].second : " ";
            if (lexeme != "#") {
            OutProductionRules.at(i-1) += "R1 Error, # expected\n";
            }
            i++;
        } else {
            OutProductionRules.at(i) += "<Opt Declaration List> ::= <Empty>\n";
            OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
            if (lexeme != "#") {
            OutProductionRules.at(i) += "R1 Error, # expected\n";
            }
            i++;
        }
        R14(LexicalAnalysis, OutProductionRules, i);
    }

    void R2(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ){
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "function") {
            OutProductionRules.at(i) += "<Opt Function Definitions> ::= <Function Definitions>\n";
            R3(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Opt Function Definitions> ::= <Empty>\n";
            OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
        }
        // check #;

    }

    void R3(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Function Definitions> ::= <Function><Function Definitions>’\n";
        R4(LexicalAnalysis, OutProductionRules, i);
        R3P(LexicalAnalysis, OutProductionRules, i);
    }

    void R3P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "function") {
            OutProductionRules.at(i) += " <Function Definitions>’  ::= <Function Definitions>\n";
            R3(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += " <Function Definitions>’  ::= Epsilon\n";
        }
    }

    void R4(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>\n";
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (token == "IDENTIFIER") {
            i++;
            lexeme = i < n ? LexicalAnalysis[i].second : " ";
            if (lexeme == "(") {
                i++;
                R5(LexicalAnalysis, OutProductionRules, i);
                lexeme = i < n ? LexicalAnalysis[i].second : " ";
                if (lexeme == ")") {
                    i++;
                    R10(LexicalAnalysis, OutProductionRules, i);
                    R9(LexicalAnalysis, OutProductionRules, i);
                } else {
                   OutProductionRules.at(i) += "R4 Error, ) Expected\n"; 
                }
            } else {
                OutProductionRules.at(i) += "R4 Error, ( Expected\n";
            }
        } else {
            OutProductionRules.at(i) += "R4 Error, Identifier Expected\n";
        }
    }

    void R5(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (token == "IDENTIFIER") {
            OutProductionRules.at(i) += "<Opt Parameter List> ::= <Parameter List>\n";
            R6(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Opt Parameter List> ::= <Empty>\n";
            OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
            //R29(LexicalAnalysis, OutProductionRules, i);
        }
    }

    void R6(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Parameter List> ::= <Parameter><Parameter List>’\n";
        R7(LexicalAnalysis, OutProductionRules, i);
        R6P(LexicalAnalysis, OutProductionRules, i);
    }

    void R6P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ){
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == ",") {
           OutProductionRules.at(i) += "<Parameter List>’ ::= ,<Parameter List>\n";
           i++;
           R6(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Parameter List>’ ::= Epsilon\n";
        }
    }

    void R7(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Parameter> ::=  <IDs >  <Qualifier>\n";
        mySymbolTable.SetIsDeclaring(false);
        R13(LexicalAnalysis, OutProductionRules, i);
        R8(LexicalAnalysis, OutProductionRules, i);
    }

    void R8(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "int") {
            OutProductionRules.at(i) += " <Qualifier> ::= int\n";
            i++;
        } else if (lexeme == "bool") {
            OutProductionRules.at(i) += " <Qualifier> ::= bool\n";
            i++;
        } else if (lexeme == "real") {
            OutProductionRules.at(i) += " <Qualifier> ::= real\n";
            i++;
        }

    }

    void R9(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "{") {
            OutProductionRules.at(i) += "<Body>  ::=  { < Statement List> }\n";
            i++;
            R14(LexicalAnalysis, OutProductionRules, i);
            //i--;
            lexeme = i < n ? LexicalAnalysis[i].second : " ";
            if (lexeme != "}") {
                OutProductionRules.at(i) += "R9 Error, } expected\n";
            }
            i++;
        } else {
            OutProductionRules.at(i) += "R9 Error, { expected\n";
        }
    }

    void R10(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "int" || lexeme == "bool" || lexeme == "real"){
            OutProductionRules.at(i) += "<Opt Declaration List> ::= <Declaration List>\n";
            R11(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Opt Declaration List> ::= <Empty>\n";
            OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
            //R29(LexicalAnalysis, OutProductionRules, i);
        }
    }

    void R11(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Declaration List>  ::= <Declaration> ; <Declaration List>’\n";
        R12(LexicalAnalysis, OutProductionRules, i);
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == ";") {
            i++;
            R11P(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "R11 Error, ; Expected\n";
        }
    }

    void R11P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "int" || lexeme == "bool" || lexeme == "real") {
            OutProductionRules.at(i) += "<Declaration List>’ ::= <Declaration List>\n";
            R11(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Declaration List>’ ::= Epsilon\n";
        }
    }
    
    void R12(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i ) {
        OutProductionRules.at(i) += "<Declaration> ::= <Qualifier> <IDs>\n";
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        mySymbolTable.SetIsDeclaring(true);
        if (lexeme == "int") {
                mySymbolTable.SetType("integer");
        } else if (lexeme == "bool") {
                mySymbolTable.SetType("boolean");
        }
        R8(LexicalAnalysis, OutProductionRules, i);
        R13(LexicalAnalysis, OutProductionRules, i);
    }

    void R13(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (token == "IDENTIFIER") {
            OutProductionRules.at(i) += "<IDs> ::= <Identifier><IDs>’\n";
            mySymbolTable.Insert(lexeme);
            i++;
            R13P(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "R13 Error, Identifier Expected\n";
        }
    }

    void R13P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == ",") {
            OutProductionRules.at(i) += "<IDs>’ ::= ,<IDs>\n";
            i++;
            R13(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<IDs>’ ::= Epsilon\n";
        }
    }



    void R14(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i){ 
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "{" || token == "IDENTIFIER" || lexeme == "if" || lexeme == "return" || lexeme == "put" || lexeme == "get" || lexeme == "while") {
            OutProductionRules.at(i) += "<Statement List> ::=   <Statement><Statement List>’\n";
            R15(LexicalAnalysis, OutProductionRules, i);
            R14P(LexicalAnalysis, OutProductionRules, i);
        }
    }

    void R14P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "{" || token == "IDENTIFIER" || lexeme == "if" || lexeme == "return" || lexeme == "put" || lexeme == "get" || lexeme == "while") {
            OutProductionRules.at(i) += "<Statement List>’ ::= <Statement List>\n";
            R14(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Statement List>’ ::= Epsilon\n";
        }
    }

    void R15(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "{") {
            OutProductionRules.at(i) += "<Statement> ::=   <Compound>\n";
            R16(LexicalAnalysis, OutProductionRules, i);
        } else if (token == "IDENTIFIER") {
            OutProductionRules.at(i) += "<Statement> ::=   <Assign>\n";
            R17(LexicalAnalysis, OutProductionRules, i);
        } else if (lexeme == "if") {
            OutProductionRules.at(i) += "<Statement> ::=   <If>\n";
            R18(LexicalAnalysis, OutProductionRules, i);
        } else if (lexeme == "return") {
            OutProductionRules.at(i) += "<Statement> ::=   <Return>\n";
            R19(LexicalAnalysis, OutProductionRules, i);
        } else if (lexeme == "put") {
            OutProductionRules.at(i) += "<Statement> ::=   <Print>\n";
            R20(LexicalAnalysis, OutProductionRules, i);
        }  else if (lexeme == "get") {
            OutProductionRules.at(i) += "<Statement> ::=   <Scan>\n";
            R21(LexicalAnalysis, OutProductionRules, i);
        }  else if (lexeme == "while") {
            OutProductionRules.at(i) += "<Statement> ::=   <While>\n";
            R22(LexicalAnalysis, OutProductionRules, i);
        }  
    }

    void R16(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Compound> ::= { <Statement List> }\n";
        i++;
        R14(LexicalAnalysis, OutProductionRules, i);
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme != "}") {
            OutProductionRules.at(i) += "R16 Error, } expected\n";
        }
        //i++;
    }

    void R17(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Assign> ::=  <Identifier> = <Expression> ;\n";
        string save = i < n ? LexicalAnalysis[i].second : " ";
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "=") {
         i++;        
         R25(LexicalAnalysis, OutProductionRules, i);
         myInstrTable.GenInstr("POPM", mySymbolTable.GetAddress(save));
         
         lexeme = i < n ? LexicalAnalysis[i].second : " ";
         if (lexeme != ";") {
            OutProductionRules.at(i) += "R17 Error, ; expected\n";
         }
        } else {
            OutProductionRules.at(i) += "R17 Error, = expected\n";
        }
    }

    void R18(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<If> ::= if ( <Condition> ) <Statement><If>’\n";
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "(") {
            i++;
            R23(LexicalAnalysis, OutProductionRules, i);
            i--;
            lexeme = i < n ? LexicalAnalysis[i].second : " ";
            if (lexeme != ")") {
                OutProductionRules.at(i) += "R18 Error, ) expected\n";
            } else {
            i++;    
            R15(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.backPatch();
            i++;
            R18P(LexicalAnalysis, OutProductionRules, i);
            }
        } else {
            OutProductionRules.at(i) += "R18 Error, ( expected\n";
        }
        //myInstrTable.GenInstr("LABEL", " ");
    }

    void R18P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "fi") {
            OutProductionRules.at(i) += "<If>’ ::= fi\n";
            myInstrTable.GenInstr("LABEL", " ");
        } else if (lexeme == "else") {
            OutProductionRules.at(i) += "<If>’ ::= else <Statement> fi\n";
            i++;
            myInstrTable.GenInstr("LABEL", " ");
            R15(LexicalAnalysis, OutProductionRules, i);
            //myInstrTable.backPatch();
        }  
    }    

    void R19(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        OutProductionRules.at(i) += "<Return> ::=  return<Return>’\n";
        i++;
        R19P(LexicalAnalysis, OutProductionRules, i);
    }
    
    void R19P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == ";") {
            OutProductionRules.at(i) += "<Return>’ ::= ;\n";
        } else {
            OutProductionRules.at(i) += "<Return>’ ::= <Expression>;\n";
            R25(LexicalAnalysis, OutProductionRules, i);
            // error checking
            //i++;
            
        }
    }   

    void R20(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Print> ::= put (<Expression>);\n";
        
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "(") {
            i++;
            R25(LexicalAnalysis, OutProductionRules, i);
            lexeme = i < n ? LexicalAnalysis[i].second : " ";
            if (lexeme == ")") {
                i++;
            } else if (lexeme == ";") {

            }
            
            // if (lexeme != ")"){
            //   OutProductionRules.at(i) += "R20 Error, ) Expected\n";  
            // }
            // i++;
            // lexeme = i < n ? LexicalAnalysis[i].second : " ";
            // if (lexeme != ";"){
            //   OutProductionRules.at(i) += "R20 Error, ; Expected\n";  
            // }
            
        } else {
            OutProductionRules.at(i) += "R20 Error, ( Expected\n";
        }
        myInstrTable.GenInstr("OUT", "");
    }

    void R21(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Scan> ::= get (<IDs>);\n";
        mySymbolTable.SetIsDeclaring(false);
        myInstrTable.GenInstr("IN", "");
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "(") {
            i++;
            string save = i < n ? LexicalAnalysis[i].second : " ";
            R13(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("POPM", mySymbolTable.GetAddress(save));
            i++;
            //lexeme = i < n ? LexicalAnalysis[i].second : " ";
            // if (lexeme != ")"){
            //   OutProductionRules.at(i) += "R21 Error, ) Expected\n";  
            // }
            // i++;
            // lexeme = i < n ? LexicalAnalysis[i].second : " ";
            // if (lexeme != ";"){
            //   OutProductionRules.at(i) += "R21 Error, ; Expected\n";  
            // }
            // i++;
        } else {
            OutProductionRules.at(i) += "R21 Error, ( Expected\n";
        }
    }

    void R22(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        int addr = myInstrTable.instrAddress;
        myInstrTable.GenInstr("LABEL", " ");
        OutProductionRules.at(i) += "<While> ::= while ( <Condition> ) <Statement> endwhile\n";
        i++;
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "(") {
            i++;
            R23(LexicalAnalysis, OutProductionRules, i);
            R15(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("JMP", to_string(addr));
            myInstrTable.backPatch();
            i++;
            // lexeme = i < n ? LexicalAnalysis[i].second : " ";
            // if (lexeme == ")") {
            //     i++;
            //     R15(LexicalAnalysis, OutProductionRules, i);
            //     lexeme = i < n ? LexicalAnalysis[i].second : " ";
            //     if (lexeme != "endwhile") {
            //         OutProductionRules.at(i) += "R22 Error, endwhile Expected\n";
            //     }
            // } else {
            //     OutProductionRules.at(i) += "R22 Error, ) Expected\n";
            // }
        } else {
            OutProductionRules.at(i) += "R22 Error, ( Expected\n";
        }


    }
    


    void R23(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Condition> ::=     <Expression>  <Relop>   <Expression>\n";
        R25(LexicalAnalysis, OutProductionRules, i);
        R24(LexicalAnalysis, OutProductionRules, i);
        R25(LexicalAnalysis, OutProductionRules, i);
        
        if (relopSign == ">") {
            myInstrTable.GenInstr("GRT", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        } else if (relopSign == "<") {
            myInstrTable.GenInstr("LES", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        } else if (relopSign == "==") {
            myInstrTable.GenInstr("EQU", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        } else if (relopSign == "!=") {
            myInstrTable.GenInstr("NEQ", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        } else if (relopSign == "=>") {
            myInstrTable.GenInstr("GEQ", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        } else if (relopSign == "<=") {
            myInstrTable.GenInstr("LEQ", " ");
            myInstrTable.pushJumpStack();
            myInstrTable.GenInstr("JMPZ", " ");
        }
    }

    void R24(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        relopSign = lexeme;
        if (lexeme == "==") {
            OutProductionRules.at(i) += "<Relop> ::=  ==\n";
        } else if (lexeme == "!=") {
            OutProductionRules.at(i) += "<Relop> ::=  !=\n";
        } else if (lexeme == ">") {
            OutProductionRules.at(i) += "<Relop> ::=  >\n";
        } else if (lexeme == "<") {
            OutProductionRules.at(i) += "<Relop> ::=  <\n";
        } else if (lexeme == "<=") {
            OutProductionRules.at(i) += "<Relop> ::=  <=\n";
        } else if (lexeme == "=>") {
            OutProductionRules.at(i) += "<Relop> ::=  =>\n";
        }
        i++;
    }


    void R25(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
         OutProductionRules.at(i) += "<Expression>   ::= <Term><Expression>’\n";
         R26(LexicalAnalysis, OutProductionRules, i);
         R25P(LexicalAnalysis, OutProductionRules, i);
    }

    void R25P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "+") {
            OutProductionRules.at(i) += "<Expression>’  ::= + <Term><Expression>’\n";
            i++;
            R26(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("ADD", " ");
            R25P(LexicalAnalysis, OutProductionRules, i);
        } else if (lexeme == "-") {
            OutProductionRules.at(i) += "<Expression>’  ::= - <Term><Expression>’\n";
            i++;
            R26(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("SUB", " ");
            R25P(LexicalAnalysis, OutProductionRules, i);
        } else {
            OutProductionRules.at(i) += "<Expression>’ ::= Epsilon\n";
        }
    }


    void R26(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        OutProductionRules.at(i) += "<Term>    ::=   <Factor><Term>’\n";
        R27(LexicalAnalysis, OutProductionRules, i);
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == ")") {
            i++;
        }
        R26P(LexicalAnalysis, OutProductionRules, i);
    }

    void R26P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i){
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "*") {
            OutProductionRules.at(i) += "<Term>’ ::= * <Factor><Term>’\n";
            i++;
            R27(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("MUL", " ");
        } else if (lexeme == "/") {
            OutProductionRules.at(i) += "<Term>’ ::= / <Factor><Term>’\n";
            i++;
            R27(LexicalAnalysis, OutProductionRules, i);
            myInstrTable.GenInstr("DIV", " ");
        } 
        // else if (lexeme == ")"){
        //     i++;
        //     R26P(LexicalAnalysis, OutProductionRules, i);
        // } 
        else {
            OutProductionRules.at(i) += "<Term>’ ::= Epsilon\n";
        }
    }

    void R27(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        if (lexeme == "-") {
            OutProductionRules.at(i) += "<Factor> ::=  -<Primary>\n";
            i++;
        } else {
            OutProductionRules.at(i) += "<Factor> ::= <Primary>\n";
        }

        R28(LexicalAnalysis, OutProductionRules, i);
    }

    void R28(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        
        if (token == "IDENTIFIER") {
            OutProductionRules.at(i) += "<Primary> ::= <Identifier><Primary>'\n";
            myInstrTable.GenInstr("PUSHM", mySymbolTable.GetAddress(lexeme));
            i++;
            R28P(LexicalAnalysis, OutProductionRules, i);
        } else if (token == "INTEGAR") {
            OutProductionRules.at(i) += "<Primary> ::= <Integer>\n";
            myInstrTable.GenInstr("PUSHI", lexeme);
            i++;
        } else if (lexeme == "(") {
            OutProductionRules.at(i) += "<Primary> ::= ( <Expression> )\n";
            i++;
            R25(LexicalAnalysis, OutProductionRules, i);
            //error checking
            //i++;
 
        } else if (token == "Real") {
            OutProductionRules.at(i) += "<Primary> ::= <Real>\n";
            i++;
        } else if (lexeme == "true") {
            OutProductionRules.at(i) += "<Primary> ::= true\n";
            myInstrTable.GenInstr("PUSHI", "1");
            i++;
        } else if (lexeme == "false") {
            OutProductionRules.at(i) += "<Primary> ::= false\n";
            myInstrTable.GenInstr("PUSHI", "0");
            i++;
        }
        

    }
    
    void R28P(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
        string token = i < n ? LexicalAnalysis[i].first : " ";
        string lexeme = i < n ? LexicalAnalysis[i].second : " ";
        mySymbolTable.SetIsDeclaring(false);
        if (lexeme == "(") {
            OutProductionRules.at(i) += "<Primary>’ := ( <IDs> )\n";
            i++;
            R13(LexicalAnalysis, OutProductionRules, i);
            // error checking
            i++;
        } else {
            OutProductionRules.at(i) += "<Primary>’ := Epsilon\n";
        }
    }

    // void R29(const vector<pair<string, string>> &LexicalAnalysis, vector<string> &OutProductionRules, int& i) {
    //     OutProductionRules.at(i) += "<Empty> ::= Epsilon\n";
    // }

    void PrintInstrTable(ofstream& outFile) {
        outFile << "Address" << "          " << "Op" << "            " << "Oprnd\n";
        vector<vector<string>> table = myInstrTable.GetTable();
        for (vector<string> entry : table) {
            outFile << entry[0] << "             " << entry[1] << "            " << entry[2] << "\n";
        }
    }

    void PrintSymbolTable(ofstream& outFile) {
        outFile << "Identifier" << "          " << "MemoryLocation" << "            " << "Type\n";
        vector<vector<string>> table = mySymbolTable.GetTable();
        for (vector<string> entry : table) {
            outFile << entry[0] << "                          " << entry[1] << "            " << entry[2] << "\n";
        }
    }


};