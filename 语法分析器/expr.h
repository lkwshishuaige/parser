#pragma once

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;



// 将string转换成double
double toDouble(string str);

// 初始化操作符优先级映射表
void init_mapping(map<string, int> &mapping);

double eval(string &infix);

double calculatePostfix(vector<string> &postfix);

// 将中缀表达式转化为后缀表达式
vector<string> toPostfix(string formula);
