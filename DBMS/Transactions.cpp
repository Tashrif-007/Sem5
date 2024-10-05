#include <bits/stdc++.h>
using namespace std;

struct varChange {
    char var;
    int oldValue;
    int newValue;
};

struct Transaction {
    string id;
    bool committed;
    bool committedBeforeCheckpoint;
    vector<varChange> changes;

    bool operator<(const Transaction& tran) const {
        return id < tran.id;
    }
};

unordered_map<char, int> val; 

int main() {
    ifstream file("log.txt");
    if (!file.is_open()) 
    {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }

    set<Transaction> transactions;
    set<string> redoList;
    set<string> undoList;
    string line;
    bool checkpointEncountered = false;

    while(getline(file, line))
    {
        istringstream iss(line);
        string word;

        if(line.find("<START") != string::npos || line.find("<COMMIT") != string::npos)
        {
            iss>>word;
            string tran_id;

            if(word == "<START") 
            {
                iss >> tran_id;
                if (tran_id.back() == '>') tran_id.pop_back();
                transactions.insert({tran_id, false, false, {}});
            } 
            else if(word == "<COMMIT") 
            {
                iss>>tran_id;
                if(tran_id.back() == '>') tran_id.pop_back();
                auto it = transactions.find({tran_id, false, false, {}});
                if(it != transactions.end()) 
                {
                    Transaction committed_tran = *it;
                    transactions.erase(it);
                    committed_tran.committed = true;
                    committed_tran.committedBeforeCheckpoint = !checkpointEncountered;
                    transactions.insert(committed_tran);
                }
            }
        } 
        else if(line.find("<CKPT") != string::npos)
            checkpointEncountered = true; 
        else if(line[1] == 'T') 
        {
            iss >> word;
            string tran_id = word.substr(1, word.find(' ') - 1);
            char var = line[4];
            iss >> word;
            string oldStr, newStr;
            iss >> oldStr >> newStr;

            newStr.pop_back();
            auto it = transactions.find({tran_id, false, false, {}});
            if(it != transactions.end()) 
            {
                Transaction temp_tx = *it;
                temp_tx.changes.push_back({var, stoi(oldStr), stoi(newStr)});
                transactions.erase(it);
                transactions.insert(temp_tx);
            }
        }
    }

    file.close();

    for(auto& tran : transactions) 
    {
        if(tran.committed && (!tran.committedBeforeCheckpoint || !checkpointEncountered))
            redoList.insert(tran.id);
        if(!tran.committed && (!tran.committedBeforeCheckpoint || !checkpointEncountered))
            undoList.insert(tran.id);
    }

    for(const auto& tran_id : undoList) 
    {
        auto it = find_if(transactions.begin(), transactions.end(), [&tran_id](const Transaction& t) { return t.id == tran_id; });
        if(it != transactions.end()) 
        {
            auto& tran = *it;
            for(auto& change : tran.changes) 
            {
                val[change.var] = change.oldValue;  
            }
        }
    }


    for(auto& tran_id : redoList)
    {
        auto it = find_if(transactions.begin(), transactions.end(), [&tran_id](const Transaction& t) { return t.id == tran_id; });

        if(it != transactions.end()) 
        {
            const auto& tran = *it;
            for(auto& change : tran.changes) 
            {
                val[change.var] = change.newValue; 
            }
        }
    }

    cout<<"Undo: ";
    for(auto x : undoList) cout<<x<<" ";
    cout<<"\nRedo: ";
    for(auto x : redoList) cout<<x<<" ";
    cout<<"\n";

    cout<<"Final values after undo/redo:"<<endl;
    for(auto&[var, value] : val)
        cout<<var<<": "<<value<<endl;
    return 0;
}
