#include <bits/stdc++.h>
using namespace std;

// Define a k-dimensional point as a vector of doubles
typedef vector<double> Point;

// Node structure for the k-d tree
struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(const Point& pt) : point(pt), left(nullptr), right(nullptr) {}
};
// Function to calculate the squared Euclidean distance between two points
double squaredDistance(const Point& a, const Point& b) {
    double dist = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        dist += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return dist;
}

// Function to find the nearest neighbor using the k-d tree
void findNearest(KDNode* root, const Point& target, KDNode*& best, double& bestDist, int depth, int k) {
    if (!root) return;

    // Calculate the distance between the current node and the target
    double dist = squaredDistance(root->point, target);

    // Update the best point if the current node is closer
    if (dist < bestDist) {
        bestDist = dist;
        best = root;
    }

    // Determine the axis to compare
    int axis = depth % k;

    // Recursively search the subtree that contains the target
    KDNode* nextBranch = (target[axis] < root->point[axis]) ? root->left : root->right;
    KDNode* otherBranch = (target[axis] < root->point[axis]) ? root->right : root->left;

    findNearest(nextBranch, target, best, bestDist, depth + 1, k);

    // Check the other branch if there's a possibility of closer points
    if ((target[axis] - root->point[axis]) * (target[axis] - root->point[axis]) < bestDist) {
        findNearest(otherBranch, target, best, bestDist, depth + 1, k);
    }
}

// Function to start the nearest neighbor search
KDNode* findNearestNeighbor(KDNode* root, const Point& target, int k) {
    KDNode* best = nullptr;
    double bestDist = numeric_limits<double>::max();
    findNearest(root, target, best, bestDist, 0, k);
    return best;
}

// Function to print a point
void printPoint(const Point& point) {
    cout << "(";
    for (size_t i = 0; i < point.size(); ++i) {
        cout << point[i];
        if (i < point.size() - 1) cout << ", ";
    }
    cout << ")";
}
// Function to sort points based on a specific dimension
bool comparePoints(const Point& a, const Point& b, int dimension) {
    return a[dimension] < b[dimension];
}

// Recursive function to build a balanced k-d tree
KDNode* buildKDTree(vector<Point>& points, int depth, int k) {
    if (points.empty()) return nullptr;

    int axis = depth % k;  // Select axis based on depth
    size_t medianIdx = points.size() / 2;

    // Sort points by the current axis
    nth_element(points.begin(), points.begin() + medianIdx, points.end(),
                [&](const Point& a, const Point& b) { return comparePoints(a, b, axis); });

    // Create the current node
    KDNode* node = new KDNode(points[medianIdx]);

    // Split points into left and right subsets and build subtrees
    vector<Point> leftPoints(points.begin(), points.begin() + medianIdx);
    vector<Point> rightPoints(points.begin() + medianIdx + 1, points.end());

    node->left = buildKDTree(leftPoints, depth + 1, k);
    node->right = buildKDTree(rightPoints, depth + 1, k);

    return node;
}

// Recursive function to print the k-d tree
void printKDTree(KDNode* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "(";
    for (size_t i = 0; i < node->point.size(); ++i) {
        cout << node->point[i];
        if (i < node->point.size() - 1) cout << ", ";
    }
    cout << ")" << endl;

    printKDTree(node->left, depth + 1);
    printKDTree(node->right, depth + 1);
}

// Function to find the minimum node in a specific dimension
KDNode* findMin(KDNode* root, int dim, int depth, int k) {
    if (!root) return nullptr;

    int cd = depth % k;

    if (cd == dim) {
        if (!root->left) return root;
        return findMin(root->left, dim, depth + 1, k);
    }

    KDNode* leftMin = findMin(root->left, dim, depth + 1, k);
    KDNode* rightMin = findMin(root->right, dim, depth + 1, k);

    KDNode* minNode = root;
    if (leftMin && leftMin->point[dim] < minNode->point[dim]) minNode = leftMin;
    if (rightMin && rightMin->point[dim] < minNode->point[dim]) minNode = rightMin;

    return minNode;
}

// Function to delete a node with a specific point
KDNode* deleteNode(KDNode* root, const Point& target, int depth, int k) {
    if (!root) return nullptr;

    int cd = depth % k;

    if (root->point == target) {
        if (root->right) {
            KDNode* minNode = findMin(root->right, cd, depth + 1, k);
            root->point = minNode->point;
            root->right = deleteNode(root->right, minNode->point, depth + 1, k);
        } else if (root->left) {
            KDNode* minNode = findMin(root->left, cd, depth + 1, k);
            root->point = minNode->point;
            root->right = deleteNode(root->left, minNode->point, depth + 1, k);
            root->left = nullptr;
        } else {
            delete root;
            return nullptr;
        }
        return root;
    }

    if (target[cd] < root->point[cd]) {
        root->left = deleteNode(root->left, target, depth + 1, k);
    } else {
        root->right = deleteNode(root->right, target, depth + 1, k);
    }
    return root;
}

// Function to insert a point into the k-d tree
KDNode* insertPoint(KDNode* root, const Point& point, int depth, int k) {
    if (!root) return new KDNode(point);

    int cd = depth % k;
    if (point[cd] < root->point[cd]) {
        root->left = insertPoint(root->left, point, depth + 1, k);
    } else {
        root->right = insertPoint(root->right, point, depth + 1, k);
    }

    return root;
}

// Function to delete the k-d tree and free memory
void deleteKDTree(KDNode* node) {
    if (!node) return;
    deleteKDTree(node->left);
    deleteKDTree(node->right);
    delete node;
}

// Function to read points from a text file
vector<Point> readPointsFromFile(const string& filename, int& k) {
    ifstream file(filename);
    vector<Point> points;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return points;
    }

    string line;
    // Read the first line for the dimension
    if (getline(file, line)) {
        k = stoi(line);
    }

    // Read the rest of the lines for points
    while (getline(file, line)) {
        stringstream ss(line);
        Point point;
        string value;

        while (getline(ss, value, ',')) {
            point.push_back(stod(value));
        }

        if (point.size() != k) {
            cerr << "Error: Point dimensionality does not match the specified dimension k=" << k << endl;
            continue;
        }

        points.push_back(point);
    }

    file.close();
    return points;
}

void testInsertion(KDNode* root, Point newPoint, int k)
{
    cout << "\nInserting point: ";
    for (size_t i = 0; i < newPoint.size(); ++i) {
        cout << newPoint[i];
        if (i < newPoint.size() - 1) cout << ", ";
    }
    cout << "\n";

    root = insertPoint(root, newPoint, 0, k);

    cout << "\nTree after insertion:" << endl;
    printKDTree(root, 0);
}
void testDelete(KDNode* root, vector<Point>& points, int k)
{
    if (!points.empty()) {
        cout << "\nDeleting point: ";
        for (size_t i = 0; i < points[0].size(); ++i) {
            cout << points[0][i];
            if (i < points[0].size() - 1) cout << ", ";
        }
        cout << "\n";

        root = deleteNode(root, points[0], 0, k);

        cout << "\nTree after deletion:" << endl;
        printKDTree(root, 0);
    }
}
int main(int argc, char** argv) {
    string filename = argv[1]; // File containing the points
    int k = 0; // Dimensionality
    vector<Point> points = readPointsFromFile(filename, k);

    if (points.empty() || k <= 0) {
        cerr << "Error: No valid points or invalid dimension in the input file." << endl;
        return 1;
    }

    KDNode* root = buildKDTree(points, 0, k);

    cout << "Balanced k-d Tree:" << endl;
    printKDTree(root, 0);

    // Example: Insert a new point
    Point newPoint = {1.5, 2.5}; // Change to your desired point
    
    testInsertion(root, newPoint, k);
    // Example: Delete a point
    testDelete(root, points, k);

    Point queryPoint = {6.0, 10.0}; // Change to your desired query point
    cout << "\nQuery Point: ";
    printPoint(queryPoint);
    cout << endl;

    KDNode* nearest = findNearestNeighbor(root, queryPoint, k);
    if (nearest) {
        cout << "Nearest Neighbor: ";
        printPoint(nearest->point);
        cout << endl;
    } else {
        cout << "No nearest neighbor found." << endl;
    }
    // Free memory
    deleteKDTree(root);

    return 0;
}
