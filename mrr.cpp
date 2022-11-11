#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class Solution {
    public:
        double val;
        int k;
        Solution()
        {
            val = double(0);
            k = -1;
        }
        Solution(int _val)
        {
            val = double(_val);
            k = -1;
        }
        Solution(float _val)
        {
            val = double(_val);
            k = -1;
        }
        Solution(double _val)
        {
            val = _val;
            k = -1;
        }
        Solution(double _val, int _k)
        {
            val = _val;
            k = _k;
        }
        void operator = (const Solution& c){
            val = c.val;
            k = c.k;
        }
        Solution operator * (const Solution& c){
            Solution r(val*c.val, k);
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
            //for (int i=1; i <= n; ++i){
            int i = n;
                int c = cost[i-1];
                for (int b=c; b <= budget; b+=c){
                    int r = floor(b / c);
                    double tr = 1 - pow((1-rel[i-1]), r);
                    //for (int bb=b; bb < b+c; bb++)
                    set(i, i, b, tr);
                }
            //}
        }
        Solution get(int i, int j, int k){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return Solution(0.0);
            if ((i == j && k < cost[i-1]) || (i < j && k < cost[i-1]+cost[j-1]))
                return Solution(0.0);
            return data[idx];
        }
        bool set(int i, int j, int k, double val){
            int idx = get_index(i, j, k);
            if (idx < 0 || idx >= sz)
                return false;
            data[idx] = Solution(val, k);
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


    int tmp_budget = cost[n-1];

    //for (int st=1; st <= n; ++st){
    for (int i=n-1; i > 0; --i){
        //int j = i + st;
        int c = cost[i-1];
        tmp_budget += c;
        //if (j <= n){
        for (int k=tmp_budget; k <= budget; ++k){
            Solution m(0);
            for (int b=c; b <= k; b+=c){
                Solution pa = p.get(i, i, b);
                if (pa.val == 0){
                    int r = floor(b / c);
                    pa.val = 1 - pow((1-rel[i-1]), r);
                }
                //for (int bb=b; bb < b+c; bb++){
                    p.set(i, i, b, pa);
                    Solution s(pa.val * p.get(i+1, n, k-b).val, b);
                    if (s > m)
                        m = s;
                //}
            }
            p.set(i, n, k, m);
        }
        //}
    }
    //}

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
