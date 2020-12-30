#include "parser.h"

Parser::Parser()
{
	stateStack.push_back('$');
	stateStack.push_back('E');
}
Parser::Parser(vector<string> G, string& seperator)
{
	init(G, seperator);
	stateStack.push_back('$');
	stateStack.push_back('E');
}

Parser::~Parser()
{
}

//获取nontem所在位置
int Parser::getVectorIndex(char nontem, vector<char> str)
{
	for (int i=0;i<str.size();i++)
	{
		if (nontem==str[i])
		{
			return i;
		}
	}
	return -1;
}



void Parser::setFirst()
{
	bool isEnd = false;
	vector<char> firstBefore[MAXNT];
	while (!isEnd)
	{
		for (auto i : productions)
		{
			setFirst(i[0]);
		}
		isEnd = true;
		for (int i = 0; i < MAXNT; i++)
		{
			if (firstBefore[i].size() != first[i].size())
			{
				firstBefore[i] = first[i];
				isEnd = false;
			}
		}
	}
}
void Parser::setFirst(char nontem)
{
	int index = getVectorIndex(nontem, NT);
	if (index==-1)
	{
		return;
	}
	for (int i = 0;i< productions.size();i++)
	{
		if (nontem == productions[i][0]) {
			if (find(productions[i][1],NT))
			{
				first[index].push_back(productions[i][1]);
				clearVector(first[index]);
			}
			else
			{
				setFirst(productions[i][1]);
				int index1 = getVectorIndex(productions[i][1], NT);
				first[index].insert(first[index].end(), first[index1].begin(), first[index1].end());
				clearVector(first[index]);
			}
		}
	}
}

void Parser::setFollow()
{
	//最开始判断哪个非终结符可以推导出空串
	for (auto i : productions)
	{
		if (i[1]=='@')
		{
			NULLNT.push_back(i[0]);
		}
	}
	follow[0].push_back('$');
	bool isEnd = false;
	vector<char> followBefore[MAXNT];
	while (!isEnd)
	{
		for (auto i : productions)
		{
			setFollow(i);
		}
		isEnd = true;
		for (int i = 0;i<MAXNT;i++)
		{
			if (followBefore[i].size() != follow[i].size())
			{
				followBefore[i] = follow[i];
				isEnd = false;
			}
		}
	}

	//剔除follow集中的空串元素
	for (int i = 0;i<MAXNT;i++)
	{
		vector<char>::iterator it;
		for (it = follow[i].begin(); it != follow[i].end();)
		{
			if (*it=='@')
			{
				it = follow[i].erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}


void Parser::setFollow(vector<char> production)
{
	int indexFather = getVectorIndex(production[0], NT);
	if (indexFather == -1)
	{
		//不是非终结符
		return;
	}
	if (production[production.size() - 1]=='@')
	{
		return;
	}
	for (int i = production.size()-1;i>0;i--)
	{
		int indexThis;
		int indexFollow;
		//判断推出式是否为非终结符
		if ((indexThis=getVectorIndex(production[i],NT))!=-1)
		{
			//如果是推出式的最后一位
			if (i == production.size() - 1) {
				follow[indexThis].insert(follow[indexThis].end(), follow[indexFather].begin(), follow[indexFather].end());
				clearVector(follow[indexThis]);
			}
			//否则如果他的后一位是非终结符，则加上后一位的first集
			else if ((indexFollow = getVectorIndex(production[i+1], NT)) != -1)
			{
				follow[indexThis].insert(follow[indexThis].end(), first[indexFollow].begin(), first[indexFollow].end());
				clearVector(follow[indexThis]);
				//如果后一位可以推导出空串，则加上后一位的follow集
				if (!find(production[i + 1], NULLNT)) {
					follow[indexThis].insert(follow[indexThis].end(), follow[indexFollow].begin(), follow[indexFollow].end());
					clearVector(follow[indexThis]);
				}
			}
			//否则认为后一位为非空串的终结符
			else
			{
				follow[indexThis].push_back(production[i + 1]);
				clearVector(follow[indexThis]);
			}
		}
	}
}

void Parser::setSelect()
{
	//初始化select集
	select = productions;
	for (int i = 0; i < select.size(); i++)
	{
		select[i].clear();
	}
	//计算select集
	for (int i = 0; i < select.size(); i++)
	{
		int indexFather = getVectorIndex(productions[i][0], NT);
		int index = getVectorIndex(productions[i][1],NT);
		if (index!=-1)
		{
			select[i].insert(select[i].end(), first[index].begin(), first[index].end());
		}
		else if (productions[i][1]=='@')
		{
			select[i].insert(select[i].end(), follow[indexFather].begin(), follow[indexFather].end());
		}
		else
		{
			select[i].push_back(productions[i][1]);
		}
	}
}

vector<vector<char>> Parser::getSelect()
{
	return this->select;
}

void Parser::setAnalyzeTable()
{
	for (int i  = 0;i< select.size();i++)
	{
		for (int j = 0;j<select[i].size();j++)
		{
			string index;
			vector<char> content;
			index += productions[i][0];
			index += select[i][j];
			content = productions[i];
			analyzeTable[index] = content;
		}
	}
}

vector<char> Parser::selectTable(string index)
{
	return analyzeTable[index];
}


void Parser::init(vector<string> G, string& seperator)
{
	int length = G.size();
	for (int i = 0; i < length; i++)
	{
		vector<string> res = split(G[i], seperator);
		vector<char> buffer;
		for (int j = 0; j < res.size(); j++)
		{
			for (int k = 0; k < res.at(j).size(); k++)
			{
				buffer.push_back(res.at(j).at(k));
			}
		}
		productions.push_back(buffer);

	}
	setSymbol();
	setFirst();
	setFollow();
	setSelect();
	setAnalyzeTable();
}

bool Parser::startJudge(vector<Word*> words)
{
	this->tokenStack = words;
	tokenStack.push_back(new Word(tokenStack[0]->line, "$", 7, '$'));
	int err = 0;
	while (tokenStack.size() > 0)
	{
		Word* curToken = tokenStack[0];
		char curState = stateStack[stateStack.size() - 1];
		string index;
		index += curState;
		index += curToken->state;
		vector<char> analyze = analyzeTable[index];
		if (analyze.empty()) {
			if (curToken->token != "$")
			{
				cout << "error: line:" << curToken->line << " near " << curToken->token << endl;
				for (int i = 0; i < words.size(); i++)
				{
					if (i == err)
					{
						cout << "\033[31m" << words[i]->token << "\033[37m";
					}
					else
					{
						cout << words[i]->token;
					}
				}
				cout << endl;
			}
			else
			{
				cout << "error: line:" << curToken->line << ". The end seems to be missing something." << endl;
			}
			return false;
		}
		else
		{
			stateStack.pop_back();
			for (vector<char>::iterator it = analyze.end()-1;it!=analyze.begin();it--)
			{
				if (*it!='@')
				{
					stateStack.push_back(*it);
				}
			}
			if (stateStack[stateStack.size()-1] == tokenStack[0]->state)
			{
				popFirstToken();
				stateStack.pop_back();
				err++;
			}
		}
	}
	if (tokenStack.size() == 0 && stateStack.size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

string Parser::stateStack2String()
{
	string str;
	for (int i = 0; i < this->stateStack.size(); i++)
		str += this->stateStack[i];
	return str;
}

string Parser::tokenStack2String() {
	string str;
	for (int i = 0; i < this->tokenStack.size();i++)
	{
		str += this->tokenStack[i]->token;
	}
	return str;
}

int Parser::getAnswer(vector<Word*> words)
{
	string str;
	for (int i = 0; i < words.size(); i++)
	{
		str += words[i]->token;
	}
	return eval(str);
}



void Parser::popFirstToken()
{
	vector<Word*>::iterator it = tokenStack.begin();
	tokenStack.erase(it);
}


void Parser::clearT(vector<char> &vch)
{
	for (vector<char>::iterator it = vch.begin(); it != vch.end();)
	{
		if (getVectorIndex(*it,T)!=-1)
		{
			vch.erase(it);
		}
		else
		{
			it++;
		}
	}
}

//判断ch是否在str中,不在则返回true
bool Parser::find(char ch,vector<char> str)
{
	for (auto a:str)
	{
		if (ch == a) {
			return false;
		}
	}
	return true;
}

void Parser::clearVector(vector<char> &v)
{
	set<char>s(v.begin(), v.end());
	v.assign(s.begin(), s.end());
}



vector<string> Parser::split(string& str, string& seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != str.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != str.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (str[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != str.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (str[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(str.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

void Parser::setSymbol()
{
	for (auto i : productions)
	{
		NT.push_back(i[0]);
	}
	clearVector(NT);
	for (auto i : productions)
	{
		for (int j = 1; j < i.size(); j++) {
			if (find(i[j],NT)) {
				T.push_back(i[j]);
			}
		}
	}
	clearVector(T);
}

