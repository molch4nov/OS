#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>
using namespace std;
int main () {
    const char* lib_array[] = {"libd1.so", "libd2.so"};
    int cur, StartLib;
    
    cout << "Enter start library: " << endl;
    cout << "1 for using first library" << endl;
    cout << "2 for using second library" << endl;
    cin >> StartLib;

    bool flag = true;
    while (flag) {
        if (StartLib == 1) {
            cur = 0;
            flag = false;
        }
        else if (StartLib == 2) {
            cur = 1;
            flag = false;
        }
        else {
            cout << "Only one or one + one" << endl;
            cin >> StartLib;
        }
    }
    void* handle = NULL; 
    handle = dlopen(lib_array[cur], RTLD_LAZY); 
    if (!handle) {
        cout << "An error while opening library has been detected" << endl;
        exit(EXIT_FAILURE);
    }
    int (*PrimeCount)(int A, int B); 
    float (*Pi)(int K); 
    PrimeCount = (int(*)(int, int))dlsym(handle, "PrimeCount"); 
    Pi = (float(*)(int))dlsym(handle, "Pi"); 


    int command;
    cout << "Choose: " << endl;
    cout << "1 for changing the contract;" << endl;
    cout << "2 for calculating the count of simple numbers; " << endl;
    cout << "3 for calculating the Pi; " << endl; 
    while (printf("Please enter your command: ") && (scanf("%d", &command)) != EOF) {
        if (command == 1) {
            dlclose(handle); //освобождает указатель на библиотеку и программа перестает ей пользоваться
            if (cur == 0) {
                cur = 1;
                handle = dlopen(lib_array[cur], RTLD_LAZY);
                if (!handle) {
                    cout << "An error while opening library has been detected" << endl;
                    exit(EXIT_FAILURE);
                }
                PrimeCount = (int(*)(int, int))dlsym(handle, "PrimeCount");
                Pi = (float(*)(int))dlsym(handle, "Pi"); 
            }
            else if (cur == 1) {
                cur = 0;
                handle = dlopen(lib_array[cur], RTLD_LAZY);
                if (!handle) {
                    cout << "An error while opening library has been detected" << endl;
                    exit(EXIT_FAILURE);
                }
                PrimeCount = (int(*)(int, int))dlsym(handle, "PrimeCount");
                Pi = (float(*)(int))dlsym(handle, "Pi");
            }
        cout << "You have changed contracts!" << endl;
        }
        else if (command == 2) {
            int A, B;
            cout << "Enter A and B "<< endl;
            cin >> A >> B;
            int ans = PrimeCount(A, B);
            cout << "Your answer: " << ans << endl;
            
        }
        else if (command == 3) {
            float pi; int K;
            cout << "Enter K:" << endl;
            cin >> K;
            pi = Pi(K);
            cout << "Your answer: " << pi << endl;
        
        }
        else {
            cout << "You had to enter only 1, 2 or 3!" << endl;
        }    
    }
    dlclose(handle);
    return 0;
}