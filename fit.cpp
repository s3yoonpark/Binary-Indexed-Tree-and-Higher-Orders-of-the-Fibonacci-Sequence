// Solution verified with Dynamic Range Sum Queries on CSES 
// https://cses.fi/problemset/task/1648

#include <bits/stdc++.h> 
using namespace std; 

#define int long long 

const int order = 9; // choose the order of the Fibonacci sequence used 
const int mxN = 2e5;  

int N; 

vector<int> fib; // stores the Fibonacci sequence 
vector<vector<bool>> fib_bit; // stores the Fibonacci representation of numbers 
vector<int> smallest_fib; // stores SUF(k) 
vector<int> smallest_sum; // stores G(k) 
vector<int> pref_sum; // stores prefix sum of the Fibonacci sequence 

int FIT[mxN+1]; 

void update(int k, int x) {
  while (k <= N) {
    FIT[k] += x; 
    k += smallest_sum[k];
  }
}

int query(int k) {
  int sum = 0;  
  while (k >= 1) {
    sum += FIT[k]; 
    k -= smallest_fib[k]; 
  }
  return sum;
}

signed main() {
  ios_base::sync_with_stdio(false); 
  cin.tie(nullptr); 

  fib.assign(order+1, 1LL); 
  fib[0] = 0; 
  smallest_fib.assign(mxN+1, -1); 
  smallest_sum.assign(mxN+1, -1); 

  int tmp = order; 
  while (fib.back() <= mxN) {
    fib.push_back(tmp); 
    tmp += tmp - fib[ssize(fib)-order-1]; 
  }

  pref_sum.assign(ssize(fib)+1, 0LL); 
  for (int i=1; i<=ssize(fib); i++) {
    pref_sum[i] = pref_sum[i-1] + fib[i];
  }

  fib_bit.assign(mxN+1, vector(ssize(fib), false)); 

  for (int i=1; i<=mxN; i++) {
    int num = i; 
    int consecutive = 0, last = -1; 
    for (int j=ssize(fib)-1; j>=1; j--) {
      if (num - fib[j] >= 0) {
        num -= fib[j]; 
        if (last != j+1) {
          consecutive = 0; 
        }
        last = j; 
        ++consecutive; 

        fib_bit[i][j] = true; 
      }
      if (num == 0 && smallest_fib[i] == -1) {
        smallest_fib[i] = fib[j]; 
        smallest_sum[i] = pref_sum[j-1] - pref_sum[j-order+consecutive-1]; 
      }
    }
  }

  int q; 
  cin >> N >> q; 
  for (int i=1; i<=N; i++) {
    int x; 
    cin >> x;
    update(i, x); 
  }
  while (q--) {
    int op; 
    cin >> op; 
    if (op == 1) {
      int k, u; 
      cin >> k >> u; 
      int cur_val = query(k) - query(k-1); 
      int diff = u - cur_val; 
      update(k, diff); 
    }
    if (op == 2) {
      int a, b; 
      cin >> a >> b; 
      cout << query(b) - query(a-1) << '\n'; 
    }
  }

  return 0; 
}