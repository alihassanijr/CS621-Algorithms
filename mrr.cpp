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
/// The additional integer keeps track of the budget assigned to the specific reliability score..
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
            // Upper triangular n x n matrix, times budget.t
            sz = (n * (n + 1) / 2) * budget;
            data = new Solution[sz];
            // The init method just runs copies over reliability for the last machine (base case).
            init();
        }
        int get_index(int i, int j, int k){
            /* Commented out these checks to get a nice little performance improvement. 
               These int ops pile up when they're called frequently, which they are.
             */
            //if (i > j || k < 1 || i < 1 || j < 1 || i > n || j > n || k > budget)
            //    return -1;
            //int _i = i - 1;
            //int _j = j - 1;
            //int _k = k - 1;
            //int x = (j - i) + floor(((i - 1) * (i - 2 - (2 * n))) * -0.5);
            //return x * budget + k - 1;
            return ((j - i) + (((i - 1) * (i - 2 - (2 * n))) * -0.5)) * budget + k - 1;
        }
        void init(){
            int c = cost[n-1];
            double rc = 1-rel[n-1];
            double rcc = rc;
            for (int b=c; b <= budget; b+=c){
                set(n, n, b, 1 - rcc);
                rcc = rcc * rc;
            }
        }
        Solution get(int i, int j, int k){
            /* Commented out these checks to get a nice little performance improvement. 
               These int ops pile up when they're called frequently, which they are.
             */
            //int idx = get_index(i, j, k);
            //if (idx < 0 || idx >= sz)
            //    return Solution(0.0);
            //if ((i == j && k < cost[i-1]) || (i < j && k < cost[i-1]+cost[j-1]))
            //    return Solution(0.0);
            return data[get_index(i, j, k)];
        }
        bool set(int i, int j, int k, double val){
            /* Commented out these checks to get a nice little performance improvement. 
               These int ops pile up when they're called frequently, which they are.
             */
            //int idx = get_index(i, j, k);
            //if (idx < 0 || idx >= sz)
            //    return false;
            data[get_index(i, j, k)] = Solution(val, k);
            return true;
        }
        bool set(int i, int j, int k, Solution val){
            /* Commented out these checks to get a nice little performance improvement. 
               These int ops pile up when they're called frequently, which they are.
             */
            //int idx = get_index(i, j, k);
            //if (idx < 0 || idx >= sz)
            //    return false;
            data[get_index(i, j ,k)] = val;
            return true;
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

    // Remaining budget: as we move up in the problem, we assume every machine has at least one copy.
    // As a result, we can reduce budget to save on iterations.
    int rem_budget = cost[n-1];

    // Case `n, n` is already computed within the Problem constructor
    // Now we move backwards down to case `1, n`.
    for (int i=n-1; i > 0; --i){
        double rc = 1-rel[i-1];
        int c = cost[i-1];
        rem_budget += c;
        for (int k=rem_budget; k <= budget; ++k){
            Solution m(0);
            double rcc = rc;
            for (int b=c; b <= k; b+=c){
                Solution pa = p.get(i, i, b);
                if (pa.val < 0)
                    pa.val = 1 - rcc;
                p.set(i, i, b, pa);
                Solution s(pa.val * p.get(i+1, n, k-b).val, b);
                if (s > m)
                    m = s;
                rcc = rcc * rc;
            }
            p.set(i, n, k, m);
        }
    }

    print_solution_head(budget, n);

    cout << "Iterated Version:" << endl;
    cout << "Maximum reliability: ";
    cout << p.get(1, n, budget).val;
    int _bud = budget;
    int* copies = new int[n];
    // Reconstruct first, then print, to maintain the reversed order in the PDF.
    for (int i=1; i <= n; i++){
        int _curr_bud = p.get(i, n, _bud).k;
        copies[i-1] = _curr_bud / cost[i-1];
        _bud -= _curr_bud;
    }
    for (int i=n; i > 0; i--){
        cout << endl;
        cout << copies[i-1] << " copies of machine " << i << " of cost " << cost[i-1];
    }
    cout << endl;
    return 0;
}
