#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <stack>

using namespace std;

// A class to represent an undirected graph using adjacency list representation
class Graph
{
    int V;                   // number of vertices
    vector<vector<int>> adj; // adjacency list
public:
    Graph(int V) : V(V)
    {
        adj.resize(V);
    }

    // function to add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // function to perform parallel BFS starting from a given source vertex
    void Parallel_BFS(int src)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            // get the next vertex from the queue
            int u = q.front();
            q.pop();

			// process all neighbors of the current vertex
			#pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];

				// if the neighbor has not been visited yet, mark it as visited and add it to the queue
				#pragma omp critical
                {
                    if (!visited[v])
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }

    // function to perform sequential BFS starting from a given source vertex
    void Serial_BFS(int src)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            // get the next vertex from the queue
            int u = q.front();
            q.pop();

            // process all neighbors of the current vertex
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];

                // if the neighbor has not been visited yet, mark it as visited and add it to the queue
                {
                    if (!visited[v])
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }

    void Serial_DFS(int src)
    {
        vector<bool> visited(V, false);
        stack<int> q;

        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            // get the next vertex from the queue
            int u = q.top();
            q.pop();

            // process all neighbors of the current vertex
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];

                // if the neighbor has not been visited yet, mark it as visited and add it to the queue

                if (!visited[v])
                {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    // function to perform parallel BFS starting from a given source vertex
    void Parallel_DFS(int src)
    {
        vector<bool> visited(V, false);
        stack<int> q;

        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            // get the next vertex from the queue
            int u = q.top();
            q.pop();

			// process all neighbors of the current vertex
			#pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];

				// if the neighbor has not been visited yet, mark it as visited and add it to the queue
				#pragma omp critical
                {
                    if (!visited[v])
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }
};

int main()
{

    int n;
    cout << "Enter Number of Nodes : ";
    cin >> n;
    Graph g(n);

    // Add edges
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if ((i + 3 * j) % 7 == 0 || (j + 2 * i) % 5 == 0)
            {
                g.addEdge(i, j);
            }
        }
    }

    // perform BFS starting from vertex 0
    cout << "BFS starting from vertex 0:" << endl;
    double t1, t2;

    t1 = omp_get_wtime();
    g.Serial_BFS(0);
    t2 = omp_get_wtime();
    double serial_time = t2 - t1;
    cout << "\nTime taken by Serial BFS: " << t2 - t1 << " seconds" << endl;

    t1 = omp_get_wtime();
    g.Parallel_BFS(0);
    t2 = omp_get_wtime();
    double parallel_time = t2 - t1;
    cout << "\nTime taken by Parallel BFS: " << t2 - t1 << " seconds" << endl;

    cout << "Speed Up of BFS : " << serial_time / parallel_time << endl;

    // perform DFS starting from vertex 0
    cout << "DFS starting from vertex 0:" << endl;

    t1 = omp_get_wtime();
    g.Serial_DFS(0);
    t2 = omp_get_wtime();
    serial_time = t2 - t1;
    cout << "\nTime taken by Serial DFS: " << t2 - t1 << " seconds" << endl;

    t1 = omp_get_wtime();
    g.Parallel_DFS(0);
    t2 = omp_get_wtime();
    parallel_time = t2 - t1;
    cout << "\nTime taken by Parallel DFS: " << t2 - t1 << " seconds" << endl;

    cout << "Speed Up of DFS : " << serial_time / parallel_time << endl;
}
