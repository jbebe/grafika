#pragma once

namespace rt {

// Raytracer pseudo object
struct Object {
	Color color; // color
	Color fresnel; // fresnel factor
	Color frac; // fracture ratio
	Color ext; // extinction coefficient
	bool refl; // is reflective
	bool refr; // is refractive
	Object()
	: color(0, 0, 0), fresnel(0, 0, 0), frac(0, 0, 0),
		ext(0, 0, 0), refl(false), refr(false) {}
	virtual double intersect(Ray3d &ray) const = 0;
	virtual Vec3d normalAt(Vec3d &isect_p) const = 0;
	void setFresnel(double costheta){
		fresnel.r = (pow(frac.r-1,2)+pow(ext.r,2)+pow(1-costheta,5)*4*frac.r)/(pow(frac.r+1, 2)+pow(ext.r,2));
		fresnel.g = (pow(frac.g-1,2)+pow(ext.g,2)+pow(1-costheta,5)*4*frac.g)/(pow(frac.g+1, 2)+pow(ext.g,2));
		fresnel.b = (pow(frac.b-1,2)+pow(ext.b,2)+pow(1-costheta,5)*4*frac.b)/(pow(frac.b+1, 2)+pow(ext.b,2));
	}
};

} // namespace rt