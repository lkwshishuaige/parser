#pragma once

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;



// ��stringת����double
double toDouble(string str);

// ��ʼ�����������ȼ�ӳ���
void init_mapping(map<string, int> &mapping);

double eval(string &infix);

double calculatePostfix(vector<string> &postfix);

// ����׺���ʽת��Ϊ��׺���ʽ
vector<string> toPostfix(string formula);
