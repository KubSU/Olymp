#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

typedef double WeightType;
typedef char StateType;

struct TVertex
{
	TVertex(){}

	TVertex(int index, string name, WeightType weight, bool boolPar)
	:Index(index), Name(name), Weight(weight), BoolPar(boolPar)
	{}

	int 		Index;
	string 		Name;
	WeightType 	Weight;
	bool 		BoolPar;
};

struct TEdge
{
	TEdge(int aimVertexIndex, WeightType weight, StateType state)
	:State(state), AimVertexIndex(aimVertexIndex), Weight(weight) 
	{}

	StateType 	State;
	int 		AimVertexIndex;
	WeightType 	Weight;
};

typedef vector< vector<TEdge> > Graph;

vector<TVertex> vertexInfo;
vector < vector<int> > ResultVertexes;
Graph graph;

int WideSearch (int vertexIndex)
{  
	vector<int> CurrentLevel, NextLevel;
	ResultVertexes=vector< vector<int> >(); 
   CurrentLevel=vector<int>();
   NextLevel=vector<int>();
   
   CurrentLevel.push_back(vertexIndex);
   vertexInfo[vertexIndex].BoolPar=true;
   while (CurrentLevel.size()>0)
   { 
	 ResultVertexes.push_back(CurrentLevel);
     for (int i=0; i<CurrentLevel.size(); i++)
     {
	   for (int j=0; j<graph[CurrentLevel[i]].size(); j++)
	   {
		   if (! vertexInfo[graph[CurrentLevel[i]][j].AimVertexIndex].BoolPar) 
		   {
//			    ResultVertexes.push_back(graph[CurrentLevel[i]][j].AimVertexIndex);
				NextLevel.push_back(graph[CurrentLevel[i]][j].AimVertexIndex);		
				vertexInfo[graph[CurrentLevel[i]][j].AimVertexIndex].BoolPar=true;
		   } 
	    }
     }
     CurrentLevel=NextLevel;
     NextLevel.clear();
   }
   for (int i=0; i<ResultVertexes.size(); i++)
   {
	   for (int j=0; j<ResultVertexes[i].size(); j++)
	   {
			cout << ResultVertexes[i][j] << " ";
	   }
	   cout << endl;
   }
   ResultVertexes.clear();
   return 0;
}

int CreateGraph(int vertexCount)
{
	graph = Graph (vertexCount);
	for(int i=0; i<vertexCount; i++)
		graph[i] = vector<TEdge>();
	vertexInfo = vector<TVertex>(vertexCount);
	for(int i=0; i<vertexCount; i++)
		vertexInfo[i] = TVertex(i, "", 0.0, false);
	return 0;
}

int AddEdge(int v1, int v2, WeightType weight)
{
	graph[v1].push_back(TEdge(v2, weight, 0));
	graph[v2].push_back(TEdge(v1, weight, 0));
	return 0;
}

int SetVertex(int index, string name, WeightType weight, bool boolPar)
{
	vertexInfo[index].Name = name;
	vertexInfo[index].Weight = weight;
	vertexInfo[index].BoolPar = boolPar;
	return 0;
}


vector<int> DepthRezultVertexes;

int DepthSearch(int vertexIndex)
{   vector<int> Stack;
	DepthRezultVertexes = vector<int>();
	Stack = vector<int>();
	Stack.push_back(vertexIndex);
	vertexInfo[vertexIndex].BoolPar = true;
	while (Stack.size() > 0)
	{		
		int CurrentVertex = Stack[Stack.size()-1];	
		Stack.pop_back();
		DepthRezultVertexes.push_back(CurrentVertex);
		int CurrentVertexWeight = graph[CurrentVertex].size();
		for (int i = 0; i < CurrentVertexWeight; i++)
		{
			if (! vertexInfo[graph[CurrentVertex][i].AimVertexIndex].BoolPar)
			{				
				Stack.push_back(graph[CurrentVertex][i].AimVertexIndex);
				vertexInfo[graph[CurrentVertex][i].AimVertexIndex].BoolPar = true;
			}
		}
	}

	for (int i = 0; i < DepthRezultVertexes.size(); i++)
		cout << DepthRezultVertexes[i] << " ";
	DepthRezultVertexes.clear();
	return 0;
}

int MarkEdge(int v1, int v2, StateType stateValue)
{
	for(int i=0; i<graph[v1].size(); i++)
	{
		if(graph[v1][i].AimVertexIndex == v2)
		{
			graph[v1][i].State = stateValue;
			break;
		}
	}
	for(int i=0; i<graph[v2].size(); i++)
	{
		if(graph[v2][i].AimVertexIndex == v1)
		{
			graph[v2][i].State = stateValue;
			break;
		}
	}
	return 0;
}

int BasedTreeSearch(int vertexIndex)
{   stack< pair<int, int> > Stack;
	DepthRezultVertexes = vector<int>();
	Stack = stack< pair<int,int> >();
	//Stack.push(vertexIndex);
	vertexInfo[vertexIndex].BoolPar = true;
	for(int i = 0; i<graph[vertexIndex].size(); i++)
	{
		Stack.push(make_pair(vertexIndex, graph[vertexIndex][i].AimVertexIndex));
	}
	pair<int, int> p;
	while (Stack.size() > 0)
	{
		p = Stack.top();
		Stack.pop();
		if(!vertexInfo[p.second].BoolPar)
		{
			vertexInfo[p.second].BoolPar = true;
			MarkEdge(p.first, p.second, 1);
			for(int i = 0; i<graph[p.second].size(); i++)
			{
				Stack.push(make_pair(p.second, graph[p.second][i].AimVertexIndex));
			}
		}
	}
	return 0;
}

int ConditionWideSearch (int vertexIndex, int toVertex, vector <int> &result)
{  vector< vector< pair<int, int> > > ResultVertexes=vector< vector< pair<int, int> > >(); 
   vector< pair<int, int> > CurrentLevel=vector< pair<int, int> >();
   vector< pair<int, int> > NextLevel=vector< pair<int, int> >();

   for (int i = 0; i < vertexInfo.size(); i++)
	   vertexInfo[i].BoolPar = false;

   CurrentLevel.push_back(make_pair(vertexIndex, -1));
   vertexInfo[vertexIndex].BoolPar = true;
   int i;
   while (CurrentLevel.size()>0)
   { 
	 ResultVertexes.push_back(CurrentLevel);
     for (i=0; i<CurrentLevel.size(); i++)
     {
		 if (CurrentLevel[i].first == toVertex)
		 {
			 CurrentLevel.clear();
			 break;
		 }
		for (int j=0; j<graph[CurrentLevel[i].first].size(); j++)
	    {
		   if (graph[CurrentLevel[i].first][j].State < 3
			   && !vertexInfo[graph[CurrentLevel[i].first][j].AimVertexIndex].BoolPar) 
		   {
			   vertexInfo[graph[CurrentLevel[i].first][j].AimVertexIndex].BoolPar=true;
			   NextLevel.push_back(make_pair(graph[CurrentLevel[i].first][j].AimVertexIndex, i));
		   } 
	    }
     }
	 if (CurrentLevel.empty())
		 break;
	// ResultVertexes.push_back(NextLevel);		   
     CurrentLevel=NextLevel;
     NextLevel.clear();
   }
   result.clear();
   for (int j=ResultVertexes.size() -1; j>=0; j--)
   {
	   result.push_back(ResultVertexes[j][i].first);
	   i = ResultVertexes[j][i].second;
   }

   /*for (i = 0; i < ResultVertexes.size(); i++)
   {
	   for (int j = 0; j < ResultVertexes[i].size(); j++)
		   cout << '(' << ResultVertexes[i][j].first <<';'<<ResultVertexes[i][j].second << ") ";
	   cout << endl;
   }*/

   ResultVertexes.clear();
   CurrentLevel.clear();
   return 0;
}



int BaseCircleSearch()
{
	vector <int> result;
	for (int i = 0; i < graph.size(); i++)
		for (int j = 0; j < graph[i].size(); j++)
		{
			if (graph[i][j].State == 0)
			{
				graph[i][j].State = 3;

				int k;
				for (k = 0; k < graph[graph[i][j].AimVertexIndex].size() 
					&& graph[graph[i][j].AimVertexIndex][k].AimVertexIndex != i; k++)
					;
				graph[graph[i][j].AimVertexIndex][k].State = 3;
				result.clear();
				ConditionWideSearch(i, graph[i][j].AimVertexIndex, result);
				graph[i][j].State = 2;
				graph[graph[i][j].AimVertexIndex][k].State = 2;
				for (k = 0; k < result.size(); k++)
					cout << result[k] << ' ';
				cout << endl;
			}
		}
	return 0;
}

int ConstGraph()
{
	int N;
	N = 7;
	CreateGraph(N);
	AddEdge(0,1,1);
	AddEdge(0,2,1);
	AddEdge(1,2,1);
	AddEdge(1,3,1);
	AddEdge(1,4,1);
	AddEdge(2, 5, 1);
	AddEdge(2,6,1);
	AddEdge(3,4,1);
	AddEdge(5,6,1);
	AddEdge(4,5,1);
	return 0;
}

int InputGraph()
{
	setlocale(0,"");
	cout<<"Количество вершин=";
	int N;
	cin>>N;
	CreateGraph(N);
	cout<<"Инициализировать вершины?y/n";
	char choose;
	cin>>choose;
	if(choose=='y')
	{
		for(int i=0; i<N; i++)
		{
			cout<<"Имя вершины: ";
			cin>>vertexInfo[i].Name;
		}
	}
	cout<<"Ввод ребра?y/n";
	cin>>choose;
	int i1, i2;
	WeightType w;
	while(choose=='y')
	{
		cout<<"Index1,Index2,Weight:";
		cin>>i1>>i2>>w;
		AddEdge(i1, i2, w);
		cout<<"Ввод ребра?y/n";
		cin>>choose;
	}
	return 0;
}

int main()
{
	//InputGraph();
	ConstGraph();
	//WideSearch(0);
	BasedTreeSearch(0);
	BaseCircleSearch();
	//system("PAUSE");
	return 0;
}

