#include "stdafx.h"
#include "lexer.h"
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <stack>
#include <iomanip>  //setw

using namespace std;

map<string, string> tokens;
set<string> operators = { "=","==","^=",">","<","=>","<=","+","-","*","/" };
set<string> separators = { "%%", "[", "]", ";",",", "{","}","(",")","!" };
set<string> keywords = { "function", "int", "boolean",
"real", "if", "endif", "else", "return", "put", "get", "while", "true", "false" };
stack<string> comments;

string type, value;
bool isToken = false;
bool isOperator = false;
bool isSeparator = false;
bool isKeyword = false;
bool isInteger = false;
bool isReal = false;
bool isID = false;

void lexer(string& word);
void printToken(string& type, string& value);
void helper(string word1, string word2);

void lexer(string& word) {
	string temp1, temp2, value, type;
	size_t len = word.length();
	set<string>::iterator it;
	if (!comments.empty()) {
		if (word != "!" ) {
			comments.push(word);
			return;
		}
		else {
			while (!comments.empty()) {
				comments.pop();
			}
			isToken = true;
			isSeparator = true;
			value = word;
			type = "separator";
			printToken(type, word);
			return;
		}
	}

	if (!word.empty() && comments.empty()) {
		if ((word[0] == '(') || (word[0] == '[') || (word[0] == '{') || word[0] == '!') {
			isToken = true;
			isSeparator = true;
			value = word[0];
			type = "separator";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
			if (word[0] == '!' || word == "!") {
				comments.push("!");
			}
			if (len > 1) {
				word = word.erase(0, 1);
				return lexer(word);
			}
			return;
		}

		//check first two char
		if (word.substr(0, 2) == "%%") {
			isSeparator = true;
			isToken = true;
			value = "%%";
			type = "separator";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
			word = word.erase(0, 2);
			if (!word.empty()) {
				return lexer(word);
			}
			else
				return;
		}
				
		//check the last char		
		if ( word[len - 1] == ')' || word[len - 1] == ']' || word[len - 1] == '}' || word[len - 1] == '!') {
			if (len == 1) {
				isSeparator = true;
				isToken = true;
				value = word;
				type = "separator";
				tokens.insert(pair<string, string>(type, value));
				printToken(type, value);
				return;
			}
			else {
				temp1 = word.substr(0, len - 1);
				temp2 = word.substr(len - 1, 1);
				helper(temp1, temp2);
			}
		}

		//check is operator
		it = operators.find(word);
		if (it != operators.end()) {
			isOperator = true;
			isToken = true;
			value = word;
			type = "operator";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
			return;
		}

		//check is keyword
		it = keywords.find(word);
		if (it != keywords.end()) {
			isToken = true;
			isKeyword = true;
			value = word;
			type = "keyword";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
			return;
		}

		//check identifer
		bool first = (word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A'&&word[0] <= 'Z');
		bool last = (word[len - 1] >= 'a' && word[len - 1] <= 'z') || (word[len - 1] >= 'A'&&word[len - 1] <= 'Z')
			|| (word[len - 1] == '$');
		bool t;
		if (len == 1 && first) {
			isToken = true;
			isID = true;
			value = word;
			type = "identifer";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
		}
		if (len > 1 && first && last) {
			//check the middle char
			for (size_t i = 1; i < len - 1; ++i) {
				t = (word[i] >= '0'&&word[i] <= '9') ||
					(word[i] >= 'a'&& word[i] <= 'z') ||
					(word[i] >= 'A'&& word[i] <= 'Z');
				isID = t && true;
			}
			if (isID) {
				isID = true;
				isToken = true;
				value = word;
				type = "identifer";
				tokens.insert(pair<string, string>(type, value));
				printToken(type, value);
				return;
			}
		}
		
		//check integer and real
		if (word[0] >= '0' && word[0] <= '9') {
			//check is integer;
			bool b;
			for (size_t i = 0; i <= len; ++i) {
				b = (word[i] >= '0'&&word[i] <= '9');
				isInteger = b && true;
			}
			if (isInteger == true) {
				isToken = true;
				isInteger = true;
				value = word;
				type = "integer";
				tokens.insert(pair<string, string>(type, value));
				printToken(type, value);
				return;
			}

			//check is real
			if (len >= 3) {
				if (word.find('.') != string::npos) {
					size_t p = word.find('.');
					string subword1 = word.substr(0, p);
					string subword2 = word.substr(p + 1);
					bool isReal1 = true;
					bool isReal2 = true;
					bool b;
					for (size_t i = 0; i < subword1.length(); ++i) {
						b = (word[i] >= '0'&&word[i] <= '9');
						isReal1 = b && true;
					}
					for (size_t i = 0; i < subword1.length(); ++i) {
						b = (word[i] >= '0'&&word[i] <= '9');
						isReal2 = b && true;
					}
					isReal = isReal1 && isReal2;
					if (isReal) {
						isToken = true;
						value = word;
						type = "real";
						tokens.insert(pair<string, string>(type, value));
						printToken(type, value);
						return;
					}
				}
			}
		}

	}
	return;
}

void printToken(string& type, string& value) {
	cout << left << setw(10) << type << "\t" << value << endl;
}

void helper(string word1, string word2) {
	lexer(word1);
	word1.clear();
	lexer(word2);
	word2.clear();
}
