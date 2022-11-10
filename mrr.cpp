#include <iostream>
#include <string>
#include <math.h>
using namespace std;


class Problem
{
    public:
        double* data;
        int n, budget;
        int sz = 0;
        int* cost;
        float* rel;
        Problem(int _n, int _budget, int* _cost, float* _rel){
            n = _n;
            budget = _budget;
            cost = _cost;
            rel = _rel;
            sz = floor(n * (n + 1) / 2) * budget;
            data = new double(sz);
            for (int i=0; i < sz; ++i)
                data[i] = 0.0;
            init();
        }
        int get_index(int i, int j, int k){
            if (i > j || k < 1 || i < 1 || j < 1 || i > n || j > n || k > budget)
                return -1;
            int _i = i - 1;
            int _j = j - 1;
            int _k = k - 1;
            int x = (_j - _i) + floor(((_i) * (_i - 1 - (2 * n))) * -0.5);
            return x * budget + _k;
        }
        void init()
        {
            for (int i=1; i <= n; ++i){
                int c = cost[i-1];
                for (int b=c; b <= budget; ++b){
                    int r = floor(b / c);
                    set(i, i, r, 1 - pow((1-rel[i-1]), r));
                }
            }
        }
        double get(int i, int j, int k){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return double(0.0);
            if ((i == j && k < cost[i]) || (i < j && k < cost[i]+cost[j]))
                return double(0.0);
            return data[idx];
        }
        bool set(int i, int j, int k, double val){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return false;
            data[idx] = val;
            return true;
        }
};


void print_solution_head(int budget, int n){
    cout << "Budget: " << budget << endl;
    cout << "Number machines: " << n << endl << endl;
}

int main(){
    //cout << "Machine Reliability Problem" << endl;
    int budget, n;
    cin >> budget;
    cin >> n;
    if (n <= 0){
        print_solution_head(budget, n);
        return 0;
    }
    int* cost;
    float* rel;
    cost = new int(n);
    rel = new float(n);
    for (int i=0; i < n; ++i){
        cin >> cost[i] >> rel[i];
    }

    Problem p(n, budget, cost, rel);


    //for (int i=1; i <= n; ++i){
    //    for (int j=i+1; j <= n; ++j){
    //        for (int k=cost[i]+cost[j]-1; k <= budget; ++k){
    //            float m = 0;
    //            for (int b=0; b < k; ++b)
    //                
    //        }
    //    }
    //}

    print_solution_head(budget, n);
    return 0;
}
