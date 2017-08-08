/////////////////////////////////////////////////////////////////////
//  typetable.cpp - Creates the Type Table				           //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////

#include "../TypeTable/typetable.h"

using namespace CodeAnalysis;

//<------------------ TypeTable Class Constructor Building the AST tree ------------------>
TypeTable::TypeTable() :
	ASTref_(Repository::getInstance()->AST()) {}



//<------------------ Returns count of table ------------------>
//size_t TypeTable::count()
//{
//	return table.size();
//}

//<------------------ Checks if token is of type Class, struct, enum or global func ------------------>
bool TypeTable::checkFunction(ASTNode*pNode)
{
	static std::string types[] = {
		"class", "struct", "enum"
	};
	if (pNode->type_ == "function" && nameSpace == "Global Namespace" && pNode->name_ != "main" && pNode->name_ != "void" && parent_type == "namespace")
	{
		return true;
	}
	for (std::string type : types)
	{
		if (pNode->type_ == type)
			return true;
	}
	return false;
}

//<------------------ DFS traversal through AST Tree ------------------>
void TypeTable::InsertIntoTypeTable(ASTNode* pNode)
{
	try
	{
		if (pNode->type_ == "namespace")
			namespaceStack.push(pNode->name_);
		nameSpace = namespaceStack.top();
		if (checkFunction(pNode))
		{
			Values x;
			x.file1 = pNode->path_;
			x.Namespace = nameSpace;
			x.nodeType = pNode->type_;
			std::vector<Values> temp;
			if (table.find(pNode->name_) != table.end())
			{
				temp = table[pNode->name_];
				temp.push_back(x);
			}
			else {
				temp.push_back(x);
			}
			table[pNode->name_] = temp;
			//ifExists(pNode->name_, temp);
		}
		for (auto pChild : pNode->children_)
		{

			parent_type = pNode->type_;
			InsertIntoTypeTable(pChild);
			parent_type = pNode->type_;
		}
		if (pNode->type_ == "namespace") {
			namespaceStack.pop();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}

//<------------------ Populate the type table (main function to call) ------------------>
void TypeTable::GenerateTypeTable()
{
	try
	{
		ASTNode* proot = ASTref_.root();
		InsertIntoTypeTable(proot);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}

//<------------------ Returns the keys from table ------------------>
std::vector<std::string> TypeTable::keys()
{
	std::vector<std::string> temp;
	for (Item item : table)
	{
		temp.push_back(item.first);
	}
	return temp;
}

//<------------------ Returns the typeTable back to main ------------------>
std::unordered_map<Key, std::vector<Values>>& TypeTable::returnTypeTable()
{
	return table;
}

#ifdef TEST_TestTable

int main()
{
	Type_Table tb_temp;
	tb_temp.ConstructTypeTable();

	using Value_Vector = std::vector<Values>;
	using Item = std::pair<Key, Value_Vector>;

	for (Item item : tb_temp.returnTypeTable())
	{
		std::cout << "\n " << item.first;
		for (Values x : item.second)
			std::cout << "\n\t" << x.file_name << "\t" << x.nameSpace_name << "\t" << x.node_type;
		std::cout << "\n ";
	}

}
#endif


