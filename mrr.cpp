#include <iostream>
#include <string>
using namespace std;

template<typename T>
T input(string msg){
    T _a;
    cout << msg;
    cin >> _a;
    return _a;
}

template<typename T>
T* input2(string msg){
    T* _a = new T(2);
    cout << msg;
    cin >> _a[0] >> _a[1];
    return _a;
}

void print_solution_head(int budget, int n)
{
  cout << "Budget: " << budget << endl;
  cout << "Number machines: " << n << endl;
}

int main() {
  //cout << "Machine Reliability Problem" << endl;
  int budget, n;
  budget = input<int>("");
  n = input<int>("");
  if (n <= 0){
      print_solution_head(budget, n);
      return 0;
  }
  int* cost, * rel;
  cost = new int(n);
  rel = new int(n);
  for (int i=0; i < n; ++i){
      int* cr = input2<int>("");
      cost[i] = cr[0];
      rel[i] = cr[1];
  }


  print_solution_head(budget, n);
  return 0;
}
