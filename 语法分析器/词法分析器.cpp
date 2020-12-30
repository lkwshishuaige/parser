#include "词法分析器.h"


JudgeWord::JudgeWord()
{
	this->line = 1;
}

JudgeWord::~JudgeWord()
{
}

vector<Word*> JudgeWord::getToken(string fileName) {
	FILE *fpin;
	fpin = fopen(fileName.c_str(), "r");
	char* token;
	while (fgetc(fpin) != EOF) {
		fseek(fpin, -1L, SEEK_CUR);
		if ((token = getWord(fpin)) != NULL) {
			char* buffer = (char*)malloc(sizeof(char)*strlen(token));
			int types = scan(token);
			switch (types)
			{
			case 0:	//标识符
				words.push_back(new Word(getLine(), token, DAT,'i'));
				break;
			case 1:	//十进制
				words.push_back(new Word(getLine(), token, DEC, 'i'));
				break;
			case 2:	//八进制
				substr(token, buffer, false);
				words.push_back(new Word(getLine(), buffer, OCT, 'i'));
				break;
			case 3:	//十六进制
				substr(token, buffer, true);
				words.push_back(new Word(getLine(), buffer, HEX, 'i'));
				break;
			case 4:	//算术符
				words.push_back(new Word(getLine(), token, SYM, token[0]));
				break;
			case 5:	//界符
				words.push_back(new Word(getLine(), token, DEL, token[0]));
				break;
			case 6:	//关键字
				words.push_back(new Word(getLine(), token, KEY));
				break;
			default:
				vector<Word*> err;
				err.push_back(new Word(getLine(), token, 7));
				return err;
			}
		}
	}
	fclose(fpin);
	return words;
}

//文件中获取单词
char* JudgeWord::getWord(FILE *fpin) {
	char *token = (char*)malloc(20 * sizeof(char));
	char ch;
	int i = 0;
	if (!isBreak(ch = fgetc(fpin))) {
		fseek(fpin, -1L, SEEK_CUR);
		while ((ch = fgetc(fpin)) != EOF) {
			if (isBreak(ch)) {
				fseek(fpin, -1L, SEEK_CUR);
				break;
			}
			*(token + i) = ch;
			i++;
		}
		*(token + i) = '\0';
		return token;
	}
	else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
		if (ch == '\n')
		{
			line++;
		}
		return NULL;
	}
	else {
		*(token + i) = ch;
		*(token + i + 1) = '\0';
		return token;
	}
}

//判断单词终止
bool JudgeWord::isBreak(char ch) {
	{
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == '=' || ch == '\n' || ch == ';' || ch == ' ' || ch == '\t' || ch == '\r')
			return true;
		else
			return false;
	}
}

//词法分析器
int JudgeWord::scan(char *word) {
	int types = -1;
	if (*word >= '0'&&*word <= '9') {
		int sign = judgeNum(word);
		if (sign == 0)
			types = 1;
		else if (sign == 1)
			types = 2;
		else if (sign == 2)
			types = 3;
		else if (sign == -1)
			types = -1;
	}
	else if ((*(word) >= 'a'&&*(word) <= 'z') || (*(word) >= 'A' && *(word) <= 'Z')) {
		if (judgeKey(word))
			types = 6;
		else if (judgeIdentifier(word))
			types = 0;
	}
	else {
		if (!strcmp(word, "+") || !strcmp(word, "-") || !strcmp(word, "*") || !strcmp(word, "/") || !strcmp(word, "="))
			types = 4;
		else if (!strcmp(word, ">") || !strcmp(word, "<") || !strcmp(word, "(") || !strcmp(word, ")") || !strcmp(word, ";"))
			types = 5;
	}
	return types;
}

//数字判断
int JudgeWord::judgeNum(char* word) {
	int sign = -1;
	if (*(word) == '0'&&*(word + 1) != '\0') {
		if (*(word + 1) == 'x') {
			if (judgeHex(word))
				sign = 2;
		}
		else {
			if (judgeOct(word))
				sign = 1;
		}
	}
	else {
		if (judgeDec(word))
			sign = 0;
	}
	return sign;
}

//十六进制判断
bool JudgeWord::judgeHex(char* word) {
	bool isTrue = true;
	int i = 2;
	while (*(word + i) != '\0') {
		if (*(word + i) < '0' || *(word + i) > '9') {
			isTrue = false;
			if (*(word + i) == 'f' && *(word + i + 1) == '\0') {
				isTrue = true;
				break;
			}
			else
				break;
		}
		i++;
	}
	return isTrue;
}

//八进制判断
bool JudgeWord::judgeOct(char* word) {
	bool isTrue = true;
	int i = 1;
	while (*(word + i) != '\0') {
		if (*(word + i) < '0' || *(word + i) > '9') {
			isTrue = false;
			break;
		}
		i++;
	}
	return isTrue;
}

//十进制判断
bool JudgeWord::judgeDec(char* word) {
	bool isTrue = true;
	int i = 1;
	while (*(word + i) != '\0') {
		if (*(word + i) < '0' || *(word + i) > '9') {
			isTrue = false;
			break;
		}
		i++;
	}
	return isTrue;
}

//关键字判断
bool JudgeWord::judgeKey(char* word) {
	bool isTrue = false;
	for (int i = 0; i < 5; i++) {
		if (strcmp(word, keyWord[i]) == 0) {
			isTrue = true;
			break;
		}
	}
	return isTrue;
}

//标识符判断
bool JudgeWord::judgeIdentifier(char* word) {
	bool isTrue = true;
	int i = 1;
	while (*(word + i) != '\0') {
		if (((*(word + i) < 'a'&&*(word + i) > 'Z') || (*(word + i) < 'A'&&*(word + i) > '9') || *(word + i) < '0' || *(word + i) > 'z') && *(word + i) != '_') {
			isTrue = false;
			break;
		}
		i++;
	}
	return isTrue;
}
//十六进制获取子串
void JudgeWord::substr(char* str1, char* str2, bool flag) {
	if (flag) {
		int j = 0;
		for (int i = 2; str1[i] != '\0'; i++, j++) {
			*(str2 + j) = *(str1 + i);
		}
		str2[j] = '\0';
	}
	else {
		int j = 0;
		for (int i = 1; str1[i] != '\0'; i++, j++) {
			*(str2 + j) = *(str1 + i);
		}
		str2[j] = '\0';
	}
}