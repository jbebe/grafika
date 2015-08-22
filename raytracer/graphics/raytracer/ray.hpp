#pragma once

namespace rt {

template <typename Real>
struct Ray3 {
	Vec3<Real> pos;
	Vec3<Real> dir;
	int reflection_count;
	// Material *
	// fractional direction
	Ray3(
		Vec3<Real> pos = Vec3<Real>(), 
		Vec3<Real> dir = Vec3<Real>(), 
		int refl_count = 0 /* maybe i notice it */)
		: pos(pos), dir(dir), reflection_count(refl_count) {}
};

typedef Ray3<float> Ray3f;
typedef Ray3<double> Ray3d;

} // namespace rt