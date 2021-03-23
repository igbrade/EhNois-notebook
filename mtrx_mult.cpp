template<size_t sz>
struct mtrx
{
	union
	{
		int data[sz * sz];
		int elements[sz][sz];
	};

	mtrx(int diagonal) : data{0}
	{
		for(int i = 0; i < sz; ++i)
			elements[i][i] = diagonal;
	}

	template<typename... Args>
	mtrx(Args&&... args) : data{args...}
	{
	}

	mtrx<sz> operator* (const mtrx<sz> &o) const
	{
		mtrx<sz> ret;
		for(int i = 0; i < sz; ++i)
			for(int j = 0; j < sz; ++j)
			{
				for(int k = 0; k < sz; ++k)
					ret.elements[i][j] += elements[i][k] * o.elements[k][j];
			}
		return ret;
	}

	mtrx<sz> &operator*=(const mtrx<sz> &o)
	{
		(*this) = (*this) * o;
		return (*this);
	}
};

template<size_t sz>
string to_string(const mtrx<sz> &m)
{
	stringstream s;
	s << "{\n";
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < sz; ++j)
			s << m.elements[i][j] << " ";
		s << "\n";
	}
	s << "}\n";
	return s.str();
}

template<typename T>
T powm(const T &b, int e)
{
	T ret = 1;
	for(int i = __lg(e); i >= 0; --i)
	{
		ret = ret * ret;
		if(e & (1 << i))
			ret = b * ret;
	}
	return ret;
}
