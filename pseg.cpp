struct pseg
{
	struct node
	{
		int l = 0, r = 0, cnt = 0;
	};
	vector<node> nodes = {node()}; 
 
	int upd(int v, int bg, int nd, int pos, int val)
	{
		node nn = nodes[v];
		if(bg == nd)
		{
			nn.cnt += val;
			int ret = nodes.size();
			nodes.push_back(nn);
			return ret;
		}
		int md = (bg + nd) >> 1;
 
		if(pos <= md)
			nn.l = upd(nn.l, bg, md, pos, val);
		else
			nn.r = upd(nn.r, md + 1, nd, pos, val);
		
		nn.cnt = nodes[nn.l].cnt + nodes[nn.r].cnt;
 
		int ret = nodes.size();
		nodes.push_back(nn);
		return ret;
	}
 
	int get(int v, int bg, int nd, int a, int b)
	{
		if(!v)
			return 0;
		if(b < bg || nd < a)
			return 0;
		if(a <= bg && nd <= b)
			return nodes[v].cnt;
		int md = (bg + nd) >> 1;
		return get(nodes[v].l, bg, md, a, b) + get(nodes[v].r, md + 1, nd, a, b);
	}
};
