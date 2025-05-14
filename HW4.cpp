#include <iostream>
#include <vector>

using namespace std;

struct Item {
    int weight;
    int value;
};

void dynamicProgramming(const vector<Item>& items, int weight);
void greedyProgramming(const vector<Item>& items, int weight);

int main(){
    vector<Item> items = {
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6}
    };

    dynamicProgramming(items, 5);


    return 0;
}

void dynamicProgramming(const vector<Item>& items, int weight){
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

    // 출력 과정
    for(int i=0; i<=w; i++){
        for(int j=0; j<=s; j++){
            cout << dp[i][j] << "  ";
            if(j==s) cout << endl;
        }
    }
}

bool compare(struct Item a, struct Item b){
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

void greedyProgramming(const vector<Item>& items, int weight){
    int s = items.size();
    int w = weight;

    
}