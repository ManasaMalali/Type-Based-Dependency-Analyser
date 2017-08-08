#pragma once
/////////////////////////////////////////////////////////////////////
//  DependencyAnalysis.h - Creates Dependency database			   //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================

This module defines a DependencyTable class
* - The functions defined in this class are used for,
> Stores the typetable from Type_table class
> Uses this type table, and tokenizer on the whole project to detect hits
> Checks for dependencies, And stores it into Database

Private Interface:
=================
DependencyTable Class > ---- >

* std::unordered_map<Key, Value_Vector> tempTypeTable;		- Stores the populated type table
* Files filePaths;											- Stores all the .cpp, .h and .cs files in directory
* NoSQLDB::NoSqlDb<std::string> dependency_Store;			- Database to hold The dependencies


Build Process:
==============
Required files
-AbstrSynTree.h
-ActionAndRules.h
-Executive.h
-typetable.h
-Tokenizer.h
-FileSystem.h
-NoSqlDb.h

Build commands
- devenv CodeAnalyzerEx.sln

Maintenance History:
====================
ver 1.0 : 07 Mar 17
- first release
*/

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>

#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Parser/ActionsAndRules.h"
#include "../Analyzer/Executive.h"
#include "../TypeTable/typetable.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../Tokenizer/Tokenizer.h"
#include "../FileSystem/FileSystem.h"

using namespace CodeAnalysis;

class DependencyTable
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;	
	using table = std::unordered_map<Key, std::vector<Values>>;
	using File = std::string;
	using Files = std::vector<File>;
	std::vector<std::string> retrieveFiles();
	void DepAnal(Files files, table newTable);
	bool TokensInFile(File eachFile, table typetable);	
	NoSQLDB::NoSqlDb<std::string>& getDb();
	void setXMLPath(std::string);

private:
	std::unordered_map<Key, std::vector<Values>> typetable;
	std::unordered_map<Key, std::vector<Values>> ntypetable;
	Files filePaths;
	Files nfilePaths;
	NoSQLDB::NoSqlDb<std::string> db;
	std::string xmlPath;
};


