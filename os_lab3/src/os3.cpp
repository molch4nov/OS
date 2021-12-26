#include <iostream>
#include <pthread.h>

using namespace std;
string str, templ;
int N;
typedef struct arguments {
  int start;
  int stop;
} Arg;

void *thread_function(void *args) {
  Arg *arg = (Arg *)args;
  int start = arg->start;//начало скобочки
  int stop = arg->stop;//конец скобочки) полуинтервал [ )
  for (int j = start; j < stop; ++j) {
  bool k = true;
  for (int i = 0; i < N; ++i) {
    if (str[j + i] != templ[i]) {
    k = false;
    }
  }
  if (k) {
    cout << "Substring starts at " << j << endl;
  }
  }
  return nullptr;
}

int main() {
  int threads_num;
  cin >> str;
  cin >> templ;
  cin >> threads_num;
  N = (int)templ.size();
  auto *threads = new pthread_t[threads_num];
  int points_for_thread = ((int)str.size()) / threads_num;
  int num_created_threads = 0;
  Arg a;
  bool kostil = false;
  a.start = a.stop = 0;
  for (int i = 0; (i < threads_num) && !kostil; i++) { //создает потоки
    a.start = a.stop;
    a.stop = a.start + points_for_thread + 1;
    if (a.stop >= str.size()) {
      a.stop = ((int)str.size()) - N + 1;
      kostil = true;
    }
    if (pthread_create(&threads[i], nullptr, thread_function, &(a)) != 0) {
      cout << "Can not create thread" << endl;
      exit(1);
    }
    if (pthread_join(threads[i], nullptr) != 0) {
      cout << "Join error" << endl;
      exit(1);
    }
    ++num_created_threads;
    }
    cout << num_created_threads << " threads was created" << endl;
    delete threads;
    return 0;
}