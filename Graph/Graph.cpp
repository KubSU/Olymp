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
	EStateType		State;
	vector<TEdge>	Edges;

	TVertex(): Name(""), Weight(DEFAULT_WEIGHT), State(EST_UNWATCHED)  {};

	TVertex(string AName, TWeightType AWeight, EStateType AState): Name(AName), Weight(AWeight), State(AState) {};

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

	void AddVertex(TVertex AVertex)
	{
		// TODO: Проверить правильность.
		_Vertexes.push_back(AVertex);
	};

	void AddVertex(string AName, TWeightType AWeight, EStateType AWatched)
	{
		_Vertexes.push_back(TVertex(AName, AWeight, AWatched));
	};

	void Clear()
	{
		_Vertexes.clear();
	};

	void ClearStates()
	{
		for (int i = 0; i < _Vertexes.size(); i++)
			_Vertexes[i].State = EST_UNWATCHED;
	};

	void ConstGraph()
	{
		int vertexCount = 7;
		for (int i = 0; i < vertexCount; i++) AddVertex();
		AddEdge(0, 1, DEFAULT_WEIGHT);
		AddEdge(0, 2, DEFAULT_WEIGHT);
		AddEdge(1, 2, DEFAULT_WEIGHT);
		AddEdge(1, 3, DEFAULT_WEIGHT);
		AddEdge(1, 4, DEFAULT_WEIGHT);
		AddEdge(2, 5, DEFAULT_WEIGHT);
		AddEdge(2, 6, DEFAULT_WEIGHT);
		AddEdge(3, 4, DEFAULT_WEIGHT);
		AddEdge(5, 6, DEFAULT_WEIGHT);
		AddEdge(4, 5, DEFAULT_WEIGHT);
	};

	/// Остовное дерево (в глубину).
	/*! Получается обходом в глубину.
	 *  \param 	AStartVertexIndex Индекс вершины, с которой начинается обход.
	 *  \returns Новый граф - дерево, полученное обходом в глубину.
	 */
	CGraph SpanningDepthTree(int AStartVertexIndex)
	{
		CGraph result(_Vertexes.size());

		ClearStates();

		stack<int> vStack;

		vStack.push(AStartVertexIndex);
		_Vertexes[AStartVertexIndex].State = EST_WATCHED;

		while (!vStack.empty())
		{
			int nextVertex = -1;
			TWeightType edgeWeight;
			for (int i = 0; i < _Vertexes[vStack.top()].Edges.size() && nextVertex == -1; i++)
			{
				int nv = _Vertexes[vStack.top()].Edges[i].AimVertexIndex;
				if (_Vertexes[nv].State == EST_UNWATCHED)
				{
					nextVertex = nv;
					_Vertexes[nextVertex].State = EST_WATCHED;
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

	/// Остовное дерево (в ширину).
	/*! Получается обходом в ширину.
	 *  \param 	AStartVertexIndex Индекс вершины, с которой начинается обход.
	 *  \returns Новый граф - дерево, полученное обходом в ширину.
	 */
	CGraph SpanningWideTree(int AStartVertexIndex)
	{
		CGraph result(_Vertexes.size());

		ClearStates();

		vector<int> currentLayer;
		vector<int> nextLayer;

		nextLayer.push_back(AStartVertexIndex);
		_Vertexes[AStartVertexIndex].State = EST_WATCHED;

		while (!nextLayer.empty())
		{
			currentLayer = nextLayer;
			nextLayer.clear();

			for (int i = 0; i < currentLayer.size(); i++)
			{
				for (int ii = 0; ii < _Vertexes[currentLayer[i]].Edges.size(); ii++)
				{
					int nextVertexIndex = _Vertexes[currentLayer[i]].Edges[ii].AimVertexIndex;
					if (_Vertexes[nextVertexIndex].State == EST_UNWATCHED)
					{
						nextLayer.push_back(nextVertexIndex);
						_Vertexes[nextVertexIndex].State = EST_WATCHED;
						result.AddEdge(currentLayer[i], nextVertexIndex, DEFAULT_WEIGHT);
					}
				}
			}
		}
		
		return result;
	};

	/// Выводит список ребер в формате: <Начало> <Конец> <Вес> <Состояние>. Где <Начало>, <Конец> - индексы вершин.
	void WriteEdgesList()
	{
		for (int v = 0; v < _Vertexes.size(); v++)
			for (int e = 0; e < _Vertexes[v].Edges.size(); e++)
				cout << v << ' ' << _Vertexes[v].Edges[e].AimVertexIndex << ' ' << _Vertexes[v].Edges[e].Weight << _Vertexes[v].Edges[e].State << endl;
	};

	/// Выводит список вершин.
	void WriteVertexesList()
	{
		for (int v = 0; v < _Vertexes.size(); v++)
		{
			cout << v << ' ' << _Vertexes[v].Name << ' ' << _Vertexes[v].Weight << ' ' << _Vertexes[v].State << " |";
			for (int e = 0; e < _Vertexes[v].Edges.size(); e++)
				cout << ' ' << _Vertexes[v].Edges[e].AimVertexIndex;
			cout << endl;
		}
	};
};

int main()
{
	CGraph graph;
	graph.ConstGraph();
	graph.WriteVertexesList();

	cout << "Wide" << endl;
	graph.SpanningWideTree(0).WriteVertexesList();

	cout << "Depth" << endl;
	graph.SpanningDepthTree(0).WriteVertexesList();

	return 0;
}

