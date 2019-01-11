#pragma once
//词法分析相关，的工具连接两个NFA.
#include"NFA.h"
#include"middle_DFA.h"
#include"main.h"
#include"DFA.h"
//词法分析器相关，读取文件中的正则表达式。将其转换成对应的NFA，并返回一个NFA的数组。
vector<NFA> read_regex_file_to_nfa(string file_name)
{
	fstream regex_file(file_name);
	string line;
	string regex_type;
	NFA nfa;
	vector<NFA> vector_nfa;
	while (getline(regex_file, line))
	{
		int i = line.find("\t");
		regex = line.substr(0, i);
		regex_type = line.substr(i + 1);
		nfa = regex_to_nfa();
		nfa.type = regex_type;
		vector_nfa.push_back(nfa);
		former_ch = "";
	}
	return vector_nfa;
}
//连接N个NFA
NFA connect_nfa(vector<NFA> s1)
{
	NFA nfa;
	//
	for (int i = 0; i < s1.size(); i++)
	{
		for (int j = 0; j<s1[i].input_char.size(); j++)
		{
			nfa.input_char.push_back(s1[i].input_char[j]);
		}
	}

	//初始状态
	nfa.start = to_string(node_count);
	node_count++;
	//第一步
	map<string, vector<string>> w;
	vector<string>  start;
	for (int i = 0; i < s1.size(); i++)
	{
		start.push_back(s1[i].start);
		nfa.vector_end.push_back(s1[i].end);
		nfa.end_type[s1[i].end] = s1[i].type;//把结束节点和类型对应起来。
		for (auto w = s1[i].transform_f.begin(); w != s1[i].transform_f.end(); w++)
		{
			nfa.transform_f[w->first] = w->second;

		}
	}
	w["kkk"] = start;
	nfa.transform_f[nfa.start] = w;
	return nfa;
}
//词法分析相关，给中间生成的DFA改名称。
DFA middle_dfa_to_dfa(middle_DFA middle_dfa)
{
	int i = 0;
	DFA dfa;
	//输入字符串
	dfa.input_char = middle_dfa.input_char;
	//开始节点
	dfa.start = to_string(i);
	i++;
	map<vector<string>, string> sign;
	sign[middle_dfa.start] = dfa.start;
	//所有状态
	set<string> all_state_set;
	all_state_set.insert(to_string(i));

	for (auto w = middle_dfa.transform_f.begin(); w != middle_dfa.transform_f.end(); w++)
	{
		map<string, string> q;
		for (auto w0 = w->second.begin(); w0 != w->second.end(); w0++)
		{
			if (sign.find(w0->second) == sign.end())
			{
				sign[w0->second] = to_string(i);
				all_state_set.insert(to_string(i));//收集所有状态。
				q[w0->first] = to_string(i);
				i++;
			}
			else
			{
				q[w0->first] = sign[w0->second];
			}
		}
		if (sign.find(w->first) == sign.end())
		{
			sign[w->first] = to_string(i);
			all_state_set.insert(to_string(i));//收集所有状态。
			dfa.transform_f[to_string(i)] = q;
			i++;
		}
		else
		{
			dfa.transform_f[sign[w->first]] = q;
		}
	}
	//结束状态。
	for (int i = 0; i < middle_dfa.end.size(); i++)
	{
		dfa.end.push_back(sign[middle_dfa.end[i]]);//结束状态赋值
		dfa.end_type[sign[middle_dfa.end[i]]] = middle_dfa.end_type[middle_dfa.end[i]];//结束状态对应类型。
	}
	for (auto w = all_state_set.begin(); w != all_state_set.end(); w++)
	{
		dfa.all_state.push_back(*w);
	}
	return dfa;
}
//DFA的最小化
DFA min_DFA(DFA dfa)
{
	int count = 0;//所有子集的唯一标志。

	map<string, int> state_set;//第一个参数是一个状态，第二个参数子集对应的唯一标志。



	//构建初始的接受状态和非接受状态。
	
	//把各种类型的终态放进去。

	//中间数据结构
	multimap<string, string> type_end;//对应的类型，状态。

	for (auto w=dfa.end_type.begin();w!=dfa.end_type.end();w++)
	{
		type_end.insert(make_pair(w->second, w->first));
	}

	map<vector<string>, int> all_set;
	vector<string> new_;
	string old_type;
	int t = 0;
	//

	for (auto w = type_end.begin(); w != type_end.end(); w++)
	{
		if (old_type != w->first&&t!=0)
		{
			all_set[new_] = count;
			count++;
			new_.clear();
		}
		t = 1;
		new_.push_back(w->second);
		old_type = w->first;
	}

	all_set[new_] = count;
	count++;

	//
	for (auto w = all_set.begin(); w != all_set.end(); w++)
	{
		for (int i= 0; i<w->first.size(); i++)
		{
			state_set[w->first[i]] = all_set[w->first];
		}
	}
	//构建非接受状态
	sort(dfa.end.begin(), dfa.end.end());
	vector<string> no_acceptable;
	for (int i = 0; i < dfa.all_state.size(); i++)
	{
		if (binary_search(dfa.end.begin(), dfa.end.end(), dfa.all_state[i]))
		{
			continue;
		}
		no_acceptable.push_back(dfa.all_state[i]);
		state_set[dfa.all_state[i]] = count;
	}
	all_set[no_acceptable] = count;
	count++;
	cout << count << endl;
	//单元测试,输出各种结束状态，和非接受状态
	/*
	for (auto w = all_set.begin(); w != all_set.end(); w++)
	{
		for(int i=0;i<w->first.size();i++)
		{
			cout << w->first[i] << " ";
		}

		cout << ":"<<w->second <<endl;
	}

	*/
	cout << "分割法化简前：总的状态类型是：" << all_set.size() << endl;

	//分割法
	for (auto z = all_set.begin(); z != all_set.end(); z++)
	{
		if (z->first.size() != 1)
		{
			for (int q = 0; q < dfa.input_char.size(); q++)
			{
				map<string, string> A = move(z->first, dfa.input_char[q], dfa.transform_f);

				vector<vector<string>> w0(count + 1);//用来存储不同分组的信息。
				set<int> z0;
				for (auto w = A.begin(); w != A.end(); w++)
				{
					w0[state_set[w->second]].push_back(w->first);
					z0.insert(state_set[w->second]);
				}
				if (z0.size() != 1)
				{
					all_set.erase(z->first);
					for (int i = 0; i < w0.size(); i++)
					{
						if (w0[i].size() != 0)
						{
							all_set.insert(make_pair(w0[i], count));
							count++;

							for (int j = 0; j < w0[i].size(); j++)
							{
								state_set[w0[i][j]] = count;
							}
						}
					}
					z = all_set.begin();
					break;
				}
			}
		}
	}
	//转换变换关系。
	int num = 0;
	for (auto w = all_set.begin(); w != all_set.end(); w++)
	{
	//	cout << w->first.size() << endl;
		num ++;
	}
	cout << "化简过后：总的状态类型是："<<all_set.size()<< endl;



	//DFA dfa;

	return DFA();



	//分割法




}
//从已经生成的DFA开始进行词法分析。

//读取dfa文件。
DFA read_dfa_file(string file_name)
{
	int i, i0;
	ifstream dfa_file(file_name);
	DFA dfa;
	string line;
	//开始字符
	getline(dfa_file, line);
	dfa.start = line;
	//获取终态集合。
	getline(dfa_file, line);
	i0 = line.find(" ");
	dfa.end.push_back(line.substr(0, i0));
	while (1)
	{
		int i = i0;
		i0 = line.find(" ", i + 1);
		if (i0 == line.npos)
		{
			break;
		}
		dfa.end.push_back(line.substr(i + 1, i0 - i - 1));
	}
	//获取输入字符集合
	getline(dfa_file, line);
	i0 = line.find(" ");
	dfa.input_char.push_back(line.substr(0, i0));
	while (1)
	{
		int i = i0;
		i0 = line.find(" ", i + 1);
		if (i0 == line.npos)
		{
			break;
		}
		dfa.input_char.push_back(line.substr(i + 1, i0 - i - 1));
	}
	//获取终态对应类型。
	getline(dfa_file, line);
	i0 = line.find(" ");
	int j = line.find(":");
	dfa.end_type[line.substr(0, j)] = line.substr(j + 1, i0 - (j + 1));
	while (1)
	{
		int i = i0;
		i0 = line.find(" ", i + 1);
		if (i0 == line.npos)
		{
			break;
		}
		j = line.find(":", i);
		dfa.end_type[line.substr(i + 1, j - (i + 1))] = line.substr(j + 1, i0 - (j + 1));
	}
	//获得转换函数对应类型。
	string former_state = "12345678";
	string state;
	map<string, string> transform;
	while (getline(dfa_file, line))
	{
		int i = line.find(" ");
		state = line.substr(0, i);
		int i0 = line.find(" ", i + 1);
		string char_0 = line.substr(i + 1, i0 - i - 1);
		if (state != former_state)
		{
			dfa.transform_f[former_state] = transform;
			transform.clear();
		}
		i = i0;
		i0 = line.find(" ", i + 1);
		former_state = state;
		transform[char_0] = line.substr(i + 1, i0 - i - 1);
	}
	dfa.transform_f[state] = transform;
	return dfa;
}

//pl0的词法分析，读取pl0源程序，返回一个字符。
string getch_pl0()
{
	string ch;
	if (line.size() == 0)
	{
		getline(file_name, line);
		line_number++;
	}
	if (line.size() != 0)
	{
		ch = line.substr(0, 1);
		line.erase(0, 1);
		return ch;
	}
	else
	{

		return "";
	}
}
//DFA驱动下的词法分析器。
void token(DFA dfa)
{
	string s = dfa.start;
	string ch = getch_pl0();
	string final_state;
	int t = 1;
	while (ch != "")
	{
		auto w = dfa.transform_f.find(s);
		auto w0 = w->second.find(ch);
		if (w0 != w->second.end() && w0->second != "9"&&t)//有下一个状态和下一个状态不是空"9"为空和遇到空格和tab要重新开始新的定位。
		{
			id = id + ch;
			s = w0->second;	
			if (id != ".")
			{
				ch = getch_pl0();
			}
			while (ch == "\t" || ch == " ")
			{
				ch = getch_pl0();
				t = 0;
			}
			if (dfa.end_type.find(s) != dfa.end_type.end())//判断当前状态是否为终止状态。
			{
				final_state = s;
			}
		}
		else
		{
			t = 1;
			if (final_state.size() != 0)//已经访问过终止状态了
			{

				if (dfa.end_type[final_state] == "ident")
				{
					cout << id << ":ident"<<endl;
					map_count["ident"]++;
				}
				else if (dfa.end_type[final_state] == "number")
				{
					cout << id << ":number"<<endl ;
					map_count["number"]++;
				}
				else
				{
					map_count[id]++;
					cout << id << ":" << dfa.end_type[final_state] << endl;
					if (id == ".")
					{
						break;
					}
				}
				s = dfa.start;
				id.clear();
				final_state.clear();
			}
			else//出错了。
			{
				cout << "第" << line_number << "行出现错误" << endl; break;
			}
		}
	}
}