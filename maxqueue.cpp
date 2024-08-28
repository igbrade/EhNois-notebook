template<typename T>
struct maxqueue
{
	static constexpr T INF = numeric_limits<T>::min();
 
	T mna = INF;
	vector<T> a, b;
	void push(T x)
	{
		mna = max(mna, x);
		a.push_back(x);
	}
	void pop()
	{
		if(b.empty())
		{
			mna = INF;
			while(!a.empty())
				b.push_back(max(a.back(), b.empty() ? INF : b.back())), a.pop_back();
		}
		b.pop_back();
	}
	T getmax()
	{
		return max(mna, b.empty() ? INF : b.back());
	}
	void clear()
	{
		a.clear(), b.clear(), mna = INF;
	}
};
