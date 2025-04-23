//22000712 조동운

//Reference
//HW3 pdf
//https://scarlettb.tistory.com/5
//https://tyt0815.tistory.com/23

#include <iostream>
#include <climits>
#include <ctime>

using namespace std;

int recursive(int k, int n){
    if(k == 1) return n;
    if(n == 1) return 1;
    if(n == 0) return 0;

    int min_attempts = INT_MAX;
    for(int x=1; x<=n; x++){
        int case1 = recursive(k-1, x-1); // 깨진경우
        int case2 = recursive(k, n-x); // 안 깨진경우
        min_attempts = min(min_attempts, 1 + max(case1, case2));
    }

    return min_attempts;
}

int DP(int k, int n){
    int dp[k+1][n+1];

    for(int i=1; i<=k; i++){
        dp[i][0] = 0;
        dp[i][1] = 1;
    }

    for(int j=1; j<=n; j++){
        dp[1][j] = j;
    }

    for(int i=2; i<=k; i++){
        for(int j=2; j<=n; j++){
            dp[i][j] = INT_MAX;
            int temp = INT_MAX;
            for(int x=1; x<=j; x++){
                int case1 = dp[i-1][x-1];
                int case2 = dp[i][j-x];
                temp = min(temp, 1 + max(case1, case2));
                if(temp < dp[i][j]) dp[i][j] = temp;
            }
        }
    }

    for(int i=1; i<=k; i++){
        for(int j=1; j<=n; j++){
            cout << "dp[" << i << "]" <<"[" << j << "]: " << dp[i][j] << "  ";
            if(j == n){
                cout << endl;
            }
        }
    }

    cout << endl;
    //cout << "Subproblems used to compute dp[" << k << "]" << "[" << n << "]:" << endl;

    return dp[k][n];
}

int main(){
    int N,K; //n은 높이, k는 공 개수
    cout << "Enter the number of meters (N): ";
    cin >> N;
    cout << "Enter the number of fragile objects (K): ";
    cin >> K;

    // 재귀 방식 시간 측정
    clock_t rec_start = clock();
    int recursiveResult = recursive(K, N);
    clock_t rec_end = clock();
    double rec_time = (double)(rec_end - rec_start) / CLOCKS_PER_SEC * 1000000; // 마이크로초

    // DP 방식 시간 측정
    clock_t dp_start = clock();
    int DPResult = DP(K, N);
    clock_t dp_end = clock();
    double dp_time = (double)(dp_end - dp_start) / CLOCKS_PER_SEC * 1000000; // 마이크로초

    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Minimum Attempts (Recursive): " << recursiveResult << endl;
    cout << "Execution Time (Recursive): " << (int)rec_time << " microseconds" << endl;
    cout << endl;
    cout << "Minimum Attempts (DP): " << DPResult << endl;
    cout << "Execution Time (DP): " << (int)dp_time << " microseconds" << endl;
    cout << "------------------------------------------------------------" << endl;
    
    return 0;
}