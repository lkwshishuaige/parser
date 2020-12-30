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
	word(int line, string token, int type, char state) : line(line), token(token), type(type), state(state) {}//�вι���
}Word;

class JudgeWord
{
public:

	//��ȡ��ǰ��ȡ����
	int getLine() {
		return this->line;
	}
	//��ʼ����
	vector<Word*> getToken(string fileName);

	//�ļ��л�ȡ����
	char* getWord(FILE *fpin);

	//�жϵ�����ֹ
	bool isBreak(char ch);

	//�ʷ�������
	int scan(char *word);

	//�����ж�
	int judgeNum(char* word);

	//ʮ�������ж�
	bool judgeHex(char* word);

	//�˽����ж�
	bool judgeOct(char* word);
	//ʮ�����ж�
	bool judgeDec(char* word);

	//�ؼ����ж�
	bool judgeKey(char* word);

	//��ʶ���ж�
	bool judgeIdentifier(char* word);
	//ʮ�����ƻ�ȡ�Ӵ�
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
