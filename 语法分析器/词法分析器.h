#pragma once
#pragma warning(disable  : 4996)
#include<iostream>
#include<string.h>
#include<vector>
using namespace std;

#define DAT 0
#define DEC 1
#define OCT 2
#define HEX 3
#define SYM 4
#define DEL 5
#define KEY 6
#define TEM 7


typedef struct word
{
	int line;
	string token;
	int type;
	char state;
	word(int line, string token, int type) : line(line), token(token), type(type) {}
	word(int line, string token, int type, char state) : line(line), token(token), type(type), state(state) {}//有参构造
}Word;

class JudgeWord
{
public:

	//获取当前读取行数
	int getLine() {
		return this->line;
	}
	//开始函数
	vector<Word*> getToken(string fileName);

	//文件中获取单词
	char* getWord(FILE *fpin);

	//判断单词终止
	bool isBreak(char ch);

	//词法分析器
	int scan(char *word);

	//数字判断
	int judgeNum(char* word);

	//十六进制判断
	bool judgeHex(char* word);

	//八进制判断
	bool judgeOct(char* word);
	//十进制判断
	bool judgeDec(char* word);

	//关键字判断
	bool judgeKey(char* word);

	//标识符判断
	bool judgeIdentifier(char* word);
	//十六进制获取子串
	void substr(char* str1, char* str2, bool flag);
	JudgeWord();
	~JudgeWord();

private:
	int line;
	vector<Word*> words;
	char ch = ' ';
	char *keyWord[5] = { (char *)"if",(char *)"then",(char *)"else",(char *)"while",(char *)"do" };
	char token[30];
};
