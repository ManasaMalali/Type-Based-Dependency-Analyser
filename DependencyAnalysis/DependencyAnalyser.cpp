/////////////////////////////////////////////////////////////////////
//  DependencyAnalyzer.cpp - Creates the Dependency Table          //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////

#include "../DependencyAnalysis/DependencyAnalyser.h"

using namespace CodeAnalysis;

//<---------------------- Returning The dependency Table (database) ------------------->
NoSQLDB::NoSqlDb<std::string>& DependencyTable::getDb()
{
	//nfilePaths = files;
	//ntypetable = newTable;
	return db;
}

//To build the dependency table 
void DependencyTable::DepAnal(Files files, table newTable)
{
	filePaths = files;
	typetable = newTable;
	try {

		for (File f1 : filePaths)
		{
			TokensInFile(f1, typetable);
		}		
		//db.toXml(xmlPath);
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
}

// Checks for dependency with the tokens in each file 
bool DependencyTable::TokensInFile(File eachFile, table typetable)
{
	try {
		std::string just_file = FileSystem::Path::getName(eachFile);
		std::ifstream in(eachFile);
		if (!in.good())
		{
			std::cout << "\n Cannot open file " << eachFile << "\n\n";
			return false;
		}
		Scanner::Toker toker;
		toker.returnComments(false);
		toker.attach(&in);		
		do
		{
			std::string newToken = toker.getTok();
			if (newToken == "(" || newToken == ")" || newToken == "{" || newToken == "}" || newToken == "[" || newToken == "]" || newToken == ";" || newToken == "\n")
				continue;			
			
			if (typetable.find(newToken) != typetable.end())
			{
				for (Values value : typetable[newToken])
				{
					if (value.file1 != eachFile)
					{
						NoSQLDB::Element<std::string> elem = db.value(eachFile);
						elem.addChildren(value.file1);
						db.Update(eachFile, elem);
					}
				}
			}
		} while (in.good());
	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n  " << ex.what();
	}
	return true;
}

std::vector<std::string> DependencyTable::retrieveFiles()
{
	std::vector<File> tempVector;
	for (File x : filePaths)
	{
		tempVector.push_back(FileSystem::Path::getPath(x) + FileSystem::Path::getName(x));
	}
	return tempVector;
}

void DependencyTable::setXMLPath(std::string filePath)
{
	xmlPath = filePath;
}

#ifdef TEST_DPAnalyzer

#include <vector>
#include <string>

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
	tb.populateTypeTable();

	DependencyTable dp_temp(tb.returnTypeTable(), allfilePaths);
	dp_temp.dependencyMain();

	Files keys = dp_temp.getDb().keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << dp_temp.getDb().value(key).show();
	}
}

#endif
