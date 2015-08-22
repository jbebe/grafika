#pragma once

namespace rt {

struct Intersection {
	Ray3d ray;
	Object *obj;
	double distance;
	double light_angle;
	Vec3d pos;
	Vec3d normal;
	Intersection(Ray3d &ray, Object* obj_handle = NULL, double distance = 0) 
	: ray(ray), obj(obj_handle), distance(distance) {}
	Intersection(const Intersection &cpy) 
	: ray(cpy.ray), obj(cpy.obj), distance(cpy.distance) {}
	inline bool happened() const { return obj != NULL; }
};

} // namespace rt