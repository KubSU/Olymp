//#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef double WeightType;
typedef char StateType;

struct Vertex
{
    Vertex(){}

    Vertex(int index, string name, WeightType weight, bool boolPar)
    {
        this->Index = index;
        this->Name = name;
        this->Weight = weight;
        this->BoolPar = boolPar;
    }
    int Index;
    string Name;
    WeightType Weight;
    bool BoolPar;
};

struct Connected
{
    Connected(int secondVertexIndex, WeightType weight, StateType state)
    {
        this->secondVertexIndex = secondVertexIndex;
        this->Weight = weight;
        this->state = state;
        this->Taken = 0;
    }
    StateType state;
    int secondVertexIndex;
    WeightType Weight;
    WeightType Taken; 
};

struct comparer
{
    bool operator()(const Connected &x, const Connected &y) const
    {
        return x.secondVertexIndex < y.secondVertexIndex;
    }
};

typedef pair <int, WeightType> dijkstra_elem;

struct dijkstra_comparer
{
	bool operator()(const dijkstra_elem &x, const dijkstra_elem &y) const
	{
		return x.second < y.second;
	}
};

typedef vector< set<Connected, comparer> > Graph;

vector<Vertex> vertexInfo;
vector < vector<int> > ResultVertexes;
Graph graph, graph1;


int AddEdge(Graph *graph, int v1, int v2, WeightType weight, bool orient)
{
    (*graph)[v1].insert(Connected(v2, weight, 0));
    if (!orient)
		(*graph)[v2].insert(Connected(v1, weight, 0));
    return 0;
}

int setVertex(int index, string name, WeightType weight, bool boolPar)
{
    vertexInfo[index].Name = name;
    vertexInfo[index].Weight = weight;
    vertexInfo[index].BoolPar = boolPar;
    return 0;
}


int CreateGraph(int vertexCount)
{
    graph = Graph (vertexCount);
    for(int i=0; i<vertexCount; i++)
        graph[i] = set<Connected, comparer>();
	graph1 = Graph (vertexCount);
    for(int i=0; i<vertexCount; i++)
        graph1[i] = set<Connected, comparer>();
    
    vertexInfo = vector<Vertex>(vertexCount);
    for(int i=0; i<vertexCount; i++)
        vertexInfo[i] = Vertex(i, "", 0.0, false);
    return 0;
}


int markEdge(int v1, int v2, StateType stateValue, set<Connected>::iterator &edge1, set<Connected>::iterator &edge2)
{
    for(set <Connected>::iterator i=graph[v1].begin(); i != graph[v1].end(); i++)
    {
        if((*i).secondVertexIndex == v2)
        {
            Connected c = *i;
            c.state = stateValue;
            graph[v1].erase(i);
			edge1 = graph[v1].insert(c).first;
            break;
        }
    }
    for(set<Connected>::iterator i=graph[v2].begin(); i != graph[v2].end(); i++)
    {
        if((*i).secondVertexIndex == v1)
        {
            Connected c = *i;
            c.state = stateValue;
            graph[v2].erase(i);
			edge2 = graph[v2].insert(c).first;
            break;
        }
    }
    return 0;
}


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
         for (set<Connected>::iterator j=graph[CurrentLevel[i]].begin(); j != graph[CurrentLevel[i]].end(); j++)
        {
             if (! vertexInfo[(*j).secondVertexIndex].BoolPar)
           {
                NextLevel.push_back((*j).secondVertexIndex);
                vertexInfo[(*j).secondVertexIndex].BoolPar=true;
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


vector<int> DepthRezultVertexes;

int DepthSearch(int vertexIndex)
{   stack<int> Stack;
    DepthRezultVertexes = vector<int>();
    Stack = stack<int>();
    Stack.push(vertexIndex);
    vertexInfo[vertexIndex].BoolPar = true;
	int CurrentVertex;
    while (Stack.size() > 0)
    {
        CurrentVertex = Stack.top();
        Stack.pop();
        DepthRezultVertexes.push_back(CurrentVertex);
        for (set <Connected>::iterator i = graph[CurrentVertex].begin(); i != graph[CurrentVertex].end(); i++)
        {
            if (! vertexInfo[(*i).secondVertexIndex].BoolPar)
            {
                Stack.push((*i).secondVertexIndex);
                vertexInfo[(*i).secondVertexIndex].BoolPar = true;
            }
        }
    }

    for (int i = 0; i < DepthRezultVertexes.size(); i++)
        cout << DepthRezultVertexes[i] << " ";
    DepthRezultVertexes.clear();
    return 0;
}


int BasedTreeSearch(int vertexIndex)
{   stack< pair<int, int> > Stack;
    DepthRezultVertexes = vector<int>();
    Stack = stack< pair<int,int> >();
    //Stack.push(vertexIndex);
    vertexInfo[vertexIndex].BoolPar = true;
    for(set <Connected>::iterator i = graph[vertexIndex].begin(); i != graph[vertexIndex].end(); i++)
    {
        Stack.push(make_pair(vertexIndex, (*i).secondVertexIndex));
    }
    pair<int, int> p;
	set <Connected>::iterator edge;
    while (Stack.size() > 0)
    {
        p = Stack.top();
        Stack.pop();
        if(!vertexInfo[p.second].BoolPar)
        {
            vertexInfo[p.second].BoolPar = true;
			markEdge(p.first, p.second, 1, edge, edge);
            for(set <Connected>::iterator i = graph[p.second].begin(); i != graph[p.second].end(); i++)
            {
                Stack.push(make_pair(p.second, (*i).secondVertexIndex));
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
         for (set <Connected>::iterator j=graph[CurrentLevel[i].first].begin(); j != graph[CurrentLevel[i].first].end(); j++)
        {
           if ((*j).state < 3
               && !vertexInfo[(*j).secondVertexIndex].BoolPar)
           {
               vertexInfo[(*j).secondVertexIndex].BoolPar=true;
               NextLevel.push_back(make_pair((*j).secondVertexIndex, i));
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
	set<Connected>::iterator falseEdge;
    for (int i = 0; i < graph.size(); i++)
        for (set <Connected>::iterator j = graph[i].begin(); j != graph[i].end(); j++)
        {
            if ((*j).state == 0)
            {
				markEdge(i, (*j).secondVertexIndex, 3, j, falseEdge);

                result.clear();
                ConditionWideSearch(i, (*j).secondVertexIndex, result);

                //markEdge(i, (*j).secondVertexIndex, 2, j, falseEdge);

                for (int l = 0; l < result.size(); l++)
                    cout << result[l] << ' ';
                cout << endl;
            }
        }
    return 0;
}

int dijkstra(int vertexIndex)
{
	for (int i = 0; i < vertexInfo.size(); i++)
	{
		vertexInfo[i].Weight = -1;
		vertexInfo[i].BoolPar = false;
	}
	vertexInfo[vertexIndex].Weight = 0;

	set <dijkstra_elem> toCheck;
	toCheck.insert(make_pair(vertexIndex, 0));

	dijkstra_elem current;
	while (!toCheck.empty())
	{
		current = *toCheck.begin();
		vertexInfo[current.first].BoolPar = true;
		toCheck.erase(toCheck.begin());

		for (set<Connected>::iterator i = graph[current.first].begin();
			i != graph[current.first].end(); i++)
			if (!vertexInfo[(*i).secondVertexIndex].BoolPar)
			{
				if (vertexInfo[(*i).secondVertexIndex].Weight == -1)
				{
					vertexInfo[(*i).secondVertexIndex].Weight = current.second + 
						(*i).Weight;
					toCheck.insert(make_pair((*i).secondVertexIndex,
						vertexInfo[(*i).secondVertexIndex].Weight));
				}
				else
				{
					if (vertexInfo[(*i).secondVertexIndex].Weight > current.second +
						(*i).Weight)
					{
						set <dijkstra_elem>::iterator temp = 
							toCheck.find(make_pair((*i).secondVertexIndex,
							vertexInfo[(*i).secondVertexIndex].Weight));
						toCheck.erase(temp);
						vertexInfo[(*i).secondVertexIndex].Weight = current.second +
						(*i).Weight;
						toCheck.insert(make_pair((*i).secondVertexIndex,
						vertexInfo[(*i).secondVertexIndex].Weight));
					}
				}
			}
	}

	for (int i = 0; i < vertexInfo.size(); i++)
		cout << i << " " << vertexInfo[i].Weight << endl;

	return 0;
}

int FindBridges()
{   
	int treeSize = 0;
	stack< pair<int, int> > Stack;
	vector <int> marks(graph.size());
	vector <int> parent(graph.size());
	vector <int> sMarks(graph.size());

	for (int i = 0; i < graph.size(); i++)
		marks[i] = parent[i] = -1;

    Stack = stack< pair<int,int> >();
    //Stack.push(vertexIndex);
    vertexInfo[0].BoolPar = true;
    marks[0] = treeSize++;
    sMarks[0] = marks[0];
    for(set <Connected>::iterator i = graph[0].begin(); i != graph[0].end(); i++)
    {
        Stack.push(make_pair(0, (*i).secondVertexIndex));
    }
    pair<int, int> p;
	set <Connected>::iterator edge;
    while (Stack.size() > 0)
    {
        p = Stack.top();
        Stack.pop();
        if(!vertexInfo[p.second].BoolPar)
        {
            vertexInfo[p.second].BoolPar = true;
			marks[p.second] = treeSize++;
            sMarks[p.second] = marks[p.second];
			parent[p.second] = p.first;
			markEdge(p.first, p.second, 1, edge, edge);
			int currentFirst = - 1;
            for(set <Connected>::iterator i = graph[p.second].begin(); i != graph[p.second].end(); i++)
            {
				if (marks[(*i).secondVertexIndex] != -1 &&
					(*i).secondVertexIndex != p.first && (
					currentFirst == -1 || 
					marks[currentFirst] > marks[(*i).secondVertexIndex]))
					currentFirst = (*i).secondVertexIndex;
                Stack.push(make_pair(p.second, (*i).secondVertexIndex));
            }
			if (currentFirst != -1)
			{
				int currentIndex = p.second;
				while (currentIndex != currentFirst)
				{
					marks[currentIndex] = marks[currentFirst];
					currentIndex = parent[currentIndex];
				}
			}
        }
    }
/*
	for (int i = 0; i < graph.size(); i++)
	{
    	cout << i << " (" << sMarks[i] << ") " << marks[i] << endl;
        if (marks[parent[i]] < marks[i]) 
            cout << "Мост: " << parent[i] << " " << i << endl;
    }*/
    int Z = 0;
    for (int i = 1; i < graph.size(); i++)
    {
        bool p = true;
        for (set <Connected>::iterator j=graph[i].begin(); p && j != graph[i].end(); j++)
        {
            p = sMarks[i] > marks[(*j).secondVertexIndex];
        } 
        Z += parent[i] == 0;   
        if (!p)
            cout << i << endl;
    }
    if (Z > 1) 
        cout << 0 << endl;
    return 0;
}

WeightType** TableGraph;
int ConverterGraph()
{
    TableGraph = new WeightType*[graph.size()];
    for (int i = 0; i < graph.size(); i++)
        TableGraph[i] = new WeightType[graph.size()];
    for (int i = 0; i < graph.size(); i++)
    {
        for (set <Connected>::iterator j=graph[i].begin(); j != graph[i].end(); j++)
        {
            TableGraph[i][(*j).secondVertexIndex] = (*j).Weight;
        } 
    }
    return 0;
}

int Floyd()
{
    ConverterGraph();
    for (int m = 0; m < graph.size(); m++)
        for (int i = 0; i < graph.size(); i++)
            for (int j = 0; j < graph.size(); j++)
                if (TableGraph[i][m] + TableGraph[m][j] < TableGraph[i][j])
                    TableGraph[i][j] = TableGraph[i][m] + TableGraph[m][j];
    //TODO: Проверить!!!
    return 0;
}

WeightType GetEdgeWeight (Graph &graph, int v1, int v2) 
{
	for (set<Connected>::iterator i = graph[v1].connected.begin(); i != graph[v1].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v2) {
			return (*i).Weight;
		}
	} 
	
	for (set<Connected>::iterator i = graph[v2].connected.begin(); i != graph[v2].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v1) {
			return -(*i).Weight;
		}
	}
}	

WeightType GetEdgeTaken (Graph &graph, int v1, int v2) 
{
	for (set<Connected>::iterator i = graph[v1].connected.begin(); i != graph[v1].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v2) {
			return (*i).Taken;
		}
	} 
	
	for (set<Connected>::iterator i = graph[v2].connected.begin(); i != graph[v2].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v1) {
			return -(*i).Taken;
		}
	}
}	

int AddEdgeTaken (Graph &graph, int v1, int v2, WeightType Taken) 
{
	for (set<Connected>::iterator i = graph[v1].connected.begin(); i != graph[v1].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v2) {
			(*i).Taken += Taken;
			return 0;
		}
	} 
	
	for (set<Connected>::iterator i = graph[v2].connected.begin(); i != graph[v2].connected.end(); i++)  
	{
		if ((*i).secondVertexIndex == v1) {
			(*i).Taken -= Taken;
			return 0;
		}
	}
	
	return 1;

}

int *path = new int[graph.size()];

int CalcPath(int n)
{
	WeightType q = 1000000, w, t; // TODO MAXINT
	
	for (int i = 1; i <= n; i++) {
		w = GetEdgeWeight(graph, path[i - 1], path[i]);
		t = GetEdgeTaken(graph, path[i - 1], path[i]);
		if (w > 0) { 
			q = (q > w - t) ? w - t : q;
		} else {
			q = q > abs(t) ? abs(t) : q;
		}
	}
	
	if (q > 0) {
		for (int i = 1; i <= n; i++) {
			AddEdgeTaken(graph, path[i-1], path[i], q);
		}
	}
}

int findAllPaths(Graph &graph, int s, int t)
{
	stack < pair <int, int> > st;

	st.push(make_pair(s, 0));
	pair <int, int> current;
	set<Connected>::iterator i;
	while (!st.empty())
	{
		current = st.top();
		st.pop();

		vertexInfo[current.first].BoolPar = true;
		
		if (current.first == t)
		{	// Путь найден, находится в Path
			CalcPath(st.size());
			/*for (int i = 0; i <= st.size(); i++)
			{
				cout << path[i] << ' ';
			}*/
	//		system("PAUSE");
			//cout << endl;
			vertexInfo[current.first].BoolPar = false;
		}
		else
		{
			i = graph[current.first].begin();
			int j;
			for (j = 0; j < current.second; j++)
			{
				i++;
			}
			for (;j < graph[current.first].size(); j++)
			{
				if (!vertexInfo[(*i).secondVertexIndex].BoolPar)
				{
					current.second = j + 1;
					path[st.size()] = current.first; 
					st.push(current);
					path[st.size()] = (*i).secondVertexIndex; 
					st.push(make_pair((*i).secondVertexIndex, 0));
					break;
				}

				i++;
			}
			if (j == graph[current.first].size())
			{
				vertexInfo[current.first].BoolPar = false;
			}
		}
	}

	return 0;
}

int ConstGraph()
{
    int N;
    N = 9;
    CreateGraph(N);
    AddEdge(&graph, 0,1,2, true);
	AddEdge(&graph, 1,2,3, true);
	AddEdge(&graph, 1,4,2, true);
	AddEdge(&graph, 3,2,4, true);
	AddEdge(&graph, 0,3,5, true);
	AddEdge(&graph, 0,7,8, true);
	AddEdge(&graph, 7,6,1, true);
	AddEdge(&graph, 7,4,2, true);
	AddEdge(&graph, 3,7,3, true);
	AddEdge(&graph, 2,4,5, true);
	AddEdge(&graph, 4,5,4, true);
	AddEdge(&graph, 2,6,8, true);
	AddEdge(&graph, 5,8,3, true);
	AddEdge(&graph, 6,5,2, true);
	AddEdge(&graph, 6,8,3, true);

	AddEdge(&graph1, 0,1,2, false);
	AddEdge(&graph1, 1,2,3, false);
	AddEdge(&graph1, 1,4,2, false);
	AddEdge(&graph1, 3,2,4, false);
	AddEdge(&graph1, 0,3,5, false);
	AddEdge(&graph1, 0,7,8, false);
	AddEdge(&graph1, 7,6,1, false);
	AddEdge(&graph1, 7,4,2, false);
	AddEdge(&graph1, 3,7,3, false);
	AddEdge(&graph1, 2,4,5, false);
	AddEdge(&graph1, 4,5,4, false);
	AddEdge(&graph1, 2,6,8, false);
	AddEdge(&graph1, 5,8,3, false);
	AddEdge(&graph1, 6,5,2, false);
	AddEdge(&graph1, 6,8,3, false);

    
    return 0;
}
/*
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
*/


int main()
{
	// TODO : Удалить set из connected graph
	// Организовать вывод найденного потока
	
    //InputGraph();
    ConstGraph();
	//dijkstra(0);
	//FindBridges();
	findAllPaths(graph1, 0, 8);
    //WideSearch(5);
   // BasedTreeSearch(5);
    //BaseCircleSearch();
//    system("PAUSE");
    return 0;
}
