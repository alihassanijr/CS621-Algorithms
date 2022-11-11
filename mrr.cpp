#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class Solution {
    public:
        double val;
        int i, j, k;
        Solution()
        {
            val = double(0);
            i = -1;
            j = -1;
            k = -1;
        }
        Solution(int _val)
        {
            val = double(_val);
            i = -1;
            j = -1;
            k = -1;
        }
        Solution(float _val)
        {
            val = double(_val);
            i = -1;
            j = -1;
            k = -1;
        }
        Solution(double _val)
        {
            val = _val;
            i = -1;
            j = -1;
            k = -1;
        }
        Solution(double _val, int _i, int _j, int _k)
        {
            val = _val;
            i = _i;
            j = _j;
            k = _k;
        }
        void operator = (const Solution& c){
            val = c.val;
            i = c.i;
            j = c.j;
            k = c.k;
        }
        Solution operator * (const Solution& c){
            Solution r(val*c.val, i, j, k);
            return r;
        }
        bool operator== (const Solution& c){
            return val == c.val;
        }
        bool operator!= (const Solution& c){
            return val != c.val;
        }
        bool operator<= (const Solution& c){
            return val <= c.val;
        }
        bool operator>= (const Solution& c){
            return val >= c.val;
        }
        bool operator< (const Solution& c){
            return val < c.val;
        }
        bool operator> (const Solution& c){
            return val > c.val;
        }
};

class Problem
{
    public:
        Solution* data;
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
            data = new Solution[sz];
            //for (int i=0; i < sz; ++i)
            //    data[i] = Solution(0.0);
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
                    set(i, i, b, 1 - pow((1-rel[i-1]), r));
                }
            }
        }
        //void print(int start, int end){
        //    for (int k=start; k <= end; k++)
        //    {
        //        cout << k << endl;
        //        cout << endl;
        //        for (int i=1; i <= n; i++)
        //        {
        //            for (int j=1; j <= n; j++)
        //                cout << get(i, j, k) << " ";
        //            cout << endl;
        //        }
        //        cout << endl;
        //        cout << endl;

        //    }
        //}
        Solution get(int i, int j, int k){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return Solution(0.0);
            if ((i == j && k < cost[i]) || (i < j && k < cost[i]+cost[j]))
                return Solution(0.0);
            return data[idx];
        }
        bool set(int i, int j, int k, double val){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return false;
            data[idx] = Solution(val, i, j, k);
            return true;
        }
        bool set(int i, int j, int k, Solution val){
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
    cout.precision(14);
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
    cost = new int[n];
    rel = new float[n];
    for (int i=0; i < n; ++i){
        cin >> cost[i] >> rel[i];
    }

    Problem p(n, budget, cost, rel);


    for (int st=1; st <= n; ++st){
        for (int i=1; i <= n; ++i){
            int j = i + st;
            if (j <= n){
                for (int k=cost[i-1]; k <= budget; ++k){
                    Solution m(0);
                    for (int b=1; b <= k; ++b){
                        Solution c = p.get(i, i, b) * p.get(i+1, j, k-b);
                        c.i = i;
                        c.j = i;
                        c.k = b;
                        if (c > m)
                            m = c;
                    }
                    p.set(i, j, k, m);
                }
            }
        }
    }

    print_solution_head(budget, n);
    //p.print(budget, budget);

    cout << "Iterated Version:" << endl;
    cout << "Maximum reliability: ";
    cout << p.get(1, n, budget).val;
    int _bud = budget;
    int* copies = new int[n];
    for (int i=1; i <= n; i++){
        int _curr_bud = p.get(i, n, _bud).k;
        copies[i-1] = floor(_curr_bud / cost[i-1]);
        _bud -= _curr_bud;
    }
    for (int i=n; i > 0; i--){
        cout << endl;
        cout << copies[i-1] << " copies of machine " << i << " of cost " << cost[i-1];
    }
    cout << endl;
    return 0;
}
