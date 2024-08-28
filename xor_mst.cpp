//https://codeforces.com/contest/888/problem/G
#include <bits/stdc++.h>

using namespace std;
const int MAXN = 200200;
int n, a[MAXN], sa[MAXN];
vector<int> adj[MAXN];
int color[MAXN];

void paint(int v)
{
	for(int u : adj[v])
		if(!color[u])
		{
			color[u] = color[v] ^ 3;
			paint(u);
		}
}

int trieC[MAXN * 31][2], trieCnt[MAXN * 31], trieVal[MAXN * 31], trieSz;
void insert(int key, int val)
{
	int n = 0;
	for(int bit = 30; bit >= 0; --bit)
	{
		trieCnt[n]++;
		if(bit == 0)
			return void(trieVal[n] = val);
		int b = (key >> (bit - 1)) & 1;
		if(!trieC[n][b])
			trieC[n][b] = ++trieSz;
		n = trieC[n][b];
	}
}
 
void erase(int key)
{
	int n = 0;
	for(int bit = 30; bit >= 0; --bit)
	{
		trieCnt[n]--;
		if(bit == 0)
			return;
		int b = (key >> (bit - 1)) & 1;
		n = trieC[n][b];
	}
}

int get(int key)
{
	int n = 0;
	for(int bit = 30; bit >= 0; --bit)
	{
		if(bit == 0)
			return trieVal[n];
		int b = (key >> (bit - 1)) & 1;
		if(trieC[n][b] && trieCnt[trieC[n][b]])
			n = trieC[n][b];
		else
			n = trieC[n][!b];
	}
}

long long solve(int l, int r, int bit = 29)
{
	if(bit == -1 || l >= r)
		return 0;
	int m = l - 1;
	while(m <= r && ((a[sa[m + 1]] >> bit) & 1) == 0)
		m++;
	long long ret = solve(l, min(r, m), bit - 1) + solve(m + 1, r, bit - 1);
	if(l <= m && m + 1 <= r)
	{
		for(int i = l; i <= m; ++i)
			insert(a[sa[i]], sa[i]);
		
		tuple<int, int, int> mn((1 << 30), 0, 0);

		for(int i = m + 1; i <= r; ++i)
		{
			int k = get(a[sa[i]]);
			mn = min(mn, make_tuple(a[k] ^ a[sa[i]], k, sa[i]));
		}

		for(int i = l; i <= m; ++i)
			erase(a[sa[i]]);

		auto[x, y, z] = mn;
		ret += x;
		adj[y].push_back(z);
		adj[z].push_back(y);
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(false), cin.tie(NULL);
	cin >> n;
	for(int i = 1; i <= n; ++i)
		cin >> a[i], sa[i] = i;
	sort(sa + 1, sa + n + 1, [](int x, int y)
	{
		return a[x] < a[y];
	});
	
	cout << solve(1, n) << endl;

	/* color[1] = 1; */
	/* paint(1); */
	/*  */
	/* for(int i = 1; i <= n; ++i) */
	/* 	cout << color[i] - 1; */
	/* cout << endl; */
}
 
