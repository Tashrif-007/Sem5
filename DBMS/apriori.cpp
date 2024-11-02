#include<bits/stdc++.h>
using namespace std;

vector<set<int>> filter(map<set<int>, int> supportCount, int minSupport)
{
    vector<set<int>>res;
    for(auto cnt : supportCount)
    {
        if(cnt.second >= minSupport)
        {
            res.push_back({cnt.first});
        }
    }
    return res;
}

map<set<int>, int> supportCount(vector<set<int>>&combos, vector<set<int>>transactionDB)
{
    map<set<int>,int> res;
    for(auto item : combos)
    {
        for(auto transaction : transactionDB)
        {
            if(includes(transaction.begin(), transaction.end(), item.begin(), item.end()))
            {
                res[item]++;
            }
        }
    }
    return res;
}

vector<set<int>> genCombo(vector<set<int>>&freqItemList, int combi)
{
    vector<set<int>>combos;
    int sizee = freqItemList.size();
    for(int i=0; i<sizee; i++)
    {
        for(int j=i+1; j<sizee; j++)
        {
            set<int>temp = freqItemList[i];
            temp.insert(freqItemList[j].begin(), freqItemList[j].end());

            if(temp.size()==combi)
            {
                bool hasFreqSubset = false;
                for(auto item : temp)
                {
                    set<int> sub = temp;
                    sub.erase(item);
                    if(find(freqItemList.begin(), freqItemList.end(), sub)==freqItemList.end())
                    {
                        hasFreqSubset = true;
                        break;
                    }
                }
                if(!hasFreqSubset) combos.push_back(temp);
            }
        }
    }
    return combos;
}

vector<set<int>> apriori(vector<set<int>>transactionDB, int minSupport)
{
    vector<set<int>>res;
    map<set<int>,int>cnt;

    map<int,int>cnt1;
    for(auto transaction : transactionDB)
    {
        for(auto item : transaction)
        {
            cnt1[item]++;
        }
    }

    vector<set<int>>freqItemList;
    for(auto cnt : cnt1)
    {
        if(cnt.second>=minSupport)
        freqItemList.push_back({cnt.first});
    }
    int combi=2;
    while(!freqItemList.empty())
    {
        res.insert(res.end(), freqItemList.begin(), freqItemList.end());

        vector<set<int>> allCombo = genCombo(freqItemList, combi);

        cnt = supportCount(allCombo, transactionDB);

        freqItemList = filter(cnt, minSupport);
        combi++;
    }

    int mx=0;
    for(auto x : res)
    {
        mx = max(mx, (int)x.size());
    }
    vector<set<int>>finalList;
    for(auto item : res)
    {
        if(item.size() == mx)
        {
            finalList.push_back(item);
        }
    }
    return finalList;
}

int main()
{
    ifstream file("apriori.txt");
    string line;
    vector<set<int>>transactionDB;

    while(getline(file, line))
    {
        istringstream is(line);
        string transId;
        is>>transId;

        set<int>transaction;
        string temp;
        while(is>>temp)
        {
            if(temp[0]=='I')
            {
                int itemId = stoi(temp.substr(1));
                transaction.insert(itemId);
            }
        }
        transactionDB.push_back(transaction);
    }
    file.close();
    
    int minSupport = 2;

    vector<set<int>> frequentItemsets = apriori(transactionDB, minSupport);

    for(auto item : frequentItemsets)
    {
        for(auto i : item) cout<<"I"<<i<<" ";
        cout<<endl;
    }
    return 0;
}
