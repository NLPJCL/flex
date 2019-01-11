#include<deque>
#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<set>
#include<algorithm>
#include<numeric>
#include"stack"
#include"NFA.h"
#include"middle_DFA.h"
#include"main.h"
#include"DFA.h"
#include"Lexer.h"
#include <ctime>  //clock_t, clock()
using namespace std;
int main()
{

	//读取文件中的正则表达式并存储起来。

	/*
	vector<NFA> s1 = read_regex_file_to_nfa("a.txt");
	NFA nfa_3 = connect_nfa(s1);
//	nfa_3.print();
	clock_t timeStart = clock();
	middle_DFA middle_dfa = nfa_to_dfa(nfa_3);
	cout << "construction, milli-seconds : " << (clock() - timeStart) << endl;

	DFA dfa = middle_dfa_to_dfa(middle_dfa);
	dfa.save("a_dfa.txt");
//	dfa = min_DFA(dfa);//功能尚未完善，关注github更新。
	*/

	//打开对应DFA进行词法分析以及统计
	DFA dfa = read_dfa_file("dfa_opt.txt");
	file_name.open("test1.txt");
	token(dfa);

	for (auto w = map_count.begin(); w != map_count.end(); w++)
	{
		cout << w->first << "\t字符共有" << w->second <<"个"<< endl;
	}
	system("pause");
	return 0;
}