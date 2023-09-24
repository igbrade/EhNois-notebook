struct IntContainer
{
	set<pair<int, int>> ints;
	multiset<int> szs;
 
	void erase(int x)
	{
		auto it = ints.upper_bound(make_pair(x, INF));
		if(it != ints.begin())
		{
			it = prev(it);
			pair<int, int> aux = *it;
			if(x < aux.first || aux.second < x)
				return;
			ints.erase(it);
			szs.erase(szs.find(aux.second - aux.first + 1));
			if(aux.first < x && x < aux.second)
			{
				ints.emplace(aux.first, x - 1);
				szs.insert((x - 1) - aux.first + 1);
				ints.emplace(x + 1, aux.second);
				szs.insert(aux.second - (x + 1) + 1);
			}
			else
			{
				if(x == aux.first)
					aux.first++;
				else if(x == aux.second)
					aux.second--;
				if(aux.first <= aux.second)
				{
					ints.insert(aux);
					szs.insert(aux.second - aux.first + 1);
				}
			}
		}
	}
 
	void insert(int x)
	{
		auto it = ints.upper_bound(make_pair(x, 0));
		if(it == ints.begin())
		{
			pair<int, int> nxt = *it;	
			if(nxt.first == x + 1)
			{
				ints.erase(it);
				ints.emplace(x, nxt.second);
				szs.erase(szs.find(nxt.second - nxt.first + 1));
				szs.insert(nxt.second - x + 1);
			}
			else
			{
				ints.emplace(x, x);
				szs.insert(1);
			}
		}
		else if(it == ints.end())
		{
			pair<int, int> prv = *prev(it);
			if(prv.second == x - 1)
			{
				ints.erase(prev(it));	
				ints.emplace(prv.first, x);
				szs.erase(szs.find(prv.second - prv.first + 1));
				szs.insert(x - prv.first + 1);
			}
			else
			{
				ints.emplace(x, x);
				szs.insert(1);
			}
		}
		else
		{
			pair<int, int> nxt = *it;
			pair<int, int> prv = *prev(it);
			if(prv.second == x - 1 && nxt.first == x + 1)
			{
				ints.erase(nxt);
				szs.erase(szs.find(nxt.second - nxt.first + 1));
				ints.erase(prv);
				szs.erase(szs.find(prv.second - prv.first + 1));
				ints.emplace(prv.first, nxt.second);
				szs.insert(nxt.second - prv.first + 1);
			}
			else if(prv.second == x - 1)
			{
				ints.erase(prev(it));
				szs.erase(szs.find(prv.second - prv.first + 1));
				ints.emplace(prv.first, x);
				szs.insert(x - prv.first + 1);
			}
			else if(nxt.first == x + 1)
			{
				ints.erase(it);
				szs.erase(szs.find(nxt.second - nxt.first + 1));
				ints.emplace(x, nxt.second);
				szs.insert(nxt.second - x + 1);
			}
			else
			{
				ints.emplace(x, x);
				szs.insert(1);
			}
		}		
	}
 
	pair<int, int> getInterval(int x)
	{
		auto it = ints.upper_bound(make_pair(x, INF));
		assert(it != ints.begin());
		it = prev(it);
		assert(it->first <= x && x <= it->second);
		return *it;
	}
};
