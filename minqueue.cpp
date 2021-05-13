const long long LINF = 0x3f3f3f3f3f3f3f3f;

template<typename T>
struct minqueue
{
	T mna = LINF;
	vector<T> a, b;
	void push(T x)
	{
		mna = min(mna, x);
		a.push_back(x);
	}
	void pop()
	{
		if(b.empty())
		{
			mna = LINF;
			while(!a.empty())
				b.push_back(min(a.back(), b.empty() ? LINF : b.back())), a.pop_back();
		}
		b.pop_back();
	}
	T getmin()
	{
		return min(mna, b.empty() ? LINF : b.back());
	}
	void clear()
	{
		a.clear(), b.clear(), mna = LINF;
	}
};