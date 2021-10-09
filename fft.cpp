typedef complex<double> cd;
const double PI = 3.14159265358979;

int bitrev(int x, int nbits)
{
	int ret = 0;
	for(int i = 0; i < nbits; ++i)
	{
		if(x & (1 << i))
			ret |= (1 << (nbits - i - 1));
	}
	return ret;
}

void fft(cd p[], int n, bool inv)
{
	int lgn = __lg(n);
	for(int i = 0, j; i < n; ++i)
	{
		j = bitrev(i, lgn);
		if(i < j)
			swap(p[i], p[j]);
	}

	for(int sz = 2; sz <= n; sz <<= 1)
	{
		double ang = (inv ? -2 : 2) * PI / sz;
		cd w1(cos(ang), sin(ang)), cur, a, b;
		for(int i = 0; i < n; i += sz)
		{
			cur = 1;
			for(int j = 0; 2 * j < sz; ++j)
			{
				a = p[i + j], b = p[i + j + sz / 2] * cur;
				p[i + j] = a + b;
				p[i + j + sz / 2] = a - b;
				cur *= w1;
			}
		}
	}

	if(inv)
	{
		for(int i = 0; i < n; ++i)
			p[i] /= n;
	}
}

inline int ceillog(int x)
{
	return __lg(x - 1) + 1;
}

inline void fft(vector<cd> &v, bool inv) { fft(v.data(), v.size(), inv); }
void multiply(vector<cd> &a, vector<cd> &b)
{
	int tmp = a.size() + b.size();
	int n = (1 << ceillog(tmp));
	a.resize(n);
	b.resize(n);
	fft(a, false);
	fft(b, false);
	for(int i = 0; i < n; ++i)
		a[i] *= b[i];
	fft(a, true);
	a.resize(tmp);

	for(int i = 0; i < (int)a.size(); ++i)
		a[i] = (int)round(a[i].real());
}
