#pragma once

//
// 3D vector
//
template <typename Real>
class Vec3 {
	typedef Vec3<Real> _vec_t;
public:
	Real x, y, z;
	Vec3(Real x = 0, Real y = 0, Real z = 0): x(x), y(y), z(z) {}
	Vec3(const _vec_t &cpy): x(cpy.x), y(cpy.y), z(cpy.z) {}
public:
	inline _vec_t &operator = (const _vec_t &v){
		x = v.x; y = v.y; z = v.z;
		return *this;
	}
	inline _vec_t &operator *= (Real a){
		x *= a; y *= a; z *= a;
		return *this;
	}
	inline _vec_t &operator += (const _vec_t &v){
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	inline _vec_t &operator -= (const _vec_t &v){
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	inline _vec_t &operator %= (const _vec_t &v){
		x = y*v.z - z*v.y;
		y = z*v.x - x*v.z; 
		z = x*v.y - y*v.x;
		return *this;
	}
	inline _vec_t operator*(Real a) const {
		return Vec3(x * a, y * a, z * a); 
	}
	// scalar on left
	friend inline _vec_t operator*(Real a, const _vec_t &v){
		return Vec3(v.x * a, v.y * a, v.z * a); 
	}
	inline _vec_t operator+(const _vec_t& v) const {
		return Vec3(x + v.x, y + v.y, z + v.z); 
	}
	inline _vec_t operator-(const _vec_t& v) const {
		return Vec3(x - v.x, y - v.y, z - v.z); 
	}
	inline _vec_t operator / (Real a){
		a = 1/a; // NOTE: divide by zero dont care
		return _vec_t(x*a, y*a, z*a);
	}
	// dot product
	inline Real dot(const _vec_t& v) const {
		return (x * v.x + y * v.y + z * v.z); 
	}
	// cross product
	inline _vec_t cross(const _vec_t& v) const {
		return Vec3(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	inline Real length() const;
	// normalize vector
	_vec_t &normalize(){
		if (abs(x) > 1 || abs(y) > 1 || abs(z) > 1){
			Real len = length();
			x /= len; y /= len; z /= len;
		}
		return *this;
	}
	static Real abs(Real);
	_vec_t reflect(_vec_t normal) {
		return normal * 2.0 * dot(normal) - (*this);
	}
};

template <> float Vec3<float>::abs(float num){ return fabsf(num); }
template <> double Vec3<double>::abs(double num){ return fabs(num); }
template <> long double Vec3<long double>::abs(long double num){ return fabsl(num); }

template <>
float Vec3<float>::length() const {
	return sqrtf(x*x + y*y + z*z);
}

template <>
double Vec3<double>::length() const {
	return sqrt(x*x + y*y + z*z);
}

template <>
long double Vec3<long double>::length() const {
	return sqrtl(x*x + y*y + z*z);
}

// alias for clarity
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;