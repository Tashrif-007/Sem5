#include <bits/stdc++.h>
using namespace std;

struct Node {
    string attribute;
    map<string, Node*> children;
    string label;
    bool isLeaf = false;

    Node() : isLeaf(false) {}
};


vector<string> split(const string& str, char divider) 
{
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while(getline(ss, token, divider)) tokens.push_back(token);
    return tokens;
}

double calculateEntropy(const vector<vector<string>>& data, int labelIdx) 
{
    map<string, int> labelCounts;
    for(const auto& row : data) labelCounts[row[labelIdx]]++;

    double entropy = 0.0;
    int total = data.size();
    for(const auto& [label, count] : labelCounts) 
    {
        double p = (double)count / total;
        entropy -= p * log2(p);
    }
    return entropy;
}

double calculateInfoGain(const vector<vector<string>>& data, int attrIdx, int labelIdx) 
{
    double totalEntropy = calculateEntropy(data, labelIdx);

    map<string, vector<vector<string>>>subsets;
    for (const auto& row : data) subsets[row[attrIdx]].push_back(row);

    double subsetEntropy = 0.0;
    int totalSize = data.size();
    for (const auto& [value, subset] : subsets) subsetEntropy += (double)subset.size() / totalSize * calculateEntropy(subset, labelIdx);
    return totalEntropy - subsetEntropy;
}

int findBestAttribute(const vector<vector<string>>& data, const vector<string>& attributes, int labelIdx) 
{
    int bestAttr = -1;
    double maxGain = -1;

    for (int i=0; i<attributes.size(); i++) 
    {
        if (i==labelIdx) continue; 
        double gain = calculateInfoGain(data, i, labelIdx);
        if (gain > maxGain) {
            maxGain = gain;
            bestAttr = i;
        }
    }
    return bestAttr;
}

Node* buildTree(vector<vector<string>>& data, vector<string>& attributes, int labelIdx) 
{
    Node* node = new Node();

    set<string> uniqueLabels;
    for (const auto& row : data) uniqueLabels.insert(row[labelIdx]);

    if(uniqueLabels.size()==1) 
    {
        node->isLeaf = true;
        node->label = *uniqueLabels.begin();
        return node;
    }

    if(attributes.size()==1) 
    {
        map<string, int> labelCounts;
        for(const auto& row : data) labelCounts[row[labelIdx]]++;
        node->isLeaf = true;
        node->label = max_element(labelCounts.begin(), labelCounts.end(), [](const auto& a, const auto& b) { return a.second < b.second; })->first;
        return node;
    }

    int bestAttrIdx = findBestAttribute(data, attributes, labelIdx);
    node->attribute = attributes[bestAttrIdx];

    map<string, vector<vector<string>>>subsets;

    for(const auto& row : data) subsets[row[bestAttrIdx]].push_back(row);

    for(auto& [value, subset] : subsets) 
    {
        vector<string> newAttributes = attributes;
        newAttributes.erase(newAttributes.begin() + bestAttrIdx);

        for(auto& row : subset) row.erase(row.begin() + bestAttrIdx);

        node->children[value] = buildTree(subset, newAttributes, labelIdx - (labelIdx > bestAttrIdx ? 1 : 0));
    }
    return node;
}

string predict(Node* tree, const vector<string>& row, const vector<string>& attributes) 
{
    if(tree->isLeaf==true) return tree->label;

    auto it = find(attributes.begin(), attributes.end(), tree->attribute);

    // if(it==attributes.end())
    //     throw runtime_error("Attribute not found in row.");
    // }
    int attrIdx = distance(attributes.begin(), it);
    string value = row[attrIdx];

    if (tree->children.find(value) != tree->children.end()) return predict(tree->children[value], row, attributes);
    return "N/A";
}

int main(int argc, char* argv[]) 
{
    if(argc != 3) 
    {
        cerr<<"Missing: "<<argv[0]<<" <training_data.txt> <test_data.txt>"<<endl;
        return 1;
    }

    ifstream trainFile(argv[1]);
    if(!trainFile) 
    {
        cerr<<"Error: Could not open training data file."<<endl;
        return 1;
    }

    vector<vector<string>> trainData;
    string line;
    getline(trainFile, line);
    vector<string> attributes = split(line, ',');

    while(getline(trainFile, line)) trainData.push_back(split(line, ','));

    trainFile.close();

    int labelIdx = attributes.size() - 1; 
    Node* tree = buildTree(trainData, attributes, labelIdx);

    ifstream testFile(argv[2]);
    if(!testFile) 
    {
        cerr<<"Error: Could not open test data file."<<endl;
        return 1;
    }

    vector<vector<string>> testData;
    getline(testFile, line); 
    vector<string> testAttributes = split(line, ',');

    while(getline(testFile, line)) testData.push_back(split(line, ','));

    testFile.close();

    ofstream outputFile("output.txt");
    if(!outputFile) 
    {
        cerr<<"Error: Could not open output file."<<endl;
        return 1;
    }


    for(auto i=0; i<attributes.size(); i++) 
    {
        outputFile<<attributes[i];
        if(i<attributes.size()-1) outputFile<<",";
    }

    outputFile<<endl;

    for(const auto& row : testData) 
    {
        for(auto i=0; i<row.size(); i++) 
        {
            outputFile<<row[i];
            if(i<row.size()-1) outputFile<<",";
        }
        string prediction = predict(tree, row, attributes);
        outputFile<<","<<prediction<<endl;
    }

    outputFile.close();

    cout<<"Predictions written to output.txt"<<endl;
    return 0;
}
