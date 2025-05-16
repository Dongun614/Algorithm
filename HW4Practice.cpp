/*
References:
(1) Lecture slide chapter 6. page3-5
(2) https://www.w3schools.com/dsa/dsa_ref_knapsack.php
(3) https://www.digitalocean.com/community/tutorials/fractional-knapsack-cpp
(4) https://www.sanfoundry.com/cpp-program-solve-knapsack-problem-using-dynamic-programming/
(5) https://www.tutorialspoint.com/0-1-knapsack-using-branch-and-bound-in-c-cplusplus
(6) https://www.programiz.com/cpp-programming/library-function/cstdlib/srand
(7) ChatGPT, Gemini, Claude

// Did not implement brute force for data size >= 31 due to exponential time.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <ctime>
#include <cstdlib>
using namespace std;

// 랜덤 데이터 생성 함수
void generateData(vector<int>& weights, vector<int>& values, int n) {
    srand(100);
    for (int i = 0; i < n; ++i) {
        values[i] = 1 + rand() % 500;
        weights[i] = 1 + rand() % 100;
    }
}

// 1. Brute Force (재귀)
int knapsackBruteForce(const vector<int>& weights, const vector<int>& values, int n, int W, int idx = 0) {
    if (idx == n || W == 0) return 0;
    if (weights[idx] > W)
        return knapsackBruteForce(weights, values, n, W, idx + 1);
    int included = values[idx] + knapsackBruteForce(weights, values, n, W - weights[idx], idx + 1);
    int excluded = knapsackBruteForce(weights, values, n, W, idx + 1);
    return max(included, excluded);
}

// 2. Greedy (Fractional Knapsack)
struct Item {
    int value, weight;
    double ratio;
};
bool cmp(const Item& a, const Item& b) {
    return a.ratio > b.ratio;
}
double fractionalKnapsack(const vector<int>& weights, const vector<int>& values, int n, int W) {
    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        items[i].value = values[i];
        items[i].weight = weights[i];
        items[i].ratio = (double)values[i] / weights[i];
    }
    sort(items.begin(), items.end(), cmp);
    double totalValue = 0.0;
    int capacity = W;
    for (int i = 0; i < n && capacity > 0; ++i) {
        if (items[i].weight <= capacity) {
            totalValue += items[i].value;
            capacity -= items[i].weight;
        } else {
            totalValue += items[i].ratio * capacity;
            break;
        }
    }
    return totalValue;
}

// 3. Dynamic Programming
int knapsackDP(const vector<int>& weights, const vector<int>& values, int n, int W) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (weights[i - 1] > w)
                dp[i][w] = dp[i - 1][w];
            else
                dp[i][w] = max(dp[i - 1][w], values[i - 1] + dp[i - 1][w - weights[i - 1]]);
        }
    }
    return dp[n][W];
}

// 4. Branch and Bound
struct Node {
    int level, profit, weight;
    double bound;
    bool operator<(const Node& other) const {
        return bound < other.bound;
    }
};
double bound(const Node& u, int n, int W, const vector<Item>& items) {
    if (u.weight >= W) return 0;
    double profit_bound = u.profit;
    int j = u.level + 1, totweight = u.weight;
    while (j < n && totweight + items[j].weight <= W) {
        totweight += items[j].weight;
        profit_bound += items[j].value;
        ++j;
    }
    if (j < n)
        profit_bound += (W - totweight) * items[j].ratio;
    return profit_bound;
}
int knapsackBnB(const vector<int>& weights, const vector<int>& values, int n, int W) {
    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        items[i].value = values[i];
        items[i].weight = weights[i];
        items[i].ratio = (double)values[i] / weights[i];
    }
    sort(items.begin(), items.end(), cmp);
    queue<Node> Q;
    Node u, v;
    u.level = -1; u.profit = u.weight = 0;
    u.bound = bound(u, n, W, items);
    int maxProfit = 0;
    Q.push(u);
    while (!Q.empty()) {
        u = Q.front(); Q.pop();
        if (u.level == n - 1) continue;
        v.level = u.level + 1;
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;
        v.bound = bound(v, n, W, items);
        if (v.bound > maxProfit)
            Q.push(v);
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, items);
        if (v.bound > maxProfit)
            Q.push(v);
    }
    return maxProfit;
}

// 시간 측정용 함수
long long getTimeMs() {
    return static_cast<long long>(clock()) * 1000 / CLOCKS_PER_SEC;
}

int main() {
    // Brute Force: 11, 21, 31개 아이템
    vector<int> test_sizes_brute = {11, 21, 31};
    cout << "| Number of Items | Processing time in ms / Maximum benefit value |\n";
    cout << "|-----------------|------------------------------------------------|\n";
    for (int n : test_sizes_brute) {
        vector<int> weights(n), values(n);
        generateData(weights, values, n);
        int W = n * 25;
        long long t1 = getTimeMs();
        int maxVal = 0;
        if (n <= 21) { // 31개는 시간 초과 가능성 높음
            maxVal = knapsackBruteForce(weights, values, n, W);
        }
        long long t2 = getTimeMs();
        if (n <= 21)
            cout << "| " << setw(15) << n << " | " << fixed << setprecision(2)
                 << (t2 - t1) << " / " << maxVal << " |\n";
        else
            cout << "| " << setw(15) << n << " | N/A (too slow) |\n";
    }

    // Greedy/DP/BnB: 10, 100, 1000, 10000개 아이템
    vector<int> test_sizes = {10, 100, 1000, 10000};
    cout << "\n| Number of Items | Greedy (ms/max) | D.P. (ms/max) | B&B (ms/max) |\n";
    cout << "|-----------------|-----------------|---------------|--------------|\n";
    for (int n : test_sizes) {
        vector<int> weights(n), values(n);
        generateData(weights, values, n);
        int W = n * 25;

        // Greedy
        long long t1 = getTimeMs();
        double greedyVal = fractionalKnapsack(weights, values, n, W);
        long long t2 = getTimeMs();

        // DP
        long long t3 = getTimeMs();
        int dpVal = knapsackDP(weights, values, n, W);
        long long t4 = getTimeMs();

        // Branch and Bound
        long long t5 = getTimeMs();
        int bnbVal = 0;
        if (n <= 1000) // 10000개는 BnB 시간 초과 가능성
            bnbVal = knapsackBnB(weights, values, n, W);
        long long t6 = getTimeMs();

        cout << "| " << setw(15) << n
             << " | " << setw(7) << (t2 - t1) << " / " << setw(7) << (int)greedyVal
             << " | " << setw(5) << (t4 - t3) << " / " << setw(7) << dpVal
             << " | ";
        if (n <= 1000)
            cout << setw(5) << (t6 - t5) << " / " << setw(7) << bnbVal << " |\n";
        else
            cout << "N/A      |\n";
    }
    return 0;
}
