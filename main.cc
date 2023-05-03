#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include "Solution.h"
#include "Parser.h"
using namespace std;


int main() {
  vector<char> lexemes;
  string fileName;
  Dictionary myDict; 
  
  cout << "Please enter your input file name(include .txt): ";
  getline(cin, fileName);
  while (true) {
  ifstream inFile(fileName);

  if (inFile.is_open()) {
  char c;
  while(inFile.get(c)) {
    lexemes.push_back(c);
  }
  lexemes.push_back('\n');
  inFile.close();
  break;
  } else {
    cout << "No file found, please re-enter your input file name(include .txt): ";
    getline(cin, fileName);
  }
  }
   
  // ifstream inFile("input.txt");
   
  // char c;
  // while(inFile.get(c)) {
  //   lexemes.push_back(c);
  // }
  // lexemes.push_back('\n');
  // inFile.close();


 //Lexical Analysis
  Solution mySolution;
  vector<pair<string, string>> LexicalAnalysis;
  string lexeme;
  string opr;
  int curState = 1;
  bool isComment = false;
  for (size_t i = 0; i < lexemes.size(); i++)
  {
    char c = lexemes[i];
     // if [* is not closed by *], anything after [* will be ignored 
    

     isComment = c == '['  && lexemes[i+1] == '*' ? true : isComment;
     if (isComment) {
     isComment = c == ']' && lexemes[i-1] == '*' ? false : isComment;  
     continue;
     }
     // if Not Comment, do following

      lexeme += c;
      mySolution.Lexer(c, lexeme, curState, LexicalAnalysis);
     
    if (myDict.isOpr(string(1,c)) || c == '!') {
       opr += c;
     }
     mySolution.addOpr(c, opr, LexicalAnalysis);

     if (myDict.isSep(string(1,c))) {
       LexicalAnalysis.push_back(make_pair("SEPARETOR", string(1,c)));
     } 

  }



  //Syntax Analysis
  int n = LexicalAnalysis.size();
  vector<string> ProductionRules(n*2);
  Parser MyParser;
  MyParser.Analyzer(LexicalAnalysis, ProductionRules);

  // Parser MyParser;
  // try{
  //   MyParser.Analyzer(LexicalAnalysis, ProductionRules);
  // } catch (length_error& ex) {
  //   cout << "Length_error";
  // }

   //To Terminal
  //  for (int i = 0; i < n; i++) {
  //   cout << "TOKEN:" << LexicalAnalysis[i].first <<"              " <<  "LEXEME:" << LexicalAnalysis[i].second << "\n";
  //   cout << ProductionRules[i];
  //   cout << "\n";
  //   }

  //   MyParser.PrintInstrTable();
  //   cout << "\n" << "\n";
  //   MyParser.PrintSymbolTable();
    

  //To Output File
  ofstream outFile(string("Output.txt"));
  if (outFile.is_open()) { 
  // Output production rules        
  // for (int i = 0; i < n; i++) {
  //   outFile << "TOKEN:" << LexicalAnalysis[i].first <<"              " <<  "LEXEME:" << LexicalAnalysis[i].second << "\n";
  //   outFile << ProductionRules[i];
  //   outFile << "\n";
  // }
    MyParser.PrintInstrTable(outFile);
    outFile << "\n" << "\n";
    MyParser.PrintSymbolTable(outFile);
    outFile.close();
    cout << "Output.txt file has been generated under the same folder";
  } else {
    cout << "Error, Output.txt file was not generated";
  }
  cin.get();

  return 0;


}




