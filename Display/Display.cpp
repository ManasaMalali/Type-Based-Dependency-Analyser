/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display Typetable, DependencyDB, SCC		       //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID -864888072,		//
//				   mmalalin@syr.edu									//
/////////////////////////////////////////////////////////////////////

#include "Display.h"
using namespace std;

// Uses lambda to display the type table

void Display::TypeTableDisplay(table1 newtable1)
{
	if (!newtable1.empty())		// checks if the type table is not empty
	{		
		std::vector<std::string>::iterator vec_iter;
		std::for_each(newtable1.begin(), newtable1.end(), [](std::pair<Key, tempVec > element) {
			std::cout << endl;
			std::cout << "  Key: " << element.first << std::endl;
			for (Values val : element.second)
			{
				std::cout << "  Type: " << val.nodeType << std::endl;
				std::cout << "  Namespace: " << val.Namespace << std::endl;
				std::cout << "  Filepath: " << val.file1 << std::endl;
			}
			std::cout << std::endl << std::endl;
		});
	}
	else
	{
		std::cout << "  The type table is empty!" << std::endl << std::endl;
	}
}

// Function to display Dependency Table analysis 
void Display::DependencyDisplay(NoSQLDB::NoSqlDb<std::string> db)
{
	if (db.count() > 0)
	{
		std::vector<std::string>::iterator vec_iter;
		std::unordered_map<std::string, std::vector<std::string>>::iterator map_iter;
		Files keys = db.keys();		
		for (Key key : keys)
		{
			std::cout << " File Name :" << key << std::endl << " Depends on: " << std::endl;
			std::cout << db.value(key).show();
			std::cout << endl;
		}		
	}
	else
	{
		std::cout << "There are no dependencies to show." << std::endl;
	}
	std::cout << std::endl;
}

//<---------------------- Function to display Strongly connected graph files ------------------->
void Display::StrongComponentDisplay(std::unordered_map<int, std::vector<std::string>> graph)
{		
		int i = 1;
		for (auto a : graph)
		{
			cout << " Strong Component " << i << ":" << endl << endl;
			i++;
			for (auto b : a.second)
			{
				cout << "\t -> " << b << endl;
			}
			std::cout << "\n\n\n";
		}
}	

#ifdef TEST_DISPLAY

int main()
{
	using File = std::string;
	using Files = std::vector<File>;
	Files allfilePaths;

	allfilePaths.push_back("C:\\Users\\Manasa\\Desktop\\OOD\\Project2\\Temp\\B.h");
	allfilePaths.push_back("C:\\Users\\Manasa\\Desktop\\OOD\\Project2\\Temp\\A.h");
	allfilePaths.push_back("C:\\Users\\Manasa\\Desktop\\OOD\\Project2\\Temp\\A.cpp");
	allfilePaths.push_back("C:\\Users\\Manasa\\Desktop\\OOD\\Project2\\Temp\\B.cpp");

	Type_Table tb;
	tb.ConstructTypeTable();

	DependencyTable dp(tb.returnTypeTable(), allfilePaths);
	dp.DepAnal();

	StronglyConnectedComps scc;
	scc.ConstructGraph(dp);

	Show showObj;
	disp.TypeTableDisplay(tb.returnTypeTable());
	disp.DependencyDisplay(dp.getDb());
	disp.StrongComponentDisplay(scc.returnMatrix());
}

#endif


