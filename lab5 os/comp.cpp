#include <iostream>
using namespace std;
extern "C" int PrimeCount(int A, int B);
extern "C" float Pi(int K);

int main(){
    int checker;
    cout << "Choose a function" << endl;
    bool flag = true;
    while(flag){
        scanf("%d", &checker);
        switch(checker){
            case 1: {   
                int A,B;
                cout << "Enter A and B numbers" << endl;
                cin >> A >> B;
                int ans = PrimeCount(A, B);
                cout << "Your answer: " << ans << endl;
                break;
            }
            case 2: {
                float K;
                cout << "Enter positive K" << endl;
                cin >> K;
                float ans = Pi(K);
                cout << "Pi  =  " << ans << endl;
                break;
            }
            case 3: {
                flag = false;
                break;
            }

            default:
            cout << "Only one or one + one or 3" << endl;
                break;
        }
    }
    return 0;
}