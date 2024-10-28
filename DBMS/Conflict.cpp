#include <bits/stdc++.h>
using namespace std;

unordered_map<int, vector<int>>dependencyGraph;

bool hasCycle(int node, vector<bool>&visited, vector<bool>&path) 
{
    if (!visited[node]) 
    {
        visited[node] = true;
        path[node] = true;

        for (int adj : dependencyGraph[node]) 
        {
            if (!visited[adj] && hasCycle(adj, visited, path)) return true;
            else if (path[adj]) return true;
        }
    }
    path[node] = false;
    return false;
}

bool isCyclic(int numTransactions)
{
    vector<bool> visited(numTransactions, false);
    vector<bool> path(numTransactions, false);

    for (int i = 0; i < numTransactions; i++) 
    {
        if (hasCycle(i, visited, path)) return true;
    }
    return false;
}

void printGraph() 
{
    cout << "Dependency Graph:" << endl;
    for (auto pair : dependencyGraph) 
    {
        cout << "Transaction T" << pair.first + 1 << " -> ";
        for (int adj : pair.second) 
            cout << "T" << adj + 1 << " ";
        cout << endl;
    }
}

void buildGraph(const vector<vector<string>>schedule) 
{
    unordered_map<string, vector<pair<int, char>>> lastAccess;

    for (int step=0; step<schedule[0].size(); step++) 
    {
        for (int t=0; t<schedule.size(); t++) 
        {
            string op = schedule[t][step];
            if (op == "-" || op == "COM") continue;

            char type = op[0];
            string dataItem = op.substr(2);

            if (lastAccess.find(dataItem) != lastAccess.end()) 
            {
                for (auto lastTrans : lastAccess[dataItem]) 
                {
                    int lastID = lastTrans.first;
                    char lastType = lastTrans.second;

                    if (lastID != t) 
                    {
                        if ((type == 'W' && (lastType == 'R' || lastType == 'W')) || (type == 'R' && lastType == 'W')) 
                            dependencyGraph[lastID].push_back(t);
                    }
                }
            }
            lastAccess[dataItem].emplace_back(t, type);
        }
    }
}

vector<int> topologicalSort(int numTransactions) 
{
    vector<int> inDegree(numTransactions, 0);
    for (auto pair : dependencyGraph) 
    {
        for (int adj : pair.second) 
            inDegree[adj]++;
    }

    queue<int> q;
    for (int i = 0; i < numTransactions; i++) 
    {
        if (inDegree[i] == 0) 
            q.push(i);
    }

    vector<int> order;
    while (!q.empty()) 
    {
        int node = q.front();
        q.pop();
        order.push_back(node);

        for (int adj : dependencyGraph[node]) 
        {
            if (--inDegree[adj] == 0) 
                q.push(adj);
        }
    }

    if (order.size() == numTransactions) 
        return order;
    else 
        return {};  
}

int main() 
{
    ifstream file("dep3.txt");

    vector<vector<string>> schedule;
    string line;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        string temp;
        vector<string> transactionOps;
        while (ss >> temp) 
            transactionOps.push_back(temp);
        schedule.push_back(transactionOps);
    }
    file.close();

    buildGraph(schedule);

    int numTransactions = schedule.size();
    if (isCyclic(numTransactions)) 
    {
        cout << "Has Cycle" << endl;
        printGraph();
    } 
    else 
    {
        cout << "No Cycle" << endl;
        printGraph();

        vector<int> order = topologicalSort(numTransactions);
        if (!order.empty()) 
        {
            cout << "Non-conflicting Transaction Serial: ";
            for (int i : order) 
                cout << "T" << i + 1 << " ";
            cout << endl;
        } 
        else 
        {
            cout << "Cycle detected, no valid transaction order possible." << endl;
        }
    }
    return 0;
}
