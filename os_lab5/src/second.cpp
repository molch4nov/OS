extern "C" int PrimeCount(int A, int B);
extern "C" float Pi(int K);
#include <cmath>
#include <vector>
using namespace std;
int PrimeCount(int A, int B){
    int ans = 0;
    const long long N = 15485863;
    vector<bool>simple(N, true);
    vector<long long> v;
    for(int i = 2; i <= N; ++i) {
        if(simple[i] == true) {
            for(int j = i * 2; j <= N; j += i) {
                simple[j] = false;
            }
            v.push_back(i);
        }
    }
    for(int i = 0; i < v.size(); i++){
        if(v[i] >= A && v[i] <= B){
            ans += 1;
        }
    }
    return ans;
}

float Pi(int K){
    if(K < 0){
        return -1;
    }
    float pi = 1;
    for(int i = 1; i <= K; i++){
        pi *= (4*pow(i,2))/(4*pow(i,2) - 1);
    }
    pi *= 2;
    return pi;
}