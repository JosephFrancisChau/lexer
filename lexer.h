#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>
using namespace std;

class token
{
public:
	token();
	~token();
	bool isToken(const string word);
		
private:
	map<string, string> tokens;
	
	bool isID(const string word);
	bool isKeyword(const string word);
	bool isInteger(string word);
	bool isReal(string word);
	bool isOperator(string word);
	bool isSeparator(string word);
};

void getWord(const string& name) {
	ifstream file(name);
	string word;
	token token;

	if (file.is_open()) {
		while (file >> word)
			lexer(word);
	}
	else
		std::cout << "Can't open the file!";

	file.close();
	file.clear();
	return;
}

void lexer(const string& word) {
	token token;
	map<string, string>::iterator it;
	if (token.isToken(word)) {
		cout << it->first << "\t\t" << it->second << endl;
	}

	else
		cout << "It is not a legal token!\n";
}
