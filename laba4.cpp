#include <iostream>
#include <stack>
#include <list>
#include <fstream>
using namespace std;

/*
laba4.cpp:
В игре "Балда" для заданной конфигурации букв и заданного набора
слов определить последовательность ходов указанной длины, которая
принесет нерномуитроку наебольшее количестне очков.
*/

const int SIZE = 5;
struct cell {
	char letter;
	bool isAvailible;
};

void GetWords(string* words, int  n) {
	for (int i = 0; i < n; i++) {
		cout << words[i] << endl;
	}
	cout << endl;
}

void FillTheField(cell** field, string word) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			field[i][j].letter = '0';
			if ((i == (SIZE - 1) / 2 + 1) || (i == (SIZE - 1) / 2 - 1)) {
				field[i][j].isAvailible = 1;
			}
			else {
				field[i][j].isAvailible = 0;
			}
		}
	}
	for (int i = 0; i < SIZE; i++) {
		field[(SIZE-1)/2][i].letter = word[i];
	}
}
void GetField(cell** field) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << field[i][j].letter << "(" << field[i][j].isAvailible << ")" << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void FindAWord(string word, stack<char> &tryword, cell** field,int** covered, int x, int y, bool &done) {
	if ((x == SIZE) || (y == SIZE) || (x == -1) || (y == -1)) { return; }
	if ((field[x][y].letter == '0') || done || (covered[x][y] == 1)) { return; }
	tryword.push(field[x][y].letter);
	covered[x][y] = 1;
	stack<char> triedword = tryword;
	string triedword1;
	int size = triedword.size();
	for (int i = 0; i < size; i++) {
		triedword1 += triedword.top();
		triedword.pop();
	} 
	reverse(triedword1.begin(), triedword1.end());
	if (triedword1 == word) { done = true; tryword.pop(); return; }
	FindAWord(word, tryword, field, covered, x + 1, y, done);
	FindAWord(word, tryword, field, covered, x - 1, y, done);
	FindAWord(word, tryword, field, covered, x, y + 1, done);
	FindAWord(word, tryword, field, covered, x, y - 1, done);
	tryword.pop(); return;
}

void clearthat(int** mas, stack<char> &s) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			mas[i][j] = 0;
		}
	}
	int size = s.size();
	for (int i = 0; i < size; i++) {
		s.pop();
	}
}

int IsAWordPlacable(int len, string word, cell** field, int x, int y) {
	if (!field[x][y].isAvailible) { return -1; }
	int l = len - 1;
	string newword1;
	string newword2;
	for (int j = 0; j < l; j++) {
		newword1 += word[j+1];
		newword2 += word[l - j - 1];
	}
	bool done = false;
	stack<char> tw;
	int** covered = new int * [SIZE];
	for (int i = 0; i < SIZE; i++) {
		covered[i] = new int[SIZE];
	}
	FindAWord(newword1, tw, field, covered, x + 1, y, done); clearthat(covered, tw);
	if (done) { return 0; }
	FindAWord(newword2, tw, field, covered, x + 1, y, done); clearthat(covered, tw);
	if (done) { return l; }
	FindAWord(newword1, tw, field, covered, x - 1, y, done); clearthat(covered, tw);
	if (done) { return 0; }
	FindAWord(newword2, tw, field, covered, x - 1, y, done); clearthat(covered, tw);
	if (done) { return l; }
	FindAWord(newword1, tw, field, covered, x, y + 1, done); clearthat(covered, tw);
	if (done) { return 0; }
	FindAWord(newword2, tw, field, covered, x, y + 1, done); clearthat(covered, tw);
	if (done) { return l; }
	FindAWord(newword1, tw, field, covered, x, y - 1, done); clearthat(covered, tw);
	if (done) { return 0; }
	FindAWord(newword2, tw, field, covered, x, y - 1, done); clearthat(covered, tw);
	if (done) { return l; }
}

bool ALetterIsConnected(cell** field, int x, int y) {
	if ((x != SIZE)) {
		if (field[x + 1][y].letter != '0') {
			return true;
		}
	}
	if ((x != 0)) {
		if (field[x - 1][y].letter != '0') {
			return true;
		}
	}
	if ((y != SIZE)) {
		if (field[x][y + 1].letter != '0') {
			return true;
		}
	}
	if ((y != 0)) {
		if (field[x][y - 1].letter != '0') {
			return true;
		}
	}
	return false;
}

void PlaceALetter(char ch, cell** field, int x, int y) {
	field[x][y].letter = ch;
	field[x][y].isAvailible = 0;
	if ((x != SIZE)) {
		if (field[x + 1][y].letter == '0') {
			field[x + 1][y].isAvailible = 1;
		}
	}
	if ((x != 0)) {
		if (field[x - 1][y].letter == '0') {
			field[x - 1][y].isAvailible = 1;
		}
	}
	if ((y != SIZE)) {
		if (field[x][y + 1].letter == '0') {
			field[x][y + 1].isAvailible = 1;
		}
	}
	if ((y != 0)) {
		if (field[x][y - 1].letter == '0') {
			field[x][y - 1].isAvailible = 1;
		}
	}
}

void UndoLetter(cell** field, int x, int y) {
	field[x][y].letter = '0';
	field[x][y].isAvailible = 1;
	if ((x != SIZE)) {
		if ((field[x + 1][y].letter == '0') && (!ALetterIsConnected(field, x + 1, y))) {
			field[x + 1][y].isAvailible = 0;
		}
	}
	if ((x != 0)) {
		if ((field[x - 1][y].letter == '0') && (!ALetterIsConnected(field, x - 1, y))) {
			field[x - 1][y].isAvailible = 0;
		}
	}
	if ((y != SIZE)) {
		if ((field[x][y + 1].letter == '0') && (!ALetterIsConnected(field, x, y + 1))) {
			field[x][y + 1].isAvailible = 0;
		}
	}
	if ((y != 0)) {
		if ((field[x][y - 1].letter == '0') && (!ALetterIsConnected(field, x, y - 1))) {
			field[x][y - 1].isAvailible = 0;
		}
	}
}

bool PlaceAWord(int l, string word, cell** field, int &x , int &y) {
	int temp = -1;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			temp = IsAWordPlacable(l, word, field, i, j);
			if ((temp==0)||(temp==l-1)) {
				PlaceALetter(word[temp], field, i, j);
				x = i;
				y = j;
				return true;
			}
		}
	}
	return false;
}

int countOnes(int* mas, int size) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (mas[i] == 1) { count++; }
	}
	return count;
}

void GetTheMostPoints(stack<string> &endmoves, stack<string> &moves, int n, string* words, int* covered, cell** field, int &Points, int &MPoints, int whatword) {
	if (whatword >= n) {
		return;
	}
	if (covered[whatword] == 1) {
		return;
	}
	int x = -1, y = -1;
	bool AWordIsPlaced = PlaceAWord(words[whatword].size(), words[whatword], field, x, y);
	moves.push(words[whatword]);
	covered[whatword] = 1;
	if (AWordIsPlaced) {
		if (countOnes(covered, n)%2==1) {
			Points += words[whatword].size();
		}
		for (int i = whatword+1; i < n; i++) {
			GetTheMostPoints(endmoves, moves, n, words, covered, field, Points, MPoints, i);
		}
		if (MPoints < Points) { MPoints = Points; endmoves = moves; }
		Points = Points - words[whatword].size();
		UndoLetter(field, x, y);
		return;
	}
	moves.pop();
}

int main(){
	setlocale(LC_ALL, "rus");
	ifstream fin("Текст.txt");
	cell** FIELD = new cell * [SIZE];
	for (int i = 0; i < SIZE; i++) {
		FIELD[i] = new cell[SIZE];
	}
	string StartWord;
	int length;
	int howmuchwords;
	fin >> StartWord >> length >> howmuchwords;
	string* words = new string[howmuchwords];
	for(int i =0;i<howmuchwords;i++){
		string nextword;
		fin >> nextword;
		words[i] = nextword;
	}


	cout << howmuchwords << " words of the lenght of " << length << ":" << endl;
	GetWords(words, howmuchwords);
	FillTheField(FIELD, StartWord);
	GetField(FIELD);
	int x = 1, y = 1;

	int* cov = new int[howmuchwords];
	int pts = 0, mpts = 0;
	stack<string> em, m;
	GetTheMostPoints(em,m,howmuchwords, words, cov, FIELD, pts, mpts, 0);
	cout << mpts << endl;
	while (!em.empty()) {
		cout << em.top() << endl; em.pop();
	}
	GetField(FIELD);

	for (int i = 0; i < SIZE; i++) {
		delete[](FIELD[i]);
	}
	delete[](FIELD);
	delete[](words);
	delete[](cov);
	system("pause"); 
	return 0;
}