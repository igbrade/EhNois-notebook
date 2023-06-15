
int tin[MAXN], tmin[MAXN], curt, blockcnt;
vector<int> blocks[MAXN];
bool isCut[MAXN];
vector<int> st;
void findCuts(int v, int p = -1)
{
	tin[v] = tmin[v] = ++curt;
	st.push_back(v);
	int childCnt = 0;
	for(int u : adj[v])
	{
		if(u == p)
			continue;
		if(!tin[u])
		{
			childCnt++;
			findCuts(u, v);
			tmin[v] = min(tmin[v], tmin[u]);
			if(tmin[u] >= tin[v])
			{
				isCut[v] = true;
				blocks[v].push_back(++blockcnt);
				while(st.back() != u)
					blocks[st.back()].push_back(blockcnt), st.pop_back();
				blocks[st.back()].push_back(blockcnt);
				st.pop_back();
			}
		}
		else
			tmin[v] = min(tmin[v], tin[u]);
	}

	if(p == -1)
		isCut[v] = (childCnt > 1);

}

vector<int> bcAdj[2 * MAXN];

void buildBlockCutTree()
{
	findCuts(1);

	for(int i = 1; i <= n; ++i)
		if(isCut[i])
		{
			for(int b : blocks[i])
			{
				bcAdj[b + n].push_back(i);
				bcAdj[i].push_back(b + n);
			}
		}
}


