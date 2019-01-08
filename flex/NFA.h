#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;

//M = {状态集合，输入字符结合，转换函数集合，非空初态集合，终态集合}
class NFA
{
public:
	void print()
	{
		cout << "NFA的初态：";
		cout << start << endl;
		if (end .size()!=0)
		{
			cout << "NFA的终态：";
			cout << end << endl;
		}
		else
		{
			cout << "NFA的终态:";
			for (int i = 0; i < vector_end.size(); i++)
			{
				cout << vector_end[i] << "\t";
			}
			cout << endl;
		}
		cout << "NFA的输入字符：";
		for (int i = 0; i < input_char.size(); i++)
		{
			cout << input_char[i] << " ";
		}
		cout << "NFA的转移函数是：" << endl;
		for (auto w = transform_f.begin(); w != transform_f.end(); w++)
		{
			for (auto w0 = w->second.begin(); w0 != w->second.end(); w0++)
			{
				cout << "f(" << w->first;
				cout << "," << w0->first << ")->";
				for (int i = 0; i < w0->second.size(); i++)
				{
					cout << w0->second[i] << " ";
				}
				cout << endl;
			}
		}

	}
	//为了构造词法分析器引入数据类型、
	string type;//类型
	vector<string>  vector_end;//为了合并NFA引入的一个数组。
	map<string, string>  end_type;//结束节点到类型的映射。
	//NFA基础数据结构。
	string start;//开始节点
	string end;//结束节点
	vector<string> input_char;//输入字符串
	map<string, map<string, vector<string>>> transform_f;//转换函数
};

