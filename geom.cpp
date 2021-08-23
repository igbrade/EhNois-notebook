template<typename T>
struct Vec2
{
	T x, y;
	Vec2(const T& x = 0, const T& y = 0) : x(x), y(y)
	{}

	Vec2<T> operator+(const Vec2<T> &o) const { return Vec2(x + o.x, y + o.y); }
	Vec2<T> operator-(const Vec2<T> &o) const { return Vec2(x - o.x, y - o.y); }
	Vec2<T> operator-() const { return Vec2(-x, -y); }
	Vec2<T> operator*(const T& k) const { return Vec2(k * x, k * y); }
	Vec2<T> operator/(const T& k) const { return Vec2(x / k, y / k); }

	T len2() const { return x * x + y * y; }
	double len() const { return hypot(x, y); }

	static double angle(const Vec2<T> &a, const Vec2<T> &b) { return atan2(cross(a, b), dot(a, b)); }

	static T cross(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.y - b.x * a.y; }
	static T dot(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.x + a.y * b.y; }

	static T area2(const Vec2<T> &a, const Vec2<T> &b, const Vec2<T> &c) { return cross(b - a, c - a); }
	static bool collinear(const Vec2<T> &a, const Vec2<T> &b, const Vec2<T> &c) { return area(a, b, c) == 0; }

	static Vec2<T> rotate(const Vec2<T> &v, double angle)
	{
		double s = sin(angle), c = cos(angle);
		return Vec2<T>(v.x * c - v.y * s, v.x * s + v.y * c);
	}
};

const double PI = 3.141592653589;
double toDeg(double rad) { return rad / PI * 180; }
double toRad(double deg) { return deg / 180 * PI; }

typedef Vec2<long long> Vec2i;
typedef Vec2<double> Vec2d;

template<typename T>
vector<Vec2<T>> convexHull(const vector<Vec2<T>> &pts)
{
	if(pts.size() <= 2)
		return pts;

	vector<Vec2i> hull;
	for(int j = 0; j < 2; ++j)
	{
		size_t temp = hull.size();
		for(int i = (j ? pts.size() - 1 : 0); 0 <= i && i < (int)pts.size(); i += (j ? -1 : 1))
		{
			while(hull.size() >= 2 + temp && Vec2i::cross(hull.back() - hull[hull.size() - 2], pts[i] - hull.back()) <= 0)
				hull.pop_back();

			hull.push_back(pts[i]);
		}
		hull.pop_back();
	}
	return hull;
}

template<typename T>
vector<Vec2<T>> sortAndHull(vector<Vec2<T>> &pts)
{
	sort(pts.begin(), pts.end(), [](const Vec2i &a, const Vec2i &b)
	{
		if(a.x != b.x)
			return a.x < b.x;
		return a.y < b.y;
	});
	return convexHull(pts);
}
