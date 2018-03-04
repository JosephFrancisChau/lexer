#include "stdafx.h"
#include "lexer.h"
#include <set>
// testing this code
token::token()
{
}

token::~token()
{
}

bool token::isToken(const string word)
{
	if (isID(word)) {
		tokens.insert(pair<string, string>(word, "identifer"));
		return true;
	}
	
	if (isKeyword(word)) {
		tokens.insert(pair<string, string>(word, "keyword"));
		return true;
	}
		
	if (isInteger(word)) {
		tokens.insert(pair<string, string>(word, "integer"));
		return true;
	}
		
	if (isReal(word)) {
		tokens.insert(pair<string, string>(word, "real"));
		return true;
	}
		
	if (isOperator(word)) {
		tokens.insert(pair<string, string>(word, "operator"));
	}
		
	if (isSeparator(word)) {
		tokens.insert(pair<string, string>(word, "separator"));
		return true;
	}		
}

void token::printToken(const string word) {

}

bool token::isID(const string word) {
	//check the first char
	if (!(word[0] >= 'a'&&word[0] <= 'z' || word[0] >= 'A'&&word[0] <= 'Z'))
		return false;

	//check if the length is one
	size_t len = word.length();
	if (len==1 && !(word[0] >= 'a'&&word[0] <= 'z' || word[0] >= 'A'&&word[0] <= 'Z'))
		return false;

	//check the last char if the length is greater than one
	int j = len - 1;
	if ((word[j] != '$') ||
		!(word[j] >= 'a'&&word[j] <= 'z') || !(word[j] >= 'A'&&word[j] <= 'Z')) {
		return false;
	}

	//check the middle char
	for (int i = 1; i < j; ++i) {
		if (!(word[i] >= '0'&&word[i] <= '9') || !(word[i] >= 'a'&& word[i] <= 'z') || !(word[i] >= 'A'&& word[i] <= 'Z')) {
			return false;
		}
	}

	return true;
}

bool token::isKeyword(const string word) {
	set<string> keywords = { "function", "int", "boolean", "real", "if", "else", "elseif","endif","return", "put","get","while","true","false" };
	set<string>::iterator it = keywords.find(word);
	if (it == keywords.end())
		return false;
	return true;
}

bool token::isInteger(string word) {
	for (size_t i = 0; i < word.length(); ++i) {
		if (!(word[i] >= '0'&&word[i] <= '9'))
			return false;
		if (word.length() > 1 && word[0] == '0')
			return false;
	}
	return true;
}

bool token::isReal(string word) {
	if (word.length() < 3)
		return false;

	size_t found = word.find('.');
	if (found == string::npos)
		return false;

	string sub1 = word.substr(0, found);
	string sub2 = word.substr(found, word.length() - found - 1);

	for (size_t i = 0; i < found; ++i) {
		if (!(sub1[i] >= '0' && sub1[i] <= '9'))
			return false;
	}

	for (size_t i = 0; i < word.length() - found - 1; ++i) {
		if (!(sub2[i] >= '0' && sub2[i] <= '9'))
			return false;
	}

	return true;
}

bool token::isOperator(string word) {
	set<string> operators = { "+","-",".","*","/","%","=" };
	set<string>::iterator it = operators.find(word);
	if (it == operators.end())
		return false;
	return true;
}

bool token::isSeparator(string word) {
	set<string> separators = { "(",")" };
	set<string>::iterator it = separators.find(word);
	if (it == separators.end())
		return false;
	return true;
}
