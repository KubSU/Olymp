#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef double WeightType;
typedef char StateType;

struct Vertex {
    Vertex() {}

    Vertex(int index, string name, WeightType weight, bool boolPar) {
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

struct Connected {
    Connected(int secondVertexIndex, WeightType weight, StateType state) {
        this->secondVertexIndex = secondVertexIndex;
        this->Weight = weight;
        this->state = state;
    }
    StateType state;
    int secondVertexIndex;
    WeightType Weight;
};

struct comparer {
    bool operator()(const Connected &x, const Connected &y) const {
        return x.secondVertexIndex < y.secondVertexIndex;
    }
};


typedef vector< set<Connected, comparer> > Graph;

vector<Vertex> vertexInfo;
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
    while (CurrentLevel.size()>0) {
        ResultVertexes.push_back(CurrentLevel);
        for (int i=0; i<CurrentLevel.size(); i++) {
            for (set<Connected>::iterator j=graph[CurrentLevel[i]].begin(); j != graph[CurrentLevel[i]].end(); j++) {
                if (! vertexInfo[(*j).secondVertexIndex].BoolPar) {
//			    ResultVertexes.push_back(graph[CurrentLevel[i]][j].secondVertexIndex);
                    NextLevel.push_back((*j).secondVertexIndex);
                    vertexInfo[(*j).secondVertexIndex].BoolPar=true;
                }
            }
        }
        CurrentLevel=NextLevel;
        NextLevel.clear();
    }
    for (int i=0; i<ResultVertexes.size(); i++) {
        for (int j=0; j<ResultVertexes[i].size(); j++) {
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
        graph[i] = set<Connected, comparer>();
    vertexInfo = vector<Vertex>(vertexCount);
    for(int i=0; i<vertexCount; i++)
        vertexInfo[i] = Vertex(i, "", 0.0, false);
    return 0;
}

int AddEdge(int v1, int v2, WeightType weight)
{
    graph[v1].insert(Connected(v2, weight, 0));
    graph[v2].insert(Connected(v1, weight, 0));
    return 0;
}

int setVertex(int index, string name, WeightType weight, bool boolPar)
{
    vertexInfo[index].Name = name;
    vertexInfo[index].Weight = weight;
    vertexInfo[index].BoolPar = boolPar;
    return 0;
}


vector<int> DepthRezultVertexes;

int DepthSearch(int vertexIndex)
{
    stack<int> Stack;
    DepthRezultVertexes = vector<int>();
    Stack = stack<int>();
    Stack.push(vertexIndex);
    vertexInfo[vertexIndex].BoolPar = true;
    int CurrentVertex;
    while (Stack.size() > 0) {
        CurrentVertex = Stack.top();
        Stack.pop();
        DepthRezultVertexes.push_back(CurrentVertex);
        for (set <Connected>::iterator i = graph[CurrentVertex].begin(); i != graph[CurrentVertex].end(); i++) {
            if (! vertexInfo[(*i).secondVertexIndex].BoolPar) {
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

int markEdge(int v1, int v2, StateType stateValue, set<Connected>::iterator &edge1, set<Connected>::iterator &edge2)
{
    for(set <Connected>::iterator i=graph[v1].begin(); i != graph[v1].end(); i++) {
        if((*i).secondVertexIndex == v2) {
            Connected c = *i;
            c.state = stateValue;
            graph[v1].erase(i);
            edge1 = graph[v1].insert(c).first;
            break;
        }
    }
    for(set<Connected>::iterator i=graph[v2].begin(); i != graph[v2].end(); i++) {
        if((*i).secondVertexIndex == v1) {
            Connected c = *i;
            c.state = stateValue;
            graph[v2].erase(i);
            edge2 = graph[v2].insert(c).first;
            break;
        }
    }
    return 0;
}

int BasedTreeSearch(int vertexIndex)
{
    stack< pair<int, int> > Stack;
    DepthRezultVertexes = vector<int>();
    Stack = stack< pair<int,int> >();
    //Stack.push(vertexIndex);
    vertexInfo[vertexIndex].BoolPar = true;
    for(set <Connected>::iterator i = graph[vertexIndex].begin(); i != graph[vertexIndex].end(); i++) {
        Stack.push(make_pair(vertexIndex, (*i).secondVertexIndex));
    }
    pair<int, int> p;
    set <Connected>::iterator edge;
    while (Stack.size() > 0) {
        p = Stack.top();
        Stack.pop();
        if(!vertexInfo[p.second].BoolPar) {
            vertexInfo[p.second].BoolPar = true;
            markEdge(p.first, p.second, 1, edge, edge);
            for(set <Connected>::iterator i = graph[p.second].begin(); i != graph[p.second].end(); i++) {
                Stack.push(make_pair(p.second, (*i).secondVertexIndex));
            }
        }
    }
    return 0;
}

int ConditionWideSearch (int vertexIndex, int toVertex, vector <int> &result)
{
    vector< vector< pair<int, int> > > ResultVertexes=vector< vector< pair<int, int> > >();
    vector< pair<int, int> > CurrentLevel=vector< pair<int, int> >();
    vector< pair<int, int> > NextLevel=vector< pair<int, int> >();

    for (int i = 0; i < vertexInfo.size(); i++)
        vertexInfo[i].BoolPar = false;

    CurrentLevel.push_back(make_pair(vertexIndex, -1));
    vertexInfo[vertexIndex].BoolPar = true;
    int i;
    while (CurrentLevel.size()>0) {
        ResultVertexes.push_back(CurrentLevel);
        for (i=0; i<CurrentLevel.size(); i++) {
            if (CurrentLevel[i].first == toVertex) {
                CurrentLevel.clear();
                break;
            }
            for (set <Connected>::iterator j=graph[CurrentLevel[i].first].begin(); j != graph[CurrentLevel[i].first].end(); j++) {
                if ((*j).state < 3
                        && !vertexInfo[(*j).secondVertexIndex].BoolPar) {
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
    for (int j=ResultVertexes.size() -1; j>=0; j--) {
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
        for (set <Connected>::iterator j = graph[i].begin(); j != graph[i].end(); j++) {
            if ((*j).state == 0) {
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
    if(choose=='y') {
        for(int i=0; i<N; i++) {
            cout<<"Имя вершины: ";
            cin>>vertexInfo[i].Name;
        }
    }
    cout<<"Ввод ребра?y/n";
    cin>>choose;
    int i1, i2;
    WeightType w;
    while(choose=='y') {
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
    BasedTreeSearch(5);
    BaseCircleSearch();
    //system("PAUSE");
    return 0;
}