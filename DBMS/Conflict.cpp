#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<int, vector<int>> dependencyGraph;

bool hasCycle(int node, vector<bool>& visited, vector<bool>& stack) {
    if (!visited[node]) {
        visited[node] = true;
        stack[node] = true;

        for (int adj : dependencyGraph[node]) {
            if (!visited[adj] && hasCycle(adj, visited, stack)) 
                return true;
            else if (stack[adj]) 
                return true;
        }
    }
    stack[node] = false;
    return false;
}

bool isCyclic(int numTransactions) {
    vector<bool> visited(numTransactions, false);
    vector<bool> stack(numTransactions, false);

    for (int i = 0; i < numTransactions; i++) {
        if (hasCycle(i, visited, stack)) 
            return true;
    }
    return false;
}

void buildGraph(const vector<vector<string>>& schedule) {
    unordered_map<string, pair<int, char>> lastAccess;

    for (int step = 0; step < schedule[0].size(); step++) {
        for (int t = 0; t < schedule.size(); t++) {
            string op = schedule[t][step];
            if (op == "-" || op == "COM") continue;

            char type = op[0];
            string dataItem = op.substr(2, op.size() - 3);

            if (lastAccess.find(dataItem) != lastAccess.end()) {
                int lastTrans = lastAccess[dataItem].first;
                char lastType = lastAccess[dataItem].second;

                if (lastTrans != t) {
                    if ((type == 'W' && lastType == 'R') || (type == 'R' && lastType == 'W') || (type == 'W' && lastType == 'W')) {
                        dependencyGraph[lastTrans].push_back(t);
                    }
                }
            }

            if (type == 'W' || lastAccess[dataItem].second != 'W') {
                lastAccess[dataItem] = {t, type};
            }
        }
    }
}

int main() {
    ifstream file("dep.txt");
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    vector<vector<string>> schedule;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        vector<string> transactionOps;
        while (ss >> temp) {
            transactionOps.push_back(temp);
        }
        schedule.push_back(transactionOps);
    }
    file.close();

    buildGraph(schedule);

    int numTransactions = schedule.size();
    if (isCyclic(numTransactions)) {
        cout << "The schedule is not conflict serializable (it has a cycle)." << endl;
    } else {
        cout << "The schedule is conflict serializable (no cycle detected)." << endl;
    }

    return 0;
}
