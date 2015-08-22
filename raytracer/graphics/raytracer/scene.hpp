#pragma once

namespace rt {

struct Scene {
	Array<Object*, 100> objects;
	Array<Light3d*, 100> lights; // TODO: array of lights!
	Color bground;
	Scene(Color background)
	: objects(), lights(), bground(background) {}
	void addObject(Object *obj){
		objects.push_back(obj);
	}
	void addLight(Light3d *light){
		lights.push_back(light);
	}
	void setBackground(Color col){
		bground = col;
	}
};

} // namespace rt