#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>  
#include <ctime>    

using namespace std;

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
    srand(seed); // 고정된 시드로 같은 난수 생성

    items.clear();
    for (int i = 0; i < n; ++i) {
        int value = rand() % 500 + 1;   // 1 ~ 500
        int weight = rand() % 100 + 1;  // 1 ~ 100
        items.push_back({weight, value});
    }
}

int dynamicProgramming(vector<Item>& items, int weight);
double greedyProgramming(vector<Item>& items, int weight);
int bruteForceProgramming(vector<Item>& items, int weight, int idx = 0);
int branchAndBoundProgramming(vector<Item>& items, int weight);

int main(){
    vector<Item> items = {
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6}
    };

    int retDP = dynamicProgramming(items, 5);
    double retGreedy = greedyProgramming(items, 5);
    int retBrute = bruteForceProgramming(items, 5);
    int retBranch = branchAndBoundProgramming(items, 5);

    cout << "DP 결과 : " << retDP << endl;
    cout << "Greedy 결과 : " << retGreedy << endl;
    cout << "Brute Force 결과 : " << retBrute << endl;
    cout << "Branch and Bound 결과 : " << retBranch << endl;


    return 0;
}

//Dynamic Programming
int dynamicProgramming(vector<Item>& items, int weight){
    int s = items.size();
    int w = weight;

    int dp[w+1][s+1];

    for(int i=0; i<=w; i++){
        for(int j=0; j<=s; j++){
            dp[i][j] = 0;
        }
    }

    // 작성 과정
    for(int j=1; j<=s; j++){ 
        int current_w = items[j-1].weight;
        int current_b = items[j-1].value;
        
        for(int i=0; i<=w; i++){ 
            if(current_w <= i){ 
                if(current_b + dp[i-current_w][j-1] > dp[i][j-1]){
                    dp[i][j] = current_b + dp[i-current_w][j-1];
                } else {
                    dp[i][j] = dp[i][j-1];
                }
            } else { 
                dp[i][j] = dp[i][j-1];
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
            totalValue += items[i].value / items[i].weight * w;
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