/*
22000712 조동운

References:
(1) week_9_midterm_ch22_up.pdf
(2) https://www.geeksforgeeks.org/c-program-for-the-fractional-knapsack-problem/
(3) https://www.w3schools.com/dsa/dsa_ref_knapsack.php
(4) https://www.digitalocean.com/community/tutorials/fractional-knapsack-cpp
(5) https://www.sanfoundry.com/cpp-program-solve-knapsack-problem-using-dynamic-programming/
(6) https://www.tutorialspoint.com/0-1-knapsack-using-branch-and-bound-in-c-cplusplus
(7) https://www.programiz.com/cpp-programming/library-function/cstdlib/srand
*/

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib> 
#include <ctime>    
#include <chrono>
#include <iomanip> 

using namespace std;
using namespace std::chrono;

struct Item {
    int weight;
    int value;
};

struct Node {
    int level, benefit, weight;
    double bound;
    bool operator<(const Node& other) const { return bound < other.bound; }
};

void randomItems(vector<Item>& items, int n, int seed) {
    srand(seed); 

    items.clear();
    for (int i = 0; i < n; ++i) {
        int value = rand() % 500 + 1;  
        int weight = rand() % 100 + 1;
        items.push_back({weight, value});
    }
}

int dynamicProgramming(vector<Item>& items, int weight);
double greedyProgramming(vector<Item>& items, int weight);
int bruteForceProgramming(vector<Item>& items, int weight, int idx = 0);
int branchAndBoundProgramming(vector<Item>& items, int weight);

int main(){
    int forBrute[] = {11, 21, 31}; 
    int forOther[] = {10, 100, 1000, 10000}; 

    int retBrute;
    int retDP;
    int retGreedy;
    int retBranch;

    cout << "[1] Brute Force (Processing Time (ms) / Maximum benefit)" << endl;
    cout << "Number of Items\tBrute Force" << endl;


    //forBrute[]에 있는 값들 주면서 bruteforce에서 각각 걸리는 시간과 maximum benefit value 구하기
    for(int i : forBrute){
        vector<Item> items;
        cout << i << "\t\t";
        randomItems(items, i, 100);
        int capacity = i * 25;
        // retBrute = bruteForceProgramming(items, capacity);

        auto start = high_resolution_clock::now();
        int result = bruteForceProgramming(items, capacity);
        auto end = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(end - start).count() / 1000.0;

        cout << fixed << setprecision(2) << ms << " / " << result << "\t" << endl;
    }

    cout << "\n[2] Greedy / D.P. / B. & B. (Processing Time(ms) / Maximum benefit)" << endl;
    cout << "Number of Items \tGreedy" << setw(30) << "DP" << setw(33) << "B&B" << endl;

    //forOther[]에 있는 값들 주면서 bruteforce를 제외한 나머지 알고리즘들에서 걸리는 시간과 maximum benefit value 구하기 
    for (int i : forOther) {
    int capacity = i * 25;
        vector<Item> items;
    randomItems(items, i, 100);

    cout << i << "\t\t";

    // Greedy
    auto start = high_resolution_clock::now();
    double greedy = greedyProgramming(items, capacity);
    auto end = high_resolution_clock::now();
    double msGreedy = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << fixed << setprecision(2) << setw(10) << msGreedy << " / " << (int)greedy << "\t\t";

    //DP
    start = high_resolution_clock::now();
    int dp = dynamicProgramming(items, capacity);
    end = high_resolution_clock::now();
    double msDP = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << fixed << setprecision(2) << setw(10) << msDP << " / " << dp << "\t\t";

    // // B&B
    start = high_resolution_clock::now();
    int bb = branchAndBoundProgramming(items, capacity);
    end = high_resolution_clock::now();
    double msBB = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << fixed << setprecision(2) << setw(10) << msBB << " / " << bb << endl;
}

    return 0;
}

//Dynamic Programming
int dynamicProgramming(vector<Item>& items, int weight){
    int s = items.size();
    int w = weight;

    vector<vector<int>> dp(w + 1, vector<int>(s + 1, 0));

    for(int i=0; i<=w; i++){
        for(int j=0; j<=s; j++){
            dp[i][j] = 0;
        }
    }

    // 작성 과정
    for (int j = 1; j <= s; j++) {
        int current_w = items[j - 1].weight;
        int current_b = items[j - 1].value;

        for (int i = 0; i <= w; i++) {
            if (current_w <= i) {
                dp[i][j] = max(current_b + dp[i - current_w][j - 1], dp[i][j - 1]);
            } else {
                dp[i][j] = dp[i][j - 1];
            }
        }
    }

    return dp[w][s];
}


//ratio sort 할 때
bool compare(struct Item a, struct Item b){
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

//Greedy
double greedyProgramming(vector<Item>& items, int weight){
    int s = items.size();
    int w = weight;
    double totalValue = 0.0;

    sort(items.begin(), items.end(), compare);
    
    for(int i=0; i<s && w > 0; ++i){
        if(items[i].weight <= w){
            totalValue += items[i].value;
            w -= items[i].weight;
        }
        else{
            totalValue += ((double)items[i].value / items[i].weight) * w;
            break;
        }
    }
    
    return totalValue;
}

int bruteForceProgramming(vector<Item>& items, int weight, int idx){
    int s = items.size();
    if(idx == s || weight == 0) return 0;
    if(items[idx].weight > weight){
        return bruteForceProgramming(items, weight, idx + 1);
    }

    int include = items[idx].value + bruteForceProgramming(items, weight - items[idx].weight, idx + 1);
    int exclude = bruteForceProgramming(items, weight, idx + 1);

    return max(include, exclude);
}

double calBound(Node u, int n, int W, vector<Item>& items) {
    if (u.weight >= W) return 0;
    double result = u.benefit;
    int j = u.level + 1;
    int totalWeight = u.weight;
    while (j < n && totalWeight + items[j].weight <= W) {
        totalWeight += items[j].weight;
        result += items[j].value;
        ++j;
    }
    if (j < n) result += (W - totalWeight) * items[j].value / items[j].weight; //(double)a.value / a.weight
    return result;
}

//Branch and Bound
int branchAndBoundProgramming(vector<Item>& items, int W) {
    sort(items.begin(), items.end(), compare);
    priority_queue<Node> Q;
    Node u, v;
    u.level = -1; u.benefit = u.weight = 0;
    u.bound = calBound(u, items.size(), W, items);
    Q.push(u);
    int maxProfit = 0;

    while (!Q.empty()) {
        u = Q.top(); Q.pop();
        if (u.bound <= maxProfit) continue;

        v.level = u.level + 1;
        v.weight = u.weight + items[v.level].weight;
        v.benefit = u.benefit + items[v.level].value;
        if (v.weight <= W && v.benefit > maxProfit)
            maxProfit = v.benefit;
        v.bound = calBound(v, items.size(), W, items);
        if (v.bound > maxProfit) Q.push(v);

        v.weight = u.weight;
        v.benefit = u.benefit;
        v.bound = calBound(v, items.size(), W, items);
        if (v.bound > maxProfit) Q.push(v);
    }

    return maxProfit;
}