#include "stdafx.h"
//#include "lexer.h"
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <iomanip>  //setw

using namespace std;

map<string, string> tokens;
set<string> operators = { "=","==","^=",">","<","=>","<=","+","-","*","/" };
set<string> separators = { "%%", "[", "]", ";",",", "{","}","(",")","!",":" };
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
void helper2(string word1, string word2, string word3);
ofstream coutfile;

void readFile(const string& filename);
void printSourceCode(const string& filename);
void testSeparator(string& word);
int main() {
	string name, name2;

	//test sample source code
	cout << "Enter test file name(format: filename.txt): ";
	cin >> name;

	cout << "Enter test output file name(format: filename.txt): ";
	cin >> name2;

	coutfile.open(name2);
	cout << "source Code:" << endl << endl;
	coutfile << "Source Code:" << endl << endl;
	printSourceCode(name);

	cout << "\nOutput:\n\n";
	coutfile << "\nOutput:\n\n";
	cout << left << setw(10) << "Token" << "\t" << "Lexeme" << endl << endl;
	coutfile << left << setw(10) << "Token" << "\t" << "Lexeme" << endl << endl;
	readFile(name);
	coutfile.close();
	system("pause");
	return 0;
}

void readFile(const string& filename) {
	ifstream file(filename);
	string word;

	if (file.is_open()) {
		while (file >> word) {
			testSeparator(word);
		}
	}
	else
		cout << "Can't open the file: " << filename << endl;

	file.close();
	file.clear();
	return;
}

void printSourceCode(const string& filename) {
	ifstream file(filename);
	string line;

	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			cout << line << endl;
			coutfile << line << endl;
		}
	}
	else
		cout << "Can't open the file: " << filename << endl;
	file.close();
	file.clear();
	cout << endl;
	//coutfile << endl;
	return;
}


void lexer(string& word) {
	string temp1, temp2, value, type;
	size_t len = word.length();
	set<string>::iterator it;
	if (!comments.empty()) {
		if (word != "!") {
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
		if (word[len - 1] == ')' || word[len - 1] == ']' || word[len - 1] == '}' || word[len - 1] == '!') {
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
	
	//ofstream coutfile;
	//coutfile.open("result.txt");
	//coutfile << "\nOutput:\n\n";
	coutfile << left << setw(10) << type << "\t" << value << endl;
	//coutfile.close();
	
}

void helper(string word1, string word2) {
	lexer(word1);
	word1.clear();
	lexer(word2);
	word2.clear();
}

void testSeparator(string& word) {
	set<string>::iterator it1;
	set<string>::iterator it2;
	size_t len = word.length();
	string sub1, sub2;
	if (!word.empty()) {
		for (size_t i = 0; i < len; ++i) {
			string temp = word[i] + "";
			it1 = separators.find(temp);
			it2 = operators.find(temp);
			if (it1 != separators.end()) {
				sub1 = word.substr(0, i);
				sub2 = word.substr(i);
				if(sub1.length()>0)
					lexer(sub1);
				if (sub2.length()>0)
					lexer(sub2);
				sub1.clear();
				sub2.clear();
			}
			else if (it2 != operators.end()) {
				sub1 = word.substr(0, i);
				sub2 = word.substr(i);
				if (sub1.length()>0)
					lexer(sub1);
				if (sub2.length()>0)
					lexer(sub2);
				sub1.clear();
				sub2.clear();
			}
			else {
				lexer(word);
				return;
			}
		}
	}
	return;
}
