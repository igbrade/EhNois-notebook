// Segment tree 2d
// Range querry, cell update
// n rows, m columns
// 1-indexed

#include <bits/stdc++.h>
 
using namespace std;
const int MAXN = 1024;

int n, m;

int get(int a, int b, int no[]){
    int ans = 0;
    for(a += m, b += m + 1;a < b;a >>= 1, b >>= 1){
        if(a & 1) ans = max(ans, no[a++]);
        if(b & 1) ans = max(ans, no[--b]); 
    }
    return ans;
}

void upd(int p, int x, int no[]){
    for(p += m, no[p] = x;p > 1;p >>= 1)
        no[p >> 1] = max(no[p], no[p ^ 1]);
}
 
int no[4 * MAXN][4 * MAXN];
 
void upd2d(int id, int l, int r, int i, int j, int x){
    int mid = (l + r) >> 1, fe = 2 * id, fd = fe + 1;
 
    if(l == r)
        return upd(j, x, no[id]);
    if(i <= mid)
        upd2d(fe, l, mid, i, j, x);
    else
        upd2d(fd, mid + 1, r, i, j, x);
    upd(j, max(get(j, j, no[fe]), get(j, j, no[fd])), no[id]);
}
 
int get2d(int id, int l, int r, int i, int j, int i_, int j_){
    int mid = (l + r) >> 1, fe = 2 * id, fd = fe + 1;
 
    if(i_ < l || r < i)
        return 0;
    if(i <= l && r <= i_)
        return get(j, j_, no[id]);
    return max(get2d(fe, l, mid, i, j, i_, j_), get2d(fd, mid + 1, r, i, j, i_, j_));
}

inline void segupd(int i, int j, int x){ return upd2d(1, 1, n, i, j, x); }
// (i, j) -> left upper corner
// (i_, j_) -> right lower corner
inline int  segget(int i, int j, int i_, int j_) { return get2d(1, 1, n, i, j, i_, j_); } 