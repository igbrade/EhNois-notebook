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
	
	//Computes the squared length
	T len2() const { return x * x + y * y; }
	//Computes length of vector
	double len() const { return hypot(x, y); }

	static double angle(const Vec2<T> &a, const Vec2<T> &b) { return atan2(cross(a, b), dot(a, b)); }

	static T cross(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.y - b.x * a.y; }
	static T dot(const Vec2<T> &a, const Vec2<T> &b) { return a.x * b.x + a.y * b.y; }
	
	//Computes twice the SIGNED area of triangle 
	static T area2(const Vec2<T> &a, const Vec2<T> &b, const Vec2<T> &c) { return cross(b - a, c - a); }
	static bool collinear(const Vec2<T> &a, const Vec2<T> &b, const Vec2<T> &c) { return area(a, b, c) == 0; }
	
	//Rotates a vector around origin
	static Vec2<T> rotate(const Vec2<T> &v, double angle)
	{
		double s = sin(angle), c = cos(angle);
		return Vec2<T>(v.x * c - v.y * s, v.x * s + v.y * c);
	}
};

//Geogebra print functions

template<typename T>
void printGgbVec(const Vec2<T> &v)
{
	cout << "Vector((" << v.x << ", " << v.y << "))\n";
}

template<typename T>
void printGgbPt(const Vec2<T> &v)
{
	cout << "(" << v.x << ", " << v.y << ")\n";
}

template<typename T>
void printGgbPolygon(const vector<Vec2<T>> &poly)
{
	cout << "Polygon(";
	bool fst = true;
	for(const Vec2<T>& k : poly)
	{
		if(!fst)
			cout << ", ";
		cout << "(" << k.x << ", " << k.y << ")";
		fst = false;
	}
	cout << ")\n";
}

template<typename T>
T sign(T x) { return x >= 0 ? x > 0 ? 1 : 0 : -1; }

const double PI = 3.141592653589;
double toDeg(double rad) { return rad / PI * 180; }
double toRad(double deg) { return deg / 180 * PI; }

typedef Vec2<long long> Vec2i;
typedef Vec2<double> Vec2d;

//Computes convex hull of sorted vector of points
//The original vector is not modified
//O(N)
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

//Sorts and computes convex hull of vector of points
//The original vector is modified (sorted only)
//O(N log(N))
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

//Check if a triangle formed by (a, b, c) contains p
//O(1)
template<typename T>
bool contains(Vec2<T> a, Vec2<T> b, Vec2<T> c, Vec2<T> p)
{
	T aTot = abs(Vec2<T>::area(a, b, c));
	T a1 = abs(Vec2<T>::area(a, b, p));
	T a2 = abs(Vec2<T>::area(b, c, p));
	T a3 = abs(Vec2<T>::area(a, c, p));
	
	return a1 + a2 + a3 == aTot;
}

//Reorder polygon points so that first point is lexicographic minimum
//O(N)
template<typename T>
void reorder(vector<Vec2<T>> &poly)
{
	int n = poly.size();
	int mn = 0;
	for(int i = 1; i < n; ++i)
		if(poly[i].y < poly[mn].y || 
		(poly[i].y == poly[mn].y && poly[i].x < poly[mn].x))
			mn = i;
	rotate(poly.begin(), poly.begin() + mn, poly.end());
}

//Check if a convex polygon contains a point
//Assumes the points are order counter-clockwise and the first point
//is the lexicographic minimum
//O(log(N))
template<typename T>
bool contains(const vector<Vec2<T>> &poly, Vec2<T> pt)
{
	int n = poly.size();
 #if 0 //Bruteforce
	for(int i = 1; i < n - 1; ++i)
	{
		if(contains(poly[0], poly[i], poly[i + 1], pt))
		{
			return true;
		}
	}
	return false;
#else
	int bg = 1, nd = n - 2, md;
	while(bg < nd)
	{
		md = (bg + nd + 1) >> 1;
		if(Vec2<T>::cross(pt - poly[0], poly[md] - poly[0]) <= 0)
			bg = md;
		else
			nd = md - 1;
	}
 
	return contains(poly[0], poly[bg], poly[bg + 1], pt);
#endif
}

//Computes minkowski sum of polygons a, b
//O(N + M)
template<typename T>
vector<Vec2<T>> minkowskiSum(vector<Vec2<T>> a, vector<Vec2<T>> b)
{
	reorder(a);
	reorder(b);
	a.push_back(a[0]);
	a.push_back(a[1]);
	b.push_back(b[0]);
	b.push_back(b[1]);
	int n = a.size(), m = b.size();
	vector<Vec2<T>> ret;
	for(int i = 0, j = 0; i < n - 2 || j < m - 2;)
	{
		ret.push_back(a[i] + b[j]);
		T temp = Vec2<T>::cross(a[i + 1] - a[i], b[j + 1] - b[j]);
		if(temp > 0)
			i++;
		else if(temp == 0)
			i++, j++;
		else
			j++;
	}
 
	return ret;
}
