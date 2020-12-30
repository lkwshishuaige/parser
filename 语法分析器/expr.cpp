#include "expr.h"

map<string, int> op_mapping;    // ��������ȼ�ӳ���
string ops = "+-*/()";                  // ����������������ַ�����Ϊ��ͨ��find���������ж�Ŀ���ַ��Ƿ�Ϊ������
bool ValueError = false;               // �Ƿ�����ֵ����ı��
bool ExpressionError = false;      // �Ƿ������ʽ����ı��


double calculatePostfix(vector<string> &postfix)
{
	vector<double> result;
	for (int i = 0; i < postfix.size(); ++i)
	{
		if (ops.find(postfix[i]) == ops.npos)        // ɨ�赽��������ֱ��ѹ��ջ��
			result.push_back(toDouble(postfix[i]));
		else               // ɨ�赽������
		{
			// ���ʣ��Ԫ�ص�����С��2������ʽ�Ƿ�
			if (result.size() < 2)
			{
				ExpressionError = true;
				return 0.0;
			}
			double num1 = result.back();
			result.pop_back();
			double num2 = result.back();
			result.pop_back();
			double op_res;
			// �������ۣ�����num2 op num1
			if (postfix[i] == "+")
				op_res = num2 + num1;
			else if (postfix[i] == "-")
				op_res = num2 - num1;
			else if (postfix[i] == "*")
				op_res = num2 * num1;
			else if (postfix[i] == "/")
			{
				// �˴���Ҫ�ж�һ���Ƿ��ĸΪ0
				if (num1 == 0)
				{
					ValueError = true;
					return 0.0;
				}

				op_res = num2 / num1;
			}
			// �����Ľ������ѹ��ջ��
			result.push_back(op_res);
		}
	}
	if (result.size() == 1)     // ����ջ��Ԫ�أ�����ǺϷ��ı��ʽ�����ʱջ��ֻ��һ��Ԫ�أ�
		return result.back();
	else            // ���Ϸ��ı��ʽ�ᵼ�½���ʱ��result���в�ֹһ��Ԫ��
	{
		ExpressionError = true;
		return 0.0;
	}
}

double eval(string &infix)
{
	init_mapping(op_mapping);
	vector<string> postfix = toPostfix(infix);
	return calculatePostfix(postfix);
}

void init_mapping(map<string, int> &mapping)
{
	mapping["+"] = 0;
	mapping["-"] = 0;
	mapping["*"] = 1;
	mapping["/"] = 1;
	mapping["("] = 2;
	mapping[")"] = 2;
}

double toDouble(string str)
{
	double target;
	stringstream ss;
	ss << str;
	ss >> target;
	return target;
}

vector<string> toPostfix(string formula)
{
	vector<string> result;
	vector<string> op_stack;
	string cur_num, cur_op;

	for (int i = 0; i < formula.size(); ++i)
	{
		if (ops.find(formula[i]) == ops.npos)   // ɨ�赽���ǲ�����
			cur_num += formula[i];

		else        // ɨ�赽���ǲ��������ֽ��ۼӵĲ������ַ�������
		{
			if (!cur_num.empty())
			{
				result.push_back(cur_num);
				cur_num.clear();
			}

			cur_op = formula[i];

			if (op_stack.empty())           // ջΪ�գ�ֱ����ջ
				op_stack.push_back(cur_op);
			else if (cur_op == "(")         // ��ǰ������Ϊ�����ţ�ֱ����ջ
				op_stack.push_back(cur_op);
			else if (cur_op == ")")         // ��ǰ������Ϊ�����ţ�����Ҫ��op_stack��ֱ��������ǰ�����е�Ԫ�ص���
			{
				while (op_stack.back() != "(")
				{
					result.push_back(op_stack.back());
					op_stack.pop_back();

					if (op_stack.empty())           // ���Ϸ��ı��ʽ��������������
					{
						ExpressionError = true;
						result.push_back("0");
						return result;
					}
				}
				op_stack.pop_back();        // �������ŵ���
			}
			else if (op_stack.back() == "(")          // �ڵ�ǰ�������������ŵ�����£����ջ��Ԫ��Ϊ�����ţ���ֱ����ջ
				op_stack.push_back(cur_op);
			else if (op_mapping[cur_op] > op_mapping[op_stack.back()])        // �ڵ�ǰ��������ջ��Ԫ��Ϊ+-*/������£�����ǰ���������ȼ�����ջ��Ԫ�أ�ֱ����ջ
				op_stack.push_back(cur_op);
			else            // ���һ��������ǵ�ǰ�����������ȼ����ڻ����ջ��Ԫ�����ȼ�ʱ
			{
				while ((op_stack.back() != "(") && (op_mapping[op_stack.back()] >= op_mapping[cur_op]))
				{
					result.push_back(op_stack.back());
					op_stack.pop_back();
					// ��ջ�ѿգ���ֱ�ӷ���
					if (op_stack.empty())
						break;
				}
				op_stack.push_back(cur_op);     // ����Ҫ��Ĳ����������󣬵�ǰ��������ջ
			}
		}
	}

	result.push_back(cur_num);

	// ���op_stack���ܻ�����ʣ��Ԫ�أ�ȫ������
	while (!op_stack.empty())
	{
		result.push_back(op_stack.back());
		op_stack.pop_back();
	}

	return result;
}
