/*
 * Author: Ali Hassani (@alihassanijr)
 * This file is the solution to the machine reliability problem (DPV, ex 6.23, pg 196).
 * Assignment 5
 * CIS 621 - Algorithms and Complexity - University of Oregon
 * Fall 2022.
 *
 */

#include <iostream>
#include <string>
#include <math.h>

using namespace std;


/// Solution class: just a data structure to help reconstruct the solution.
/// The additional integer keeps track of the budget assigned to the specific reliability score.
class Solution {
    public:
        double val;
        int k;
        Solution(){
            val = double(-1);
            k = -1;
        }
        Solution(int _val){
            val = double(_val);
            k = -1;
        }
        Solution(float _val){
            val = double(_val);
            k = -1;
        }
        Solution(double _val){
            val = _val;
            k = -1;
        }
        Solution(double _val, int _k){
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


/// Problem class: Creates a triangular matrix (tensor really) to save on memory, and handles indexing.
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
            // Simple n x budget matrix
            sz = n * budget;
            data = new Solution[sz];
            // The init method just runs copies over reliability for the last machine (base case).
            init();
        }
        int get_index(int i, int k){
            return (i - 1) * budget + k - 1;
        }
        void init(){
            int c = cost[0];
            double rc = 1-rel[0];
            double rcc = rc;
            for (int b=c; b <= budget; b+=c){
                set(1, b, 1 - rcc);
                rcc = rcc * rc;
            }
        }
        Solution get(int i, int k){
            return data[get_index(i, k)];
        }
        void set(int i, int k, double val){
            data[get_index(i, k)] = Solution(val, k);
        }
        void set(int i, int k, Solution val){
            data[get_index(i ,k)] = val;
        }
};


void print_solution_head(int budget, int n){
    cout << "Budget: " << budget << endl;
    cout << "Number machines: " << n << endl << endl;
}


int main(){
    // Set cout precision according to the PDF
    cout.precision(14);

    // Input problem
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

    // Set up problem
    Problem p(n, budget, cost, rel);

    // R[1,:] is already computed within the Problem constructor
    // Now we move up to R[n, :].
    for (int i=2; i <= n; i++){
        double rc = 1-rel[i-1];
        int c = cost[i-1];
        for (int k=0; k <= budget; ++k){
            Solution m(0);
            double rcc = rc;
            for (int b=c; b <= k; b+=c){
                Solution s((1 - rcc) * p.get(i-1, k-b).val, b);
                if (s > m)
                    m = s;
                rcc = rcc * rc;
            }
            p.set(i, k, m);
        }
    }

    print_solution_head(budget, n);

    cout << "Iterated Version:" << endl;
    cout << "Maximum reliability: ";
    cout << p.get(n, budget).val;
    int _bud = budget;
    for (int i=n; i >= 1; i--){
        int _curr_bud = p.get(i, _bud).k;
        int c_i = _curr_bud / cost[i-1];
        _bud -= _curr_bud;
        cout << endl;
        cout << c_i << " copies of machine " << i << " of cost " << cost[i-1];
    }
    cout << endl;
    return 0;
}
