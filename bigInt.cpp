//Checked against
//https://judge.yosupo.jp/problem/addition_of_big_integers
//https://cses.fi/problemset/task/1728/

//Multiply is not fast enough, its complexity is O(N^(log3_2))
//It can be faster by employing fft
//Division is also pretty slow
//But at least, everything seems to work

struct BigInt
{
	static const int BASE = 1e9;
	bool negative = false;
	vector<int> digits;
	
	BigInt(int x = 0)
	{
		if(x < 0)
		{
			digits = {-x};
			negative = true;
		}
		else if(x != 0)
			digits = {x};
	}

	BigInt abs() const
	{
		BigInt ret = (*this);
		ret.negative = false;
		return ret;
	}

	bool operator<(const BigInt &o) const
	{
		if(negative != o.negative)
			return negative > o.negative;
		if(negative)
			return abs() > o.abs();
		if(digits.size() != o.digits.size())
			return digits.size() < o.digits.size();
		for(int i = digits.size() - 1; i >= 0; --i)
			if(digits[i] != o.digits[i])
				return (digits[i] < o.digits[i]);
		return false;
	}
	bool operator>(const BigInt &o) const
	{
		return o < (*this);
	}
	bool operator>=(const BigInt &o) const
	{
		return !((*this) < o);
	}
	bool operator<=(const BigInt &o) const
	{
		return !((*this) > o);
	}
	bool operator==(const BigInt &o) const
	{
		if(digits.size() != o.digits.size())
			return false;
		if(negative != o.negative)
			return false;
		for(int i = 0; i < digits.size(); ++i)
			if(digits[i] != o.digits[i])
				return false;
		return true;
	}
	bool operator!=(const BigInt &o) const
	{
		return !((*this) == o);
	}

	BigInt operator-() const
	{
		if(digits.empty())
			return (*this);
		BigInt ret = (*this);
		ret.negative = !ret.negative;
		return ret;
	}

	BigInt operator+(const BigInt &o) const
	{
		if(negative != o.negative)
		{
			return (*this) - (-o);
		}

		BigInt ret;
		ret.negative = negative;
		ret.digits.resize(max(digits.size(), o.digits.size()) + 1);

		int carry = 0;
		for(int i = 0; i < ret.digits.size(); ++i)
		{
			ll cur = carry;
			if(i < digits.size())
				cur += digits[i];
			if(i < o.digits.size())
				cur += o.digits[i];
			ret.digits[i] = cur % BASE;
			carry = cur / BASE;
		}

		while(!ret.digits.empty() && ret.digits.back() == 0)
			ret.digits.pop_back();
		return ret;
	}

	BigInt operator-(const BigInt &o) const
	{
		if(negative != o.negative)
		{
			return (*this) + (-o);
		}
		if(abs() < o.abs())
		{
			return -(o - (*this));
		}
		BigInt ret;
		ret.negative = negative;
		ret.digits.resize(max(digits.size(), o.digits.size()) + 1);

		int carry = 0;
		for(int i = 0; i < ret.digits.size(); ++i)
		{
			ll cur = carry;
			if(i < digits.size())
				cur += digits[i];
			if(i < o.digits.size())
				cur -= o.digits[i];
			ret.digits[i] = (cur < 0 ? cur + BASE : cur);
			carry = (cur < 0 ? -1 : 0);

		}

		while(!ret.digits.empty() && ret.digits.back() == 0)
			ret.digits.pop_back();
		return ret;
	}

	BigInt multiply(const BigInt &o) const
	{
		BigInt ret;
		ret.negative = negative ^ o.negative;
		ret.digits.resize(digits.size() * o.digits.size() + 1);
		for(int i = 0; i < digits.size(); ++i)
		{
			ll carry = 0;
			for(int j = 0; j < o.digits.size(); ++j)
			{
				ll cur = 1LL * digits[i] * o.digits[j] + carry;
				ret.digits[i + j] += cur % BASE;
				carry = cur / BASE;
				if(ret.digits[i + j] > BASE)
				{
					ret.digits[i + j] -= BASE;
					carry += 1;
				}
			}
			if(carry)
				ret.digits[i + o.digits.size()] += carry;
		}

		while(!ret.digits.empty() && ret.digits.back() == 0)
			ret.digits.pop_back();

		return ret;
	}
	
	BigInt multiplyKaratsuba(const BigInt &o) const
	{
		BigInt a, b, c, d;
		int off = max(digits.size(), o.digits.size()) / 2;

		b.digits = vector<int>(digits.begin(), digits.begin() + min(off, (int)digits.size()));
		a.digits = vector<int>(digits.begin() + min(off, (int)digits.size()), digits.end());

		d.digits = vector<int>(o.digits.begin(), o.digits.begin() + min(off, (int)o.digits.size()));
		c.digits = vector<int>(o.digits.begin() + min(off, (int)o.digits.size()), o.digits.end());

		//(a * off + b) * (c * off + d)
		//a * c * off * off + b * d + a * off * d + c * off2 * b
		//a * c * off^2 + (a * d + c * b) * off + b * d
		//(a + b) * (c + d) = ac + bd + ad + bc
		
		BigInt abcd = (a + b) * (c + d);
		BigInt ac = a * c;
		BigInt bd = b * d;

		BigInt ret = abcd - ac - bd;

		reverse(all(ret.digits));
		ret.digits.resize(ret.digits.size() + off);
		reverse(all(ret.digits));

		reverse(all(ac.digits));
		ac.digits.resize(ac.digits.size() + 2 * off);
		reverse(all(ac.digits));

		ret = ret + ac + bd;

		ret.negative = negative ^ o.negative;
		return ret;
	}

	BigInt operator*(const BigInt &o) const
	{
		if(digits.size() <= 32 || o.digits.size() <= 32)
			return (*this).multiply(o);
		return (*this).multiplyKaratsuba(o);
	}

	BigInt operator/(const BigInt &o) const
	{
		BigInt ret;
		ret.digits.push_back(1);
		ret.negative = negative ^ o.negative;
		do
		{
			ret.digits.back() = 0;
			ret.digits.push_back(1);
		} while(ret.abs() * o.abs() < abs());
		for(int i = ret.digits.size() - 1; i >= 0; --i)
		{
			ret.digits[i] = 0;
			for(int j = 29; j >= 0; --j)
			{
				if((1 << j) + ret.digits[i] > BASE)
					continue;
				ret.digits[i] += (1 << j);
				if(ret.abs() * o.abs() > abs())
					ret.digits[i] -= (1 << j);
			}
		}
	
		while(!ret.digits.empty() && ret.digits.back() == 0)
			ret.digits.pop_back();
 
		return ret;
	}
 
	BigInt operator%(const BigInt &o) const
	{
		BigInt quotient = (*this) / o;
		return (*this) - quotient * o;
	}
};

int viewtoi(const string_view &s)
{
	int ret = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		ret = 10 * ret + (c - '0');
	}
	return ret;
}

BigInt strToBig(const string_view &s)
{
	BigInt ret;
	if(s.empty())
		return ret;
	if(s[0] == '-')
	{
		ret = strToBig(s.substr(1));
		ret.negative = true;
		return ret;
	}
	for(int i = 0; i < s.size(); i += 9)
	{
		int lastindex = s.size() - i;
		int firstindex = max((int)s.size() - i - 9, 0);
		string_view group = s.substr(firstindex, lastindex - firstindex);
		ret.digits.push_back(viewtoi(group));
	}
	return ret;
}

ostream &operator<<(ostream &str, const BigInt &x)
{
	if(x.negative && !x.digits.empty())
		str << '-';
	if(x.digits.empty())
		str << '0';
	else
		str << x.digits.back();
	for(int i = x.digits.size() - 2; i >= 0; --i)
	{
		str << setfill('0') << setw(9);
		str << x.digits[i];
	}
	return str;
}

istream &operator>>(istream &str, BigInt &x)
{
	string s;
	str >> s;
	x = strToBig(s);
	return str;
}
