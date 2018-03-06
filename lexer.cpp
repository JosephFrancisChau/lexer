//#include "lexer.h"
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <iomanip>  //setw
#include <vector>
using namespace std;

map<string, string> tokens;
set<string> operators = { "=","==","^=",">","<","=>","<=","+","-","*","/" };
set<string> separators = { "%%", "[", "]", ";",",", "{","}","(",")","!",":" };
set<string> keywords = { "function", "int", "boolean",
"real", "if", "endif", "else", "return", "put", "get", "while", "true", "false" };
stack<string> comments;
map<size_t, size_t> positions;	//store all the separators and operators address and length

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

ofstream coutfile;

void readFile(const string& filename);
void printSourceCode(const string& filename);
void testSeparator(string& word);
void splitWord(string& word);
bool isSepOrOpe(string& word);
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
		it = separators.find(word);
		//check is separator
		if (it != separators.end()) {
			isToken = true;
			isSeparator = true;
			value = word;
			type = "separator";
			tokens.insert(pair<string, string>(type, value));
			printToken(type, value);
			if (word == "!") {
				comments.push("!");
			}
			return;
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
			if (len == 1) {
				isToken = true;
				isInteger = true;
				value = word;
				type = "integer";
				tokens.insert(pair<string, string>(type, value));
				printToken(type, value);
				return;
			}
			isInteger = true;
			bool isDigit;
			for (size_t i = 1; i <= len; ++i) {
				isDigit = (word[i] >= '0'&&word[i] <= '9');
				isInteger = isDigit && isInteger;
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

void testSeparator(string& word) {
	if (!word.empty()) {
		if (isSepOrOpe(word)) {
			splitWord(word);
		}
		else {
			lexer(word);
		}
	}
	return;
}

void splitWord(string& word) {
	map<size_t, size_t>::iterator p;
	string sub1, sub2, sub3;
	vector<string> cleanwords;
	size_t start = 0;
	set<string>::iterator it1;
	set<string>::iterator it2;
	size_t len, firstP;
	bool isSep = false;
	bool isOpe = false;
	
	while (isSepOrOpe(word)) {
		// find the first position of the sep or ope
		p = positions.begin();
		firstP = p->first;
		len = p->second;
		
		//split word to two parts, because once there is a sep or ope at least can be split to two parts
		if (firstP == 0) {
			sub1 = word.substr(0, len);
			sub2 = word.substr(len);
		}
		else if (firstP == word.length() - 1) {
			sub1 = word.substr(0, firstP);
			sub2 = word.substr(firstP);
		}
		else {
			sub1 = word.substr(0, firstP);
			sub2 = word.substr(firstP);
		}
		
		cleanwords.push_back(sub1);
		
		word = sub2;
	}
	for (unsigned i = 0; i < cleanwords.size(); ++i) {
		lexer(cleanwords.at(i));
	}
	
	return;
}

bool isSepOrOpe(string& word) {
	positions.clear();
	bool exist = false;
	set<string>::iterator it1;
	set<string>::iterator it2;
	size_t len = word.length(), iSep, iOpe;
	bool isSep = false;
	bool isOpe = false;
	if (!word.empty()) {
		for (it1 = separators.begin(); it1 != separators.end(); ++it1) {
			if (word.find(*it1) != string::npos) {
				isSep = true;
				iSep = word.find(*it1);
				positions.insert(pair<size_t, size_t>(iSep, (*it1).length()));
			}
		}
		for (it2 = operators.begin(); it2 != operators.end(); ++it2) {
			if (word.find(*it2) != string::npos) {
				isOpe = true;
				iOpe = word.find(*it2);
				positions.insert(pair<size_t, size_t>(iOpe, (*it2).length()));
			}
		}
		exist = isSep || isOpe;
	}
	return exist;
}
