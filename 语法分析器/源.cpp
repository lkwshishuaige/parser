#define _CRT_SECURE_NO_WARNINGS 

#include<iostream>
#include<stack>
#include"词法分析器.h"
#include "parser.h"

using namespace std;
/*
	消除左递归之后的文法：
	E→TE’
	E’ →+TE’| ε
	T→FT’
	T’ →*FT’| ε
	F→（E）| i
*/

/*
		first	follow
	E	( i		) $
	E'	+ @		) $
	T	( i		+ $ )
	T'	* @		+ $ )
	F	( i		* $ +
*/

vector<string> G=	//存文法
{
	{"E->Te"},	//( i
	{"e->+Te"},	//+
	{"e->@"},	//$ )
	{"T->Ft"},	//( i
	{"t->*Ft"},	//*
	{"t->@"},	//$ ) +
	{"F->(E)"},	//(
	{"F->i"}	//i
};

/*
1+2+3*4
*/

char first[5][3] = {
	"(i","+@","(i","*@","(i"
};
char follow[5][5] = {
	")$",")$","+$)","+$)","+$*)"
};


int main() {
	vector<Word*> words;
	JudgeWord judgeWord = JudgeWord();
	words=judgeWord.getToken("program.txt");
	if (words.size()==1&&words.at(0)->type==7)
	{
		cout << "行" << judgeWord.getLine()<<"出现错误："<< words.at(0)->token.c_str() << endl;
		exit(0);
	}
	vector<Word*> curWord;
	for (int i = 0;i<words.size();i++)
	{
		curWord.push_back(words[i]);
		if (words[i]->token==";")
		{
			curWord.pop_back();
			for (int i = 0; i < curWord.size(); i++)
			{
				cout << curWord[i]->token;
			}
			cout << endl;
			string seperator = "->";
			Parser parser = Parser();
			parser.init(G, seperator);
			if (parser.startJudge(curWord))
			{
				cout << "correct" << endl;
				double answer = parser.getAnswer(curWord);
				cout << "The answer is " << answer << endl;
			}
			curWord.clear();
			cout << endl;
		}
	}
}