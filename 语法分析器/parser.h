#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include "expr.h"
#include "�ʷ�������.h"

#define MAXT 5	//�ս���������
#define MAXNT 5	//���ս���������

/*
E��TE��
E�� ��+TE��| ��
T��FT��
T�� ��*FT��| ��
F����E��| I
*/

using namespace std;


class Parser
{
public:
	Parser();
	Parser(vector<string> G, string & seperator);
	~Parser();
	//����select��
	vector<vector<char>> getSelect();
	//��ѯԤ�������
	vector<char> selectTable(string index);
	//��ʼ������
	void init(vector<string> G, string& seperator);
	//��ʼ�﷨����
	bool startJudge(vector<Word*> words);
	//vectorתstring
	string stateStack2String();
	string tokenStack2String();
	//�����
	int getAnswer(vector<Word*> words);
private:
	//��ջ
	vector<char> stateStack;
	vector<Word*> tokenStack;
	//ɾ��tokenStack�еĵ�һ��Ԫ��
	void popFirstToken();
	//����ʽ
	vector<vector<char>> productions;
	//���ս��
	vector<char> NT;
	//�ս��
	vector<char> T;
	//�����first��
	void setFirst(char nontem);
	//����follow��
	void setFollow(vector<char> production);
	//����select��
	void setSelect(vector<char> production);
	//�ָ��ַ���
	vector<string> split(string& str, string& seperator);
	//����Ԥ�������
	void setAnalyzeTable();
	//������ս��
	void setSymbol();
	//�����first��
	void setFirst();
	//����follow��
	void setFollow();
	//����select��
	void setSelect();
	//��ȡ�ַ��������е�λ��
	int getVectorIndex(char nontem, vector<char> str);
	//first��
	vector<char> first[MAXNT];
	//follow��
	vector<char> follow[MAXNT];
	//select��
	vector<vector<char>> select;
	//����vector���ս��
	void clearT(vector<char> &vch);
	//Ԥ�������
	map<string, vector<char>> analyzeTable;
	//�����Ƶ����մ��ķ��ս������
	vector<char> NULLNT;
	//����Ԫ���Ƿ���vector��
	bool find(char ch, vector<char> str);
	//����vector����ͬ��Ԫ��
	void clearVector(vector<char> &v);
};