#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;
class middle_DFA
{
public:

	void print()
	{
		cout << "DFA的初态：";
		for (int i = 0; i < start.size(); i++)
		{
			cout << start[i] <<" ";
		}
		cout << endl;
		cout << "DFA的终态";
		for (int i = 0; i < end.size(); i++)
		{
			cout << "<";
			for (int j = 0; j < end[i].size(); j++)
			{
				cout << end[i][j] << " ";
			}
			cout << ">";
		}
		cout << "DFA的输入字符：";
		for (int i = 0; i < input_char.size(); i++)
		{
			cout << input_char[i] << " ";
		}
		cout << "DFA的转移函数是：" << endl;
		for (auto w = transform_f.begin(); w != transform_f.end(); w++)
		{
			for (auto w0 = w->second.begin(); w0 != w->second.end(); w0++)
			{
				cout << "f(";
				for (int i = 0; i < w->first.size(); i++)
				{
					cout << w->first[i] << " ";
				}
				cout << "," << w0->first << ")->";
				for (int i = 0; i < w0->second.size(); i++)
				{
					cout << w0->second[i] << " ";
				}
				cout << endl;
			}
		}

		for (auto w = end_type.begin(); w != end_type.end(); w++)
		{
			cout << "<";
			for (int i = 0; i < w->first.size(); i++)
			{
				cout << w->first[i] << " ";
			}
			cout<< ">:" << w->second << endl;
		}


	}
	//为了构造词法分析器引入数据类型、
	map<vector<string>, string> end_type;//终态集合和对应的终态类型
	//middle_DFA基础数据结构。
	vector<string> start;//开始节点
	vector<vector<string>> end;//结束节点
	vector<string> input_char;//输入字符串
	map<vector<string>, map<string, vector<string>>> transform_f;//转换函数
};

