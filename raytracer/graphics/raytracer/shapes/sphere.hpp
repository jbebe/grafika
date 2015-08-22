#pragma once

namespace rt {

class Sphere : public Object {
public:
	Vec3d origo;
	double radius;
	Sphere(Vec3d origo, double radius): origo(origo), radius(radius) {}
	virtual double intersect(Ray3d& ray) const {
		double dx = ray.dir.x;
		double dy = ray.dir.y;
		double dz = ray.dir.z;
		double x0 = ray.pos.x;
		double y0 = ray.pos.y;
		double z0 = ray.pos.z;
		double cx = origo.x;
		double cy = origo.y;
		double cz = origo.z;
		double R = radius;
		double a = dx * dx + dy * dy + dz * dz;
		double b = 2 * dx * (x0 - cx) + 2 * dy * (y0 - cy) + 2 * dz * (z0 - cz);
		double c = cx * cx + cy * cy + cz * cz + x0 * x0 + y0 * y0 + z0 * z0 - 2 * (cx * x0 + cy * y0 + cz * z0) - (R * R);
		double d = b * b - 4 * a * c;
		if(d < 0){
			return -1; // miss
		}
		double t = ((-b - sqrt(d)) / (2 * a));
		if(t > EPSd){
			// ha nem csak számolási hibát vétettünk... 
			return t; // intersect
		}
		else {
			// ha a 0 epsilon sugarú környezetében van az érték, az nagy valószínűséggel számolási hiba
			return 0; // touch
		}
	}
	virtual Vec3d normalAt(Vec3d& isect_p) const {
		return (isect_p - origo).normalize();
	}
};

} // namespace rt