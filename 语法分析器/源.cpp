#define _CRT_SECURE_NO_WARNINGS 

#include<iostream>
#include<stack>
#include"�ʷ�������.h"
#include "parser.h"

using namespace std;
/*
	������ݹ�֮����ķ���
	E��TE��
	E�� ��+TE��| ��
	T��FT��
	T�� ��*FT��| ��
	F����E��| i
*/

/*
		first	follow
	E	( i		) $
	E'	+ @		) $
	T	( i		+ $ )
	T'	* @		+ $ )
	F	( i		* $ +
*/

vector<string> G=	//���ķ�
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
		cout << "��" << judgeWord.getLine()<<"���ִ���"<< words.at(0)->token.c_str() << endl;
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