#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include "expr.h"
#include "词法分析器.h"

#define MAXT 5	//终结符最大数量
#define MAXNT 5	//非终结符最大数量

/*
E→TE’
E’ →+TE’| ε
T→FT’
T’ →*FT’| ε
F→（E）| I
*/

using namespace std;


class Parser
{
public:
	Parser();
	Parser(vector<string> G, string & seperator);
	~Parser();
	//返回select集
	vector<vector<char>> getSelect();
	//查询预测分析表
	vector<char> selectTable(string index);
	//初始化函数
	void init(vector<string> G, string& seperator);
	//开始语法分析
	bool startJudge(vector<Word*> words);
	//vector转string
	string stateStack2String();
	string tokenStack2String();
	//计算答案
	int getAnswer(vector<Word*> words);
private:
	//堆栈
	vector<char> stateStack;
	vector<Word*> tokenStack;
	//删除tokenStack中的第一个元素
	void popFirstToken();
	//产生式
	vector<vector<char>> productions;
	//非终结符
	vector<char> NT;
	//终结符
	vector<char> T;
	//计算出first集
	void setFirst(char nontem);
	//计算follow集
	void setFollow(vector<char> production);
	//计算select集
	void setSelect(vector<char> production);
	//分割字符串
	vector<string> split(string& str, string& seperator);
	//计算预测分析表
	void setAnalyzeTable();
	//计算出终结符
	void setSymbol();
	//计算出first集
	void setFirst();
	//计算follow集
	void setFollow();
	//计算select集
	void setSelect();
	//获取字符在容器中的位置
	int getVectorIndex(char nontem, vector<char> str);
	//first集
	vector<char> first[MAXNT];
	//follow集
	vector<char> follow[MAXNT];
	//select集
	vector<vector<char>> select;
	//清理vector中终结符
	void clearT(vector<char> &vch);
	//预测分析表
	map<string, vector<char>> analyzeTable;
	//可以推导出空串的非终结符集合
	vector<char> NULLNT;
	//查找元素是否在vector中
	bool find(char ch, vector<char> str);
	//消除vector中相同的元素
	void clearVector(vector<char> &v);
};