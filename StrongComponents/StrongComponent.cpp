/////////////////////////////////////////////////////////////////////
//  SCC.cpp - Creates a Strongly Connected graph		           //
//																   //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Dell Inspiron, Windows 8.1			           //
//  Application:   Dependency Analysis - CIS 687 Project 2         //
//  Author:        Manasa Malali Nagabhushana SUID:845368670       //
/////////////////////////////////////////////////////////////////////

#include "../StrongComponents/SrongComponents.h"


//<---------------------- Constructing an Adjacency Matrix ------------------->
void Strongcomponents::createMatrix()
{
	Files files = db.keys();
	for (File file : files)
	{
		Files listFiles = db.value(file).dep.getValue();
		int index = map[file];
		for (File x : listFiles)
		{
			adjacencyMatrix[index].push_back(map[x]);
		}
	}
}

//<---------------------- Returning the private Adjacency matrix ------------------->
std::list<int>* Strongcomponents::returnAdjMatrix()
{
	return adjacencyMatrix;
}

//<---------------------- Constructing an unordered Map to map Each file path to int ------------------->
int Strongcomponents::mapFiles()
{
	int i = 0;
	for (File x : newFiles) //    File x : arrayOfFiles)
	{
		map[x] = i;
		i++;
	}
	return i;
}



//<---------------------- Main Function to create SCC ------------------->
void Strongcomponents::ConstructGraph(DependencyTable dp)
{
	//dtable = dp;
	int i = 0;	
	//dp = dtable;
	db = dp.getDb();
	newFiles = dp.retrieveFiles();
	
	for (File x : newFiles)   
	{
		map[x] = i;
		i++;
	}
	verticesCount = i;
	adjacencyMatrix = new std::list<int>[verticesCount];
	createMatrix();

	mMap = map;

	vertices_no = (int)mMap.size();
	adj_matrix = returnAdjMatrix();
	FindStrongComponents();
}

//<---------------------- Strongly Connected Graph construction ------------------->
void Strongcomponents::FindStrongComponents()
{
	try {
		int *scc_disc = new int[vertices_no];
		int *scc_low = new int[vertices_no];
		bool *sBool = new bool[vertices_no];
		std::stack<int> *scc_stack = new std::stack<int>();

		for (int i = 0; i < vertices_no; i++)
		{
			scc_disc[i] = -1;
			scc_low[i] = -1;
			sBool[i] = false;
		}

		for (int i = 0; i < vertices_no; i++)
			if (scc_disc[i] == -1)
				FindStrongComponentsR(i, scc_disc, scc_low, scc_stack, sBool);

		delete[] scc_disc;
		delete[] scc_low;
		delete[] sBool;
		delete scc_stack;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

}

//<---------------------- SCC Recursive function ------------------->
void Strongcomponents::FindStrongComponentsR(int u, int scc_disc[], int scc_low[], std::stack<int>* scc_stack, bool sBool[])
{
	try {
		static int time = 0;

		scc_disc[u] = scc_low[u] = ++time;
		scc_stack->push(u);
		sBool[u] = true;

		std::list<int>::iterator i;
		for (i = adj_matrix[u].begin(); i != adj_matrix[u].end(); ++i)
		{
			int v = *i;
			if (scc_disc[v] == -1)
			{
				FindStrongComponentsR(v, scc_disc, scc_low, scc_stack, sBool);
				scc_low[u] = min(scc_low[u], scc_low[v]);
			}

			else if (sBool[v] == true)
				scc_low[u] = min(scc_low[u], scc_disc[v]);
		}

		int w = 0;
		if (scc_low[u] == scc_disc[u])
		{
			while (scc_stack->top() != u)
			{
				w = (int)scc_stack->top();
				temp.push_back(Mapback(w));				
				sBool[w] = false;
				scc_stack->pop();
			}
			w = (int)scc_stack->top();
			temp.push_back(Mapback(w));
			scc_matrix[a] = temp;
			a++;
			temp.clear();			
			sBool[w] = false;
			scc_stack->pop();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}

//<---------------------- Mapping back from int back to file path ------------------->
std::string Strongcomponents::Mapback(int w)
{
	for (auto i : mMap)
	{
		if (i.second == w)
			return i.first;
	}
	return "Null";
}

//<---------------------- Returns the Strongly connected matrix ------------------->
std::unordered_map<int, std::vector<std::string>>& Strongcomponents::returnMatrix()
{
	return scc_matrix;
}

//<---------------------- Persisting Scc graph into XML ------------------->
std::string Strongcomponents::sccToXml(std::string filename)
{
	std::string xml;
	XmlDocument document;
	sptr pRoot = makeTaggedElement("Strongly Connected Components");
	document.docElement() = pRoot;
	int i = 1;
	for (auto x : scc_matrix)
	{
		sptr pKey = makeTaggedElement("Strong Component");
		pRoot->addChild(pKey);
		for (auto xx : x.second)
		{
			sptr px = makeTaggedElement("File");
			pKey->addChild(px);
			sptr pxkey = makeTextElement(xx);
			px->addChild(pxkey);
		}
	}
	
	xml = document.toString();	
	try
	{
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


#ifdef TEST_SCC
// Test stub
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

	DependencyTable dp_temp(tb.returnTypeTable(), allfilePaths);
	dp_temp.DepAnal();

	StronglyConnectedComps scc_temp;
	scc_temp.ConstructGraph(dp_temp);

	for (auto toD : scc_temp.returnMatrix())
	{
		for (auto xx : toD.second)
		{
			std::cout << xx << "\t";
		}
		std::cout << "\n";
	}

}

#endif


