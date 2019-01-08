#pragma once
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<fstream>
using namespace std;

//M = {状态集合，输入字符结合，转换函数集合，非空初态集合，终态集合}
class DFA
{
public:
	void print()
	{
		
	}
	void save()
	{
		ofstream dfa_file("dfa_.txt");
		//开始节点
		dfa_file << start << endl;
		//结束节点
		for (int i = 0; i < end.size(); i++)
		{
			dfa_file << end[i] << " ";
		}
		dfa_file << endl;
		//输入字符串
		for (int j = 0; j < input_char.size(); j++)
		{
			dfa_file <<input_char[j] << " ";
		}
		dfa_file << endl;
		//终态对应类型
		for (auto w = end_type.begin(); w != end_type.end(); w++)
		{
			dfa_file << w->first << ":" << w->second << " ";//“:”  是英文的
		}
		dfa_file << endl;
		//转换函数
		for (auto w = transform_f.begin(); w != transform_f.end(); w++)
		{
			for (auto w0 = w->second.begin(); w0 != w->second.end(); w0++)
			{
				dfa_file << w->first << " " << w0->first << " " << w0->second <<" "<<endl;
			}
		}
	}
	//为了构造词法分析器引入数据类型、
	map<string, string> end_type;
	//NFA基础数据结构。
	string start;//开始节点
	vector<string> end;//结束节点
	vector<string> input_char;//输入字符串
	map<string, map<string, string>> transform_f;//转换函数
};	

