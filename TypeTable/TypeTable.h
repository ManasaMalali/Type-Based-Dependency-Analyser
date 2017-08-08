#pragma once
/////////////////////////////////////////////////////////////////////
//  typetable.h - Creates the Type Table				           //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana, SUID-864888072, mmalalin@syr.edu//
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================

This module defines a Type_Table class
* - The functions defined in this class are used for,
> Creates an unordered map to store the types
> Checks for Classes, structs, enums and global functions in ASTree
> Populates the unordered map
> Returns the populated type table (unordered map)

Private Interface:
=================
Type_table Class > ---- >

* AbstrSynTree& ASTref_;									- AbstractSyntaxTree
* std::unordered_map<Key, Value_Vector> table;				- Type Table
* std::string nameSpace = "Global";							- Current namespace for pNode
* std::stack<nSpace> namespaceStack;						- Keep track of current namespace
* std::string parent_type;									- Keeping track of parent type of pNode


Build Process:
==============
Required files
-AbstrSynTree.h
-ActionAndRules.h

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
#include <stack>
#include <unordered_set>

#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Parser/ActionsAndRules.h"

using namespace CodeAnalysis;

using file = std::string;
using Namespace = std::string;
using type = std::string;
using Key = std::string;

class Values
{
public:
	file file1;
	Namespace Namespace;
	type nodeType;
};

class TypeTable
{
public:
	using Value_Vector = std::vector<Values>;
	TypeTable();

	//bool ifExists(Key key, Value_Vector v);
	size_t count();
	void InsertIntoTypeTable(ASTNode* pNode);
	bool checkFunction(ASTNode* pNode);
	void GenerateTypeTable();
	std::vector<std::string> keys();
	std::unordered_map<Key, Value_Vector>& returnTypeTable();

private:
	using Item = std::pair<Key, Value_Vector>;
	AbstrSynTree& ASTref_;
	std::unordered_map<Key, Value_Vector> table;
	std::string nameSpace = "Global";
	std::stack<Namespace> namespaceStack;
	std::string parent_type;

};
