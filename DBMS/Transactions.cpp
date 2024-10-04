#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    string id;
    bool committed;
    bool committedBeforeCheckpoint;
    bool operator<(const Transaction& tran) const {
        return id < tran.id;
    }
};

int main() {
    ifstream file("log.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }
    set<Transaction>transactions;
    set<string>redoList;
    set<string>undoList;
    string line;
    bool checkpointEncountered = false;

    while (getline(file, line)) 
    {
        istringstream iss(line);
        string word;

        if(line.find("<START") != string::npos || line.find("<COMMIT") != string::npos) 
        {
            iss >> word;
            string tran_id;

            if(word == "<START") 
            {
                iss >> tran_id;
                if (tran_id.back() == '>') tran_id.pop_back();
                transactions.insert({tran_id, false, false});
            } 
            else if(word == "<COMMIT") 
            {
                iss >> tran_id;
                if (tran_id.back() == '>') tran_id.pop_back();
                auto it = transactions.find({tran_id, false, false});
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
            transactions.insert({tran_id, false, false});
        }
    }

    file.close();

    for (auto& tran : transactions)
    {
        if(tran.committed && (!tran.committedBeforeCheckpoint || !checkpointEncountered))
            redoList.insert(tran.id);
        if(!tran.committed && (!tran.committedBeforeCheckpoint || !checkpointEncountered))
            undoList.insert(tran.id);
    }

    cout<<"Total unique transactions: "<<transactions.size()<<endl;
    
    cout<<"Transaction Details:"<<endl;
    for(auto& tran : transactions)
        cout<<"Transaction ID: "<<tran.id<<", Committed: "<<(tran.committed ? "Yes" : "No")<<", Committed Before CKPT: " << (tran.committedBeforeCheckpoint ? "Yes" : "No")<<endl;

    cout<<"Transactions to Redo: ";
    for(auto& tran_id : redoList)
        cout<<tran_id<<" ";
    cout<<endl;

    cout<<"Transactions to Undo: ";
    for(auto& tran_id : undoList)
        cout<<tran_id<<" ";
    cout<<endl;

    return 0;
}
