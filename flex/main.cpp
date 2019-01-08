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
#include <ctime> 

using namespace std;
int main()
{

	/*
	regex = "if";
	NFA nfa = regex_to_nfa();
	nfa.type = "if";
	nfa.print();
	//pl/0的标识符
	regex = "(i|j)(f|8)*";
	NFA nfa_1 = regex_to_nfa();
	nfa_1.type = "ident";
	nfa_1.print();
	//pl/0的数字
	regex = "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*";
	NFA nfa_2 = regex_to_nfa();
	nfa_2.type = "number";
	vector<NFA> s1;
	s1.push_back(nfa);
	s1.push_back(nfa_1);
	*/

	//读取文件中的正则表达式并存储起来。
	vector<NFA> s1 = read_regex_finle_to_nfa("pl0.txt");
	NFA nfa_3 = connect_nfa(s1);
	nfa_3.print();
	clock_t timeStart = clock();

	middle_DFA middle_dfa = nfa_to_dfa(nfa_3);
	DFA dfa = middle_dfa_to_dfa(middle_dfa);
	cout << "milli-seconds : " << (clock()-timeStart) << endl;

	//dfa.save();
	/*
	DFA dfa = read_dfa_file("dfa_.txt");
	file_name.open("test.txt");
	token(dfa);
	*/
	system("pause");
	
	return 0;
}