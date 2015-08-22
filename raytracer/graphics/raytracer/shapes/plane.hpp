#pragma once

namespace rt {

class Plane : public Object {
public:
	Vec3d point;
	Vec3d normal;
	Plane(Vec3d point, Vec3d normal)
	: point(point), normal(normal) {}
	virtual double intersect(Ray3d& ray) const {
		double d = normal.dot(ray.dir);
		if (d == 0)
			return -1;
		double nx = normal.x;
		double ny = normal.y;
		double nz = normal.z;
		double Psx = point.x;
		double Psy = point.y;
		double Psz = point.z;
		double dvx = ray.dir.x;
		double dvy = ray.dir.y;
		double dvz = ray.dir.z;
		double Pex = ray.pos.x;
		double Pey = ray.pos.y;
		double Pez = ray.pos.z;
		double t = -1.0 * ((nx * Pex - nx * Psx + ny * Pey - ny * Psy + nz * Pez - nz * Psz) / (nx * dvx + ny * dvy + nz * dvz));
		if(t > EPSd) 
			return t;
		if(t > 0) 
			return 0;
		else
			return -1;
	}
	virtual Vec3d normalAt(Vec3d&) const {
		return normal;
	}
};

} // namespace rt