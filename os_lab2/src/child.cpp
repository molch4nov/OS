#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(){
    int n; char c;
    while(scanf("%d%c", &n, &c) != EOF){
        if(n < 0){
            break;
        }
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                printf("%d\n ", n);
                break;
            } 
            else {
                break;
            }
        }
    }
    return 0;
}