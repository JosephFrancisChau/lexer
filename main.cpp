#include "lexer.h"
#include <fstream>
#include <iomanip>  //setw
using namespace std;

void readFile(const string& filename);
void printSourceCode(const string& filename);

int main() {
	string name;
	
	//test three sample source code
	cout << "Enter test file name(format: XXXX.txt): ";
	cin >> name;
	cout << "Source Code:" << endl <<endl;
	printSourceCode(name);
	cout << "\nOutput:\n\n";
	cout << left << setw(10) << "Token" << "\t" << "Lexeme" << endl << endl;
	readFile(name);	

	system("pause");
	return 0;
}

void readFile(const string& filename) {
	ifstream file(filename);
	string word;

	if (file.is_open()) {
		while (file >> word) {
			lexer(word);
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
		}
	}
	else
		cout << "Can't open the file: " << filename << endl;
	file.close();
	file.clear();
	cout << endl;
	return;
}
