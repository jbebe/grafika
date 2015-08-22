#pragma once

namespace rt {

const float FOV_D = 60;
const float FOV_R = (FOV_D/2.0)*(PI/180.0);

template <typename Real>
struct Light3 {
	Color color;
	Vec3<Real> pv;
	Light3(Color color = Color(), Vec3<Real> pos = Vec3<Real>()): color(color), pv(pos) {}
};

typedef Light3<float> Light3f;
typedef Light3<double> Light3d;

template <typename Real>
struct Cam3 : public Vec3<Real> {
	Vec3<Real> pos;
	Vec3<Real> lookat;
	Cam3(Vec3<Real> pos = Vec3<Real>(), Vec3<Real> lookat = Vec3<Real>()): pos(pos), lookat(lookat) {}
};

typedef Cam3<float> Cam3f;
typedef Cam3<double> Cam3d;

} // namespace rt