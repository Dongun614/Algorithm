// knapsack_solver.cpp
// References:
// (1) Lecture slide chapter 6, page 3-5
// (2) GeeksforGeeks: Knapsack implementations (Brute-force, DP, Greedy)
// (3) Book: “Algorithm Design Manual” by Steven S. Skiena
// (4) Hyperscale AI: ChatGPT (OpenAI), Claude

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>
#include <random>
using namespace std;
using namespace chrono;

struct Item {
    int value, weight;
    double ratio;
};

bool compare(Item a, Item b) {
    return a.ratio > b.ratio;
}

void generateItems(vector<Item>& items, int n, int seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> valueDist(1, 500);
    uniform_int_distribution<int> weightDist(1, 100);
    items.clear();
    for (int i = 0; i < n; ++i) {
        int value = valueDist(rng);
        int weight = weightDist(rng);
        items.push_back({value, weight, (double)value / weight});
    }
}

// 1. Brute Force
int knapsackBruteForceUtil(vector<Item>& items, int n, int W) {
    if (n == 0 || W == 0) return 0;
    if (items[n-1].weight > W)
        return knapsackBruteForceUtil(items, n-1, W);
    return max(
        knapsackBruteForceUtil(items, n-1, W),
        items[n-1].value + knapsackBruteForceUtil(items, n-1, W - items[n-1].weight)
    );
}

int knapsackBruteForce(vector<Item>& items, int W) {
    return knapsackBruteForceUtil(items, items.size(), W);
}

// 2. Greedy (Fractional)
double knapsackGreedy(vector<Item> items, int W) {
    sort(items.begin(), items.end(), compare);
    double totalValue = 0.0;
    for (auto& item : items) {
        if (W >= item.weight) {
            W -= item.weight;
            totalValue += item.value;
        } else {
            totalValue += item.ratio * W;
            break;
        }
    }
    return totalValue;
}

// 3. Dynamic Programming
int knapsackDP(vector<Item>& items, int W) {
    int n = items.size();
    vector<vector<int>> dp(n+1, vector<int>(W+1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (items[i-1].weight > w)
                dp[i][w] = dp[i-1][w];
            else
                dp[i][w] = max(dp[i-1][w], items[i-1].value + dp[i-1][w - items[i-1].weight]);
        }
    }
    return dp[n][W];
}

// 4. Branch and Bound
struct Node {
    int level, profit, weight;
    double bound;
    bool operator<(const Node& other) const { return bound < other.bound; }
};

double bound(Node u, int n, int W, vector<Item>& items) {
    if (u.weight >= W) return 0;
    double result = u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;
    while (j < n && totalWeight + items[j].weight <= W) {
        totalWeight += items[j].weight;
        result += items[j].value;
        ++j;
    }
    if (j < n) result += (W - totalWeight) * items[j].ratio;
    return result;
}

int knapsackBnB(vector<Item> items, int W) {
    sort(items.begin(), items.end(), compare);
    priority_queue<Node> Q;
    Node u, v;
    u.level = -1; u.profit = u.weight = 0;
    u.bound = bound(u, items.size(), W, items);
    Q.push(u);
    int maxProfit = 0;

    while (!Q.empty()) {
        u = Q.top(); Q.pop();
        if (u.bound <= maxProfit) continue;

        v.level = u.level + 1;
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;
        v.bound = bound(v, items.size(), W, items);
        if (v.bound > maxProfit) Q.push(v);

        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, items.size(), W, items);
        if (v.bound > maxProfit) Q.push(v);
    }

    return maxProfit;
}

void runTest(int n) {
    vector<Item> items;
    generateItems(items, n, 100);
    int capacity = n * 25;

    cout << "Number of Items: " << n << endl;

    if (n <= 31) {
        auto start = high_resolution_clock::now();
        int res = knapsackBruteForce(items, capacity);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        cout << "Brute Force: " << duration << " ms / " << res << endl;
    } else {
        cout << "Brute Force: Skipped for n > 31" << endl;
    }

    // Greedy
    generateItems(items, n, 100);
    auto start = high_resolution_clock::now();
    double greedyVal = knapsackGreedy(items, capacity);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Greedy: " << duration << " ms / " << greedyVal << endl;

    // DP
    generateItems(items, n, 100);
    start = high_resolution_clock::now();
    int dpVal = knapsackDP(items, capacity);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start).count();
    cout << "DP: " << duration << " ms / " << dpVal << endl;

    // BnB
    generateItems(items, n, 100);
    start = high_resolution_clock::now();
    int bnbVal = knapsackBnB(items, capacity);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start).count();
    cout << "BnB: " << duration << " ms / " << bnbVal << endl;

    cout << "----------------------------------" << endl;
}

int main() {
    vector<int> sizes = {11, 21, 31, 10, 100, 1000, 10000};
    for (int n : sizes) {
        runTest(n);
    }
    return 0;
}
