#pragma once
#include"DFA.h"
string regex;//输入正则表达式
int  node_count=0;//所有NFA中的节点计数，为了防止有重复节点，给每生成的一个NFA节点就+1.
string former_ch;
fstream file_name;
string line;
string id;
string number;
map<string, int> map_count;
int line_number = 0;//词法分析报错使用。
//正则到NFA相关
//工具函数，是否添加连接符？
bool is_insert(string s1, string s2)
{
	if (s1 >= "0"&&s1 <= "z" &&s2 >= "0"&&s2 <= "z")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
string getch()//读取正则，返回正则表达式的一个字符。
{
	cout << regex << endl;
	if (regex.size() != 0)
	{
		string ch = regex.substr(0, 1);

		//修改正则表达式的代码
		if (ch == "("&&former_ch == ")")
		{
			former_ch = ".";
			return ".";
		}
		else if (is_insert(ch, former_ch))
		{
			former_ch = ".";
			return ".";
		}
		former_ch = ch;
		regex.erase(0, 1);
		return ch;
	}
	else
	{
		cout << "正则表达式解析完毕" << endl;
		return "";
	}
}
NFA creat_basic_nfa(string a)
{
	NFA  nfa;
	//输入字符。
	nfa.input_char.push_back(a);
	//开始字符
	nfa.start = to_string(node_count);
	node_count++;

	//构造结束字符
	nfa.end = to_string(node_count);
	//构造转换函数。
	map<string, vector<string>> w;
	w[a] = { to_string(node_count) };
	node_count++;
	nfa.transform_f[nfa.start] = w;

	return nfa;
}
NFA or_nfa(NFA s1, NFA s2)
{
	NFA  nfa;
	//输入字符
	for (int i = 0; i < s1.input_char.size(); i++)
	{
		nfa.input_char.push_back(s1.input_char[i]);
	}
	for (int i = 0; i < s2.input_char.size(); i++)
	{
		nfa.input_char.push_back(s2.input_char[i]);
	}
	//初始状态
	nfa.start = to_string(node_count);
	node_count++;
	//结束状态
	nfa.end = to_string(node_count);
	node_count++;
	//构造转换函数
	//第一步
	map<string, vector<string>> w;
	w["kkk"] = { s1.start,s2.start };
	nfa.transform_f[nfa.start] = w;
	w.clear();
	//第二步
	for (auto w = s1.transform_f.begin(); w != s1.transform_f.end(); w++)
	{
		nfa.transform_f[w->first] = w->second;
	}
	for (auto w = s2.transform_f.begin(); w != s2.transform_f.end(); w++)
	{
		nfa.transform_f[w->first] = w->second;
	}
	//第三步
	w["kkk"] = { nfa.end };
	nfa.transform_f[s1.end] = w;
	nfa.transform_f[s2.end] = w;
	return nfa;
}
NFA connect_nfa(NFA s1, NFA s2)//正则表达式的连接
{
	//构造转换函数
	for (int i = 0; i < s2.input_char.size(); i++)
	{
		s1.input_char.push_back(s2.input_char[i]);
	}
	//第一步
	map<string, vector<string>> w;
	w["kkk"] = { s2.start };
	s1.transform_f[s1.end] = w;
	w.clear();
	//第二步
	for (auto w = s2.transform_f.begin(); w != s2.transform_f.end(); w++)
	{
		s1.transform_f[w->first] = w->second;
	}
	//第三步
	s1.end = s2.end;

	return s1;
}
NFA  closure_nfa(NFA s1)//正则表达式的闭包。
{
	NFA  nfa;
	//输入字符
	nfa.input_char = s1.input_char;
	//初始状态
	nfa.start = to_string(node_count);
	node_count++;
	//结束状态
	nfa.end = to_string(node_count);
	node_count++;
	//第一步
	map<string, vector<string>> w;
	w["kkk"] = { s1.start,nfa.end };
	nfa.transform_f[nfa.start] = w;
	w.clear();
	//第二步
	w["kkk"] = { s1.start ,nfa.end };
	nfa.transform_f[s1.end] = w;
	w.clear();
	for (auto w = s1.transform_f.begin(); w != s1.transform_f.end(); w++)
	{
		nfa.transform_f[w->first] = w->second;
	}
	return nfa;
}
//正则转换为NFA
NFA regex_to_nfa()
{
	string ch = getch();
	stack<NFA> node_stack;//节点栈
	stack<string> operator_stack;//运算符号栈
	while (ch != "")
	{
		switch (*ch.c_str())
		{

		case '('://左括号运算符
		{
			if (regex == "")
			{
				NFA nfa = creat_basic_nfa(ch);
				node_stack.push(nfa);
				break;
			}
			operator_stack.push("(");
			break;
		}
		case ')'://右括号运算符
		{
			if (regex == "")
			{
				NFA nfa = creat_basic_nfa(ch);
				node_stack.push(nfa);
				break;
			}
			string current_operator = operator_stack.top();
			operator_stack.pop();
			if (current_operator == "(")
			{
				if (node_stack.size() == 0)//"()"情况
				{
					cout << "()内没有操作数" << endl;
				}
				else//(a)的这种情况
				{
					break;
				}

			}
			while (current_operator != "(")//直到栈顶元素不是“（”
			{
				switch (*current_operator.c_str())
				{
				case '.'://连接操作。
				{
					NFA s1 = node_stack.top();
					node_stack.pop();
					NFA s2 = node_stack.top();
					node_stack.pop();
					NFA nfa = connect_nfa(s2, s1);
					node_stack.push(nfa);
					break;
					break;
				}
				case '*'://闭包操作。
				{
					break;
				}
				case '+'://
				{
					break;
				}
				case '|':
				{
					NFA s1 = node_stack.top();
					node_stack.pop();
					NFA s2 = node_stack.top();
					node_stack.pop();
					NFA nfa = or_nfa(s1, s2);
					node_stack.push(nfa);
					break;
				}
				default:
				{
					cout << "()碰到未知符号" << endl;
					break;
				}
				}
				current_operator = operator_stack.top();
				operator_stack.pop();

			}
			break;
		}
		case '.':  //连接运算符
		{
			if (regex == "")//对单个字符.的匹配。
			{
				NFA nfa = creat_basic_nfa(ch);
				node_stack.push(nfa);
				break;
			}
			while (operator_stack.size() != 0 && operator_stack.top() == ".")//栈顶运算符优先级比“（”高。
			{
				string current_operator = operator_stack.top();
				operator_stack.pop();
				if (current_operator == ".")
				{
					NFA s1 = node_stack.top();
					node_stack.pop();
					NFA s2 = node_stack.top();
					node_stack.pop();
					NFA nfa = connect_nfa(s2, s1);
					node_stack.push(nfa);
					break;
				}
			}
			operator_stack.push(".");
			break;
		}
		case '|'://或运算符
		{

			while (operator_stack.size() != 0 && operator_stack.top() != "(")//栈顶运算符优先级比“（”高。
			{
				string current_operator = operator_stack.top();
				operator_stack.pop();
				if (current_operator == ".")
				{
					NFA s1 = node_stack.top();
					node_stack.pop();
					NFA s2 = node_stack.top();
					node_stack.pop();
					NFA nfa = connect_nfa(s2, s1);
					node_stack.push(nfa);
					break;
				}
				else if (current_operator == "|")
				{
					NFA s1 = node_stack.top();
					node_stack.pop();
					NFA s2 = node_stack.top();
					node_stack.pop();
					NFA nfa = or_nfa(s1, s2);
					node_stack.push(nfa);
					break;
				}
			}
			operator_stack.push("|");
			break;
		}
		case '*'://闭包运算符。
		{
			if (regex == ""&&node_stack.size() == 0)//对字符"*"的匹配。
			{
				NFA nfa = creat_basic_nfa(ch);
				node_stack.push(nfa);
				break;
			}
			NFA s1 = node_stack.top();
			node_stack.pop();
			NFA  nfa = closure_nfa(s1);
			node_stack.push(nfa);
			break;
		}
		default://普通的变量
		{
			//构造NFA.
			NFA nfa = creat_basic_nfa(ch);
			node_stack.push(nfa);
			break;
		}
		}
		ch = getch();
	}
	while (operator_stack.size() != 0)
	{
		string current_operator = operator_stack.top();
		operator_stack.pop();
		switch (*current_operator.c_str())
		{
		case '.'://连接操作。
		{
			NFA s1 = node_stack.top();
			node_stack.pop();
			NFA s2 = node_stack.top();
			node_stack.pop();
			NFA nfa = connect_nfa(s2, s1);
			node_stack.push(nfa);
			break;
		}
		case '*'://闭包操作。
		{
			NFA s1 = node_stack.top();
			node_stack.pop();
			NFA  nfa = closure_nfa(s1);
			node_stack.push(nfa);
			break;
		}
		case '+'://
		{
			break;
		}
		case '|':
		{
			NFA s1 = node_stack.top();
			node_stack.pop();
			NFA s2 = node_stack.top();
			node_stack.pop();
			NFA nfa = or_nfa(s1, s2);
			node_stack.push(nfa);
			break;
		}
		default:
		{
			cout << "()碰到未知符号" << endl;
			break;
		}
		}
	}
	return node_stack.top();
}
#include<queue>;
//NFA到DFA
vector<string> closure(vector<string> A, map<string, map<string, vector<string>>> &transform_f)
//返回A集合中任意一个元素经过任意条K弧（空弧)所能到达的状态集合B
{
	set<string> result(A.begin(),A.end());
	queue<string> queue_;
	for (int i = 0; i < A.size(); i++)
	{
		queue_.push(A[i]);
	}
	string state;
	while (queue_.size())
	{
		state = queue_.front();
		queue_.pop();
		auto w = transform_f.find(state);
		if (w != transform_f.end()) // 当state是终态时，不会找到这个状态
		{
			auto w0 = w->second.find("kkk");
			if (w0 != w->second.end())//state有空弧转换
			{
					for (int j = 0; j < w0->second.size(); j++)
					{
						if (result.find(w0->second[j])==result.end())//结果中还没有这个元素
						{
							result.insert(w0->second[j]);
							queue_.push(w0->second[j]);
						}
					}
			}
		}
	}
	vector<string> t(result.begin(), result.end());
	sort(t.begin(), t.end());
	return t;
}
vector<string> move(vector<string> T, string &char_0, map<string, map<string, vector<string>>> &transform_f)
//状态T中的某一个状态经过一次char_0弧可以到达的状态全体。（经过char_0前可以经过k弧。）
//tramsform_f是转移函数。A是状态全体。
{
	set<string> result;
	for (int i = 0; i < T.size(); i++)
	{
		auto w = transform_f.find(T[i]);
		if (w != transform_f.end())//如果不是终态
		{
			auto w0 = w->second.find(char_0);
			if (w0 != w->second.end())//如果有char_0弧转换
			{
				for (int j = 0; j < w0->second.size(); j++)
				{
					result.insert(w0->second[j]);
				}
			}
		}
	}
	vector<string> t(result.begin(), result.end());
	sort(t.begin(), t.end());
	return t;
	/*
	for (int i = 0; i < T.size(); i++)
	{

		vector<string> C;
		auto w = transform_f.find(T[i]);
		if (w != transform_f.end())
		{
			auto w0 = w->second.find("kkk");
			if (w0 != w->second.end())
			{
				for (int j = 0; j < w0->second.size(); j++)
				{
					C.push_back(w0->second[j]);
				}
				move(C, char_0, transform_f, A);
			}
			else
			{
				w0 = w->second.find(char_0);
				if (w0 != w->second.end())
				{
					for (int j = 0; j < w0->second.size(); j++)
					{
						A.push_back(w0->second[j]);
					}
				}
			}
		}
	}
	*/
}
map<string,string> move(vector<string> T, string &char_0, map<string, map<string, string>> &transform_f)
//状态T中的某一个状态经过一次char_0弧可以到达的状态全体。（经过char_0前可以经过k弧。）
{

	map<string,string> result;
	for (int i = 0; i < T.size(); i++)
	{
		auto w = transform_f.find(T[i]);
		if (w != transform_f.end())//如果不是终态
		{
			auto w0 = w->second.find(char_0);
			if (w0 != w->second.end())//如果有char_0弧转换
			{
				for (int j = 0; j < w0->second.size(); j++)
				{
					result[T[i]]= w0->second[j];
				}
			}
		}
	}
	return result;
}
middle_DFA nfa_to_dfa(NFA nfa)
{
	middle_DFA   dfa;
	//输入字符。
	dfa.input_char = nfa.input_char;

	vector<string> search;//来检测是否为终态
	if (nfa.vector_end.size() != 0)
	{
		for (int i = 0; i < nfa.vector_end.size(); i++)
		{
			search.push_back(nfa.vector_end[i]);
		}
	}
	else
	{
		search.push_back(nfa.end);
	}
	//求出初态。
	vector<string> B = closure(vector<string>{nfa.start}, nfa.transform_f);
	deque<vector<string>> stack_;//当stack用。
	stack_.push_back(B);
	set<vector<string>> sign;//看当前状态集合是否已经放入栈中的标记符号。
	sign.insert(B);
	vector<string> T;
	dfa.start = B;//DFA的初始状态。
	while (stack_.size() != 0)
	{
		T = stack_.front();
		stack_.pop_front();
		//DFA的终态。
		for (int i=0; i<search.size(); i++)
		{
			if (binary_search(T.begin(), T.end(), search[i]))
			{
				dfa.end_type[T] = nfa.end_type[search[i]];
				dfa.end.push_back(T); break;
			}
		}
		map<string, vector<string>> W;
		for (int i = 0; i < nfa.input_char.size(); i++)
		{
			vector<string> A=move(T, nfa.input_char[i], nfa.transform_f);
			vector<string> Z = closure(A, nfa.transform_f);
			if (Z.size() == 0)
			{
				Z.push_back("111111");
				W[nfa.input_char[i]] = Z;
				continue;
			}
			if (sign.find(Z) == sign.end())
			{
				sign.insert(Z);
				stack_.push_front(Z);
			}

			W[nfa.input_char[i]] = Z;
		}
		dfa.transform_f[T] = W;
	}
	cout << endl;
	cout << "转换成DFA之后:" << endl;
	return dfa;
}
