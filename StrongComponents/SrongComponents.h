#pragma once
/////////////////////////////////////////////////////////////////////
//  StrongComponents.h		 - Executed Tarjan's SCC algorithm     //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <list>
#include <stack>

#include "../TypeTable/typetable.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../FileSystem/FileSystem.h"
#include "../DependencyAnalysis/DependencyAnalyser.h"
#include "../Analyzer/Executive.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"


using namespace CodeAnalysis;
using namespace NoSQLDB;
using namespace XmlProcessing;

using sptr = std::shared_ptr<AbstractXmlElement>;


//------------------------------------------------------------------------------------------------------------//
class Strongcomponents
{
public:
	using File = std::string;
	using Files = std::vector<File>;
	using Key = std::string;
	using Children = std::vector<Key>;
	using Item = std::pair<Key, Element<std::string>>;

	void ConstructGraph(DependencyTable dp);
	void FindStrongComponentsR(int u, int scc_disc[], int scc_low[], std::stack<int> *scc_stack, bool sBool[]);
	void FindStrongComponents();
	std::string Mapback(int w);
	std::unordered_map<int, std::vector<std::string>>& returnMatrix();
	
	std::unordered_map<std::string, std::vector<std::string>> tab;
	int mapFiles();
	void createMatrix();
	
	std::list<int>* returnAdjMatrix();	
	std::string Strongcomponents::sccToXml(std::string filename);

private:
	DependencyTable dtable;
	
	int vertices_no;
	std::unordered_map<File, int> mMap;
	std::list<int>* adj_matrix;
	std::unordered_map<int, std::vector<std::string>> scc_matrix;
	std::vector<std::string> temp;
	int a = 0;
	int verticesCount;
	std::vector<File> newFiles;
	std::unordered_map<File, int> map;
	std::list<int> *adjacencyMatrix;
	DependencyTable dp;
	NoSQLDB::NoSqlDb<std::string> db;

};
