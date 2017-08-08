#pragma once
/////////////////////////////////////////////////////////////////////
//  NoSqlDb.h - Key-value pair in Db, and corresponding functions  //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   NoSql Database - CIS 687 Project 1              //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

using namespace XmlProcessing;

using sptr = std::shared_ptr<AbstractXmlElement>;

namespace NoSQLDB
{
	using Key = std::string;
	using Children = std::vector<Key>;


	template<typename Data>
	class Element
	{
	public:

		Element() {}
		Property<Children> dep;          // Dependencies

		void addChildren(Key key);
		std::string show();
	};

	//<---------------------- Function to append child to particular key ------------------->
	template<typename Data>
	void Element<Data>::addChildren(Key key)
	{
		Children temp = dep.getValue();
		for (Key x : temp)
		{
			if (x == key)
				return;
		}
		dep.getValue().push_back(key);
		return;
	}

	//<---------------------- Function to Display the dependencies (children) ------------------->
	template<typename Data>
	std::string Element<Data>::show()
	{
		int j = 1;
		std::ostringstream out;
		out.setf(std::ios::adjustfield, std::ios::left);
		Children dependencies_ = static_cast<Children>(dep);
		if (dependencies_.size() > 0)
		{
			for (int i = 0; i < dependencies_.size(); ++i)
			{
				out << "\t" << j << dependencies_[i] << endl;
				j++;
			}
			out << "\n\n";
		}
		return out.str();
	}


	template<typename Data>
	class NoSqlDb
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using xmlstring = std::string;

		Keys keys();
		bool save(Key key, Element<Data> elem);
		bool Update(Key key, Element<Data> elem);
		bool Delete(Key key);
		Element<Data> value(Key key);
		size_t count();
		//void display();
		std::string toXml(std::string filename);

	private:
		using Item = std::pair<Key, Element<Data>>;
		//std::string path;
		std::unordered_map<Key, Element<Data>> store;
	};

	//keys of dataBase are stored in a collection 
	template<typename Data>
	typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
	{
		Keys keys;
		for (Item item : store)
		{
			keys.push_back(item.first);
		}
		return keys;
	}

	//Elements are saved into DataBase using save function 
	template<typename Data>
	bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
	{

		if (store.find(key) != store.end())
			return false;

		store[key] = elem;
		return true;
	}

	//Elements metadata or data can be updated in to the database using update function
	template<typename Data>
	bool NoSqlDb<Data>::Update(Key key, Element<Data> elem)
	{
		//if (store.find(key) != store.end())
		store[key] = elem;
		return true;
	}

	//An element can be removed from database using Delete function
	template<typename Data>
	bool NoSqlDb<Data>::Delete(Key key)
	{
		if (store.find(key) == store.end()) {
			cout << "\n Given key does not exist in the database. \n";
			return false;
		}
		else {
			store.erase(key);
			cout << "\n Record with the given key was deleted\n";
			DeleteChildren(key);
			return true;
		}
	}
	// Function that returns all the metadata value
	template<typename Data>
	Element<Data> NoSqlDb<Data>::value(Key key)
	{
		if (store.find(key) != store.end())
			return store[key];
		return Element<Data>();
	}

	//count function returns the number of elements in the database
	template<typename Data>
	size_t NoSqlDb<Data>::count()
	{
		return store.size();
	}

	// Function to write the contents of the database to an Xml file
	template<typename Data>
	std::string NoSqlDb<Data>::toXml(std::string filename)
	{
		xmlstring xml;
		XmlDocument document;
		sptr pRoot = makeTaggedElement("Dependency_Table");
		document.docElement() = pRoot;
		Element<Data> temp;
		Keys newkeys = keys();

		for (Key k : newkeys)
		{
			sptr pKey = makeTaggedElement("Entry");
			pRoot->addChild(pKey);
			sptr px = makeTaggedElement("Key");
			pKey->addChild(px);
			sptr pxkey = makeTextElement(k);
			px->addChild(pxkey);

			temp = value(k);
			Keys children = temp.dep;

			sptr pDp = makeTaggedElement("Value - Dependencies");
			pKey->addChild(pDp);
			for (Key k : children)
			{
				sptr pChild = makeTaggedElement("File");
				pDp->addChild(pChild);
				sptr pFile = makeTextElement(k);
				pChild->addChild(pFile);
			}
		}
		xml = document.toString();
		try
		{
			if (filename == "")
				filename = "DependencyXML.xml";
			std::ofstream sample(filename);
			sample << xml;
			sample.close();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
		return xml;
	}
}