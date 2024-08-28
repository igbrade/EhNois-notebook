//https://codeforces.com/contest/613/problem/D
#include <bits/stdc++.h>
#ifdef LOCAL
#include "dbg.hpp"
#endif
#ifndef dbg
#define dbg(...)
#endif

using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const int MAXN = 100100, MAXLN = 18;
int n, q;
vector<int> adj[MAXN];

int p[MAXN][MAXLN], tin[MAXN], tout[MAXN], curt, lvl[MAXN];
void dfs(int v)
{
	for(int i = 1; (1 << (i - 1)) <= lvl[v]; ++i)
		p[v][i] = p[p[v][i - 1]][i - 1];

	tin[v] = ++curt;
	for(int u : adj[v])
		if(u != p[v][0])
		{
			p[u][0] = v;
			lvl[u] = lvl[v] + 1;
			dfs(u);
		}
	tout[v] = curt;
}

bool ins(int u, int v)
{
	return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int lca(int u, int v)
{
	if(lvl[u] < lvl[v])
		swap(u, v);
	for(int i = MAXLN - 1; i >= 0; --i)
		if(lvl[u] - (1 << i) >= lvl[v])
			u = p[u][i];
	if(u == v)
		return u;
	for(int i = MAXLN - 1; i >= 0; --i)
		if(p[u][i] != p[v][i])
		{
			u = p[u][i];
			v = p[v][i];
		}
	return p[u][0];
}

vector<int> virtualTree[MAXN];

vector<int> buildVirtualTree(const vector<int> &vtx)
{
	auto comp = [](int x, int y)
	{
		return tin[x] < tin[y];
	};
	vector<int> vv;
	for(int j = 0; j + 1 < sz(vtx); ++j)
		vv.push_back(lca(vtx[j], vtx[j + 1]));
	sort(all(vv), comp);
	vv.erase(unique(all(vv)), vv.end());

	{
		vector<int> temp;
		merge(all(vtx), all(vv), back_inserter(temp), comp);
		temp.erase(unique(all(temp)), temp.end());
		vv = temp;
	}
	
	vector<int> stack;
	for(int x : vv)
	{
		while(!stack.empty() && !ins(stack.back(), x))
			stack.pop_back();
	
		if(!stack.empty())
		{
			int u = stack.back();
			virtualTree[u].push_back(x);
			virtualTree[x].push_back(u);
		}

		stack.push_back(x);
	}


	return vv;
}

bool marked[MAXN];
int cnt[MAXN];

int solve(const vector<int> &vtx, const vector<int> &markedVtx)
{
	for(int x : markedVtx)
		marked[x] = true;
	
	for(int u : markedVtx)
		for(int v : adj[u])
			if(marked[v])
				return -1;
	
	
	vector<int> q;
	q.reserve(sz(vtx));

	for(int x : vtx)
	{
		cnt[x] = sz(virtualTree[x]);
		if(cnt[x] == 1)
			q.push_back(x);
	}

	int ans = 0;
	while(!q.empty())
	{
		int cur = q.back();
		q.pop_back();
		
		cnt[cur] = 0;

		int a = 0;
		for(int u : virtualTree[cur])
		{
			if(cnt[u] != 0)
			{
				cnt[u]--;
				if(cnt[u] == 1)
					q.push_back(u);
				continue;
			}
			if(marked[u] && marked[cur])
				ans++;
			else if(marked[u])
				a++;
		}
		if(a > 1)
		{
			ans++;
		}
		else if(a == 1){
			marked[cur] = true;
		}
	}
	

	return ans;
}

void solve_case()
{
	cin >> n;
	for(int i = 1, u, v; i < n; ++i)
	{
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(1);
	cin >> q;
	for(int i = 1; i <= q; ++i)
	{
		int k;
		cin >> k;
		vector<int> v(k);
		for(int j = 0; j < k; ++j)
			cin >> v[j];
		sort(all(v), [](int x, int y)
		{
			return tin[x] < tin[y];
		});
		
		vector<int> vtx = buildVirtualTree(v);

		cout << solve(vtx, v) << '\n';

		for(int x : vtx)
		{
			marked[x] = false;
			cnt[x] = 0;
			virtualTree[x].clear();
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false), cin.tie(NULL);
	cout << setprecision(15) << fixed;
	int t = 1;
	// cin >> t;
	while(t--)
		solve_case();
}

