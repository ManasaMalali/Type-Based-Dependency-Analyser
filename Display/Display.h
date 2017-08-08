#pragma once
/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display Typetable, DependencyDB, SCC		       //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID -864888072, mmalalin@syr.edu//
/////////////////////////////////////////////////////////////////////


#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

#include "../DependencyAnalysis/DependencyAnalyser.h"
#include "../NoSqlDb/NoSqlDb.h"
//#include "../StrongComponents/SCC.h"
#include "../TypeTable/typetable.h"

using namespace CodeAnalysis;

class Display
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	using File = std::string;
	using Files = std::vector<File>;

	using tempVec = std::vector<Values>;
	using table1 = std::unordered_map<Key, tempVec>;

	void TypeTableDisplay(table1 newtable1);
	void DependencyDisplay(NoSQLDB::NoSqlDb<std::string> db);
	void StrongComponentDisplay(std::unordered_map<int, std::vector<std::string>> scc_matrix_temp);
	void showXML(std::string xmlString);

};
