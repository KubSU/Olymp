#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

#define DEFAULT_WEIGHT 1

typedef double TWeightType;

enum EStateType
{
	EST_UNWATCHED = 0,
	EST_WATCHED
};

struct TEdge
{
	TEdge(int aimVertexIndex, TWeightType weight, EStateType state): State(state), AimVertexIndex(aimVertexIndex), Weight(weight) {};

	EStateType 	State;
	int 		AimVertexIndex;
	TWeightType Weight;
};

struct TVertex
{
	string 			Name;
	TWeightType		Weight;
	bool 			Watched;
	vector<TEdge>	Edges;

	TVertex(): Name(""), Weight(DEFAULT_WEIGHT), Watched(false)  {};

	TVertex(string AName, TWeightType AWeight, bool AWatched): Name(AName), Weight(AWeight), Watched(AWatched) {};

	void AddEdge(int AimVertex, TWeightType AWeight)
	{
		Edges.push_back(TEdge(AimVertex, AWeight, EST_UNWATCHED));
	};
};

class CGraph
{
	vector<TVertex> _Vertexes;
public:
	CGraph(){};

	CGraph(int AVertexCount)
	{		
		for (int i = 0; i < AVertexCount; i++) 
			AddVertex();
	};

	~CGraph(){};

	void AddEdge(int AVertexIndex1, int AVertexIndex2, TWeightType AWeight)
	{
		_Vertexes[AVertexIndex1].AddEdge(AVertexIndex2, AWeight);
		_Vertexes[AVertexIndex2].AddEdge(AVertexIndex1, AWeight);
	};

	void AddVertex()
	{
		_Vertexes.push_back(TVertex());
	};

	void AddVertex(string AName, TWeightType AWeight, bool AWatched)
	{
		_Vertexes.push_back(TVertex(AName, AWeight, AWatched));
	};

	void Clear()
	{
		_Vertexes.clear();
	};

	void ConstGraph()
	{
		int vertexCount = 7;
		for (int i = 0; i < vertexCount; i++) AddVertex();
		AddEdge(0, 1, 1);
		AddEdge(0, 2, 1);
		AddEdge(1, 2, 1);
		AddEdge(1, 3, 1);
		AddEdge(1, 4, 1);
		AddEdge(2, 5, 1);
		AddEdge(2, 6, 1);
		AddEdge(3, 4, 1);
		AddEdge(5, 6, 1);
		AddEdge(4, 5, 1);
	};

	/// Остовное дерево.
	CGraph SpanningTree()
	{
		CGraph result;

		stack<int> vStack;

		vStack.push(0);
		_Vertexes[0].Watched = true;

		while (!vStack.empty())
		{
			int nextVertex = -1;
			TWeightType edgeWeight;
			for (int i = 0; i < _Vertexes[vStack.top()].Edges.size() && nextVertex == -1; i++)
			{
				int nv = _Vertexes[vStack.top()].Edges[i].AimVertexIndex;
				if (!_Vertexes[nv].Watched)
				{
					nextVertex = nv;
					_Vertexes[nextVertex].Watched = true;
					edgeWeight = _Vertexes[vStack.top()].Edges[i].Weight;
				};
			};
			if (nextVertex == -1)
				vStack.pop();
			else
			{
				result.AddEdge(vStack.top(), nextVertex, edgeWeight);
				vStack.push(nextVertex);
			}
		};

		return result;
	};

	void WriteEdgesList()
	{
		for (int v = 0; v < _Vertexes.size(); v++)
			for (int e = 0; e < _Vertexes[v].Edges.size(); e++)
				cout << v << ' ' << _Vertexes[v].Edges[e].AimVertexIndex << endl;
	};
};

int main()
{
	CGraph graph;
	graph.ConstGraph();
	graph.WriteEdgesList();
	graph.SpanningTree().WriteEdgesList();
	return 0;
}

