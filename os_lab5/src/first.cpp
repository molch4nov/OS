extern "C" int PrimeCount(int A, int B);
extern "C" float Pi(int K);

#include <cmath>
int PrimeCount(int A, int B){
    int ans = 0;
    bool flag = true;
    for(int i = A; i <= B; i++){
        for(int j = 2; j < B; j++){
            if(i % j == 0 && i != j){
                flag = false;
            }
        }
        if(flag == true){
            ans += 1;
        }
        flag = true;
    }
    if(A == 1){
        return ans - 1;
    }
    else{
        return ans;
    }
}

float Pi(int K){
    if(K < 0){
        return -1;
    }
    float pi = 1.0;
    for(int i = 1; i <= K; i++){
        pi += pow((-1), i)/(2*i+1);
    }
    return pi * 4;
}

