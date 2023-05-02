#include <unordered_map>
#include <vector>
#include <cctype>
#include <string>
#include "Dictionary.h"
using namespace std;



class Solution
{
private:
   Dictionary myDict; 
   
   

//                 |   L     D    .
// q0 =         1  |   2     5    5 
// Accept  =    2  |   3     4    5 
// Accept  =    3  |   3     4    5
// Accept  =    4  |   3     4    5
// Nontoken=    5  |   5     5    5 
   unordered_map<int, vector<int>> idDFSM = 
   {
    {1, {2, 5, 5}},
    {2, {3, 4, 5}},
    {3, {3, 4, 5}},
    {4, {3, 4, 5}},
    {5, {5, 5, 5}},
   };




//                 |   D     .    L
// q0 =         1  |   2     7    7 
// int     =    2  |   2     4    7 
// NonToken=    3  |   5     7    7
// NonToken=    4  |   6     7    7
// Real    =    5  |   5     7    7
// Real    =    6  |   6     7    7
// NonToken=    7  |   7     7    7 
   unordered_map<int, vector<int>> numDFSM = {
    {1, {2, 7, 7}},
    {2, {2, 4, 7}},
    {3, {5, 7, 7}},
    {4, {6, 7, 7}},
    {5, {5, 7, 7}},
    {6, {6, 7, 7}},
    {7, {7, 7, 7}}
   };





public:
   void addOpr(char c, string &opr, vector<pair<string, string>> &answer) {
      bool isTerm = isalpha(c) || isdigit(c) || c == ' ' || c == '\n' || c == '\t' || myDict.isSep(string(1,c));
      if (isTerm && myDict.isOpr(opr)) {
            answer.push_back(make_pair("OPERATOR", opr));
            opr = "";
      } else if (isTerm && !myDict.isOpr(opr) && opr != "") {
         answer.push_back(make_pair("NON-TOKEN", opr));
         opr = "";
      }
   }

   void idLexer(char c, int &curState) {
     if (isalpha(c)) {
        curState = idDFSM[curState][0];
       } else if (isdigit(c)) {
        curState = idDFSM[curState][1];
       } else if (c == '.') {
        curState = idDFSM[curState][2];
       }
   }

   void idHandler(string &lexeme, int &curState, vector<pair<string, string>> &answer) {
      lexeme.pop_back();
      switch(curState) {
        case 1:
         //nothing happens, state 1 is q0
        break;

        case 2:
        case 3:
        case 4:
         // id
         if (myDict.isKeyword(lexeme)) {
          answer.push_back(make_pair("KEYWORD", lexeme));
         } else {
         answer.push_back(make_pair("IDENTIFIER", lexeme)); 
         }
        break;

        case 5:
        answer.push_back(make_pair("NON-TOKEN", lexeme)); 
        break;
      } 
   }


    void numLexer(char c, int &curState) {
     if (isalpha(c)) {
        curState = numDFSM[curState][2];
       } else if (isdigit(c)) {
        curState = numDFSM[curState][0];
       } else if (c == '.') {
        curState = numDFSM[curState][1];
       }
   }

   void numHandler(string &lexeme, int &curState, vector<pair<string, string>> &answer) {
      lexeme.pop_back();
      switch(curState) {
        case 1:
         //nothing happens, state 1 is q0
          
        break;

        case 2:
        //int
        answer.push_back(make_pair("INTEGAR", lexeme));
        break;

        case 5:
        case 6:
         //real
        answer.push_back(make_pair("Real", lexeme));
        break;

        case 3:
        case 4:
        case 7:
        answer.push_back(make_pair("NON-TOKEN", lexeme)); 
        break;
      } 
   }

  
  
  void Lexer(char c, string &lexeme, int &curState, vector<pair<string, string>> &answer)
  
  {
    bool isTerm = c == '\t' || c == '!' || c == ' ' || c == '\n' || myDict.isSep(string(1,c)) || myDict.isOpr(string(1,c));
    bool potentialId = isalpha(lexeme[0]); 
    bool potentialNum = isdigit(lexeme[0]) || lexeme[0] == '.'; 
    
    if (!isTerm) {        
       if (potentialId) {
       idLexer(c, curState);
       }
       
       if(potentialNum){
        numLexer(c, curState);
       }

    } else {
      if (potentialId) {
        idHandler(lexeme, curState, answer);
      } else if (potentialNum){ 
        numHandler(lexeme, curState, answer);
      } else {
         lexeme.pop_back();
         if (answer.size() > 0 && lexeme != ""){
            answer.push_back(make_pair("NON-TOKEN", lexeme));
         }
      }
     curState = 1;
     lexeme = "";
     }

     // if (myDict.isSep(string(1,c))) {
      //   answer.push_back(make_pair("SEPARETOR", string(1,c)));
      // } else if (myDict.isOpr(string(1,c)) || c == '!') {
      //   opr += c;
      // }
      // addOpr(c, opr, answer);

      

  }
};







