
#include "intersection.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "misc.hpp"
#include "object.hpp"

#pragma once

namespace rt {

struct Engine {
private:
	Scene *scene;
	const int width;
	const int height;
	const Cam3d camera;
	const int samples;
	const int max_reflections;
	const int depth_complexity;
	const double dispersion;
	const double image_scale;
	int render_line;
public:
	Engine(Scene *scene_ptr, int width, int height, Cam3d camera, 
		int samples = 1, int max_reflections = 4, int depth_complexity = 2, 
		double dispersion = 40.0, double image_scale = 1)
	: scene(scene_ptr), width(width), height(height), camera(camera),
		samples(samples), max_reflections(max_reflections), 
		depth_complexity(depth_complexity), dispersion(dispersion), 
		image_scale(image_scale), render_line(0)
	{}
	
	double render_slice(rtti_image * const image){
		for(int w = 0; w < width; w++){
			image->data[render_line*width + w] = castRaySamples(w, render_line);
		}
		++render_line;
		return (double)render_line/height;
	}
	
	void render(rtti_image * const image){
		for(int h = 0; h < height; h++){
			for(int w = 0; w < width; w++){
				image->data[h*width + w] = castRaySamples(w, h);
			}
		}
	}
	
	Color castRaySamples(const double w, const double h){
		Color color;
		if (samples == 1){
			color += castUniqueRay(w, h);
		}
		else {
			const double sample_weight = 1.0/(samples*samples);
			const double interval_from = -0.3; // magic number
			const double interval_to = 0.3;
			const double step = (interval_to-interval_from)/(samples-1);
			for (double xoff = interval_from; xoff <= interval_to; xoff += step) {
			   for (double yoff = interval_from; yoff <= interval_to; yoff += step) {
				  color += castUniqueRay(w + xoff, h + yoff)*sample_weight;
			   }
			}
		}
		return color;
	}
	
	Color castUniqueRay(const double x, const double y){
		// TODO: implement camera position and direction
		Ray3d ray(
			Vec3d(x - width/2, y - height/2, 0),
			ray.dir = (ray.pos-camera.pos).normalize(),
			max_reflections + 1
		);
		if (depth_complexity == 1) {
			return getLighting(ray);
		}
		else {
			Color color;
			double inv_depth_complexity = 1.0/depth_complexity;
			for (int i = 0; i < depth_complexity; i++) {
				Ray3d new_ray(ray.pos, 0, max_reflections + 1);
				Vec3d disturbance(
					((double)rand()/RAND_MAX) * dispersion,
					((double)rand()/RAND_MAX) * dispersion,
					0
				);
				Vec3d focus_point(ray.pos.x*1.8, ray.pos.y*1.8, 450);

				new_ray.pos += disturbance;
				new_ray.dir = (focus_point - new_ray.pos).normalize();
				color += (getLighting(new_ray) * inv_depth_complexity);
			}
			return color;
		}
	}
	
	Intersection getClosestObject(Ray3d &ray){
		double dist;
		double min_dist = 1e+37;
		int closest_idx = -1;
		for (unsigned int i = 0, length = scene->objects.size(); i < length; i++){
			dist = scene->objects[i]->intersect(ray);
			if (dist > EPSd && dist < min_dist){
				min_dist = dist;
				closest_idx = i;
			}
		}
		if (closest_idx != -1){
			return Intersection(ray, scene->objects[closest_idx], min_dist);
		}
		else {
			return Intersection(ray, NULL);
		}
	}

	Color getAmbientLight(Intersection &isection){
		// TODO: remove light_angle
		isection.pos = isection.ray.pos + (isection.ray.dir * isection.distance); // ez lesz a pontos helye
		isection.normal = isection.obj->normalAt(isection.pos); // a felület normálisa
		
		//Ray3d ray_to_light; // egy sugár, ami a metszéspontból a fény felé fog majd nézni
		//ray_to_light.pos = isection.pos + isection.normal * 0.01; // kicsit "arrébb" húzzuk, hogy biztos ne legyen a gömbünkben a sugár kezdőpontja
		//ray_to_light.dir = ; // beállítjuk az irányvektorát
		double light_angle = isection.normal.dot((scene->lights[0]->pv - isection.pos).normalize()); // ez a cos(theta)
		if(light_angle < 0){ // csak a 0..1 tartományt vesszük figyelembe
			light_angle = 0;
		}
		isection.light_angle = light_angle;
		return isection.obj->color; // az elmetszettnek a színét vesszük.
	}
	
	Color &getReflection(Color &color, Intersection &isection){
		// reflection
		if (isection.obj->refl){
			isection.obj->setFresnel(isection.light_angle);
			double angle_cam = -1.0 * isection.ray.dir.dot(isection.normal);
			Ray3d reflect_ray; // visszavert sugarunk
			reflect_ray.pos = isection.pos + isection.normal * EPSd; // kezdeti pontja a metszés pont egy kicsit eltolva
			reflect_ray.dir = isection.ray.dir + isection.normal * 2 * angle_cam; // az irányát így számoljuk
			reflect_ray.dir.normalize(); // normalizáljuk
			reflect_ray.reflection_count = isection.ray.reflection_count - 1;
			Color reflect_color = getLighting(reflect_ray); // elindítjuk az új sugarunkat
			color = color + reflect_color; // majd az eredményt hozzáadjuk az eddig színhez
			
			color = color * isection.obj->fresnel * isection.light_angle;
			//out_color = out_color * scene->light.color * isection.light_angle;
		}
		return color;
	}
	
	Color &getRefracted(Color &out_color, Intersection &isection){
		// refraction
		if (isection.obj->refr){
			double refrac_idx = isection.obj->frac.r;  // vesszük a törésmutatót
			Vec3d tnormal = isection.normal; // csinálunk egy temporális normálvektort
			double cos_refrac = -1 * isection.ray.dir.dot(tnormal); // vesszük a beesési szög koszinuszának -1-szeresét
			if (cos_refrac < 0){ // ha ez kisebb, mint 0, akkor a testünk
				refrac_idx = 1.0/refrac_idx; // vesszük a törésmutató reciprokát
				tnormal = tnormal * -1; // és a normálvektort megfordítjuk
				cos_refrac = -1 * isection.ray.dir.dot(tnormal); // majd megint 
			}
			double disc = 1-((1-cos_refrac*cos_refrac)/(refrac_idx*refrac_idx)); // megnézzük, hogy sikerül-e betörni
			if (disc > 0){
				Ray3d refract_ray; // létrehozunk egy új sugarat
				refract_ray.pos = isection.pos + tnormal * EPSd * -1; // az új sugár kezdőpontja ne pontosan
				// ott legyen, ahol a beérkezési pont a
				// számolási pontatlanságok elkerülése végett
				refract_ray.dir = isection.ray.dir / refrac_idx + tnormal * (cos_refrac / refrac_idx - sqrt(disc)); // a tört sugár iránya
				refract_ray.dir.normalize(); // normalizáljuk
				refract_ray.reflection_count = isection.ray.reflection_count - 1;
				
				Color color = getLighting(refract_ray); // rekurzívan elindítjuk
				out_color = color;
			}
		}
		return out_color;
	}
	
	Color getLighting(Ray3d &ray){
		Color output_color(scene->bground);
		if (ray.reflection_count != 0){
			Intersection isection(getClosestObject(ray));
			if (isection.happened()){
				output_color = getAmbientLight(isection);
				output_color = getDiffuseAndSpecular(output_color, isection);
				output_color = getReflection(output_color, isection);
				output_color = getRefracted(output_color, isection);
			}
		}
		return output_color;
	}
	
	bool isShadow(Ray3d& ray, double lightDistance){
		Intersection isection = getClosestObject(ray);
		return isection.obj->refr == false && isection.happened() && isection.distance < lightDistance;
	}
	
	Color getSpecularLighting(Intersection& intersection, Light3d &light) {
		Vec3d incomVector    = intersection.pos - light.pv;
		Vec3d incomVectorN   = incomVector.normalize();
		double myDot = - incomVectorN.dot(intersection.normal);
		double myLen = 2.0f * myDot;
		Vec3d tempNormal     = intersection.normal * myLen;
		Vec3d reflectVector  = tempNormal + incomVectorN;
		Vec3d reflectVectorN = reflectVector.normalize();
		double dotprod = -reflectVectorN.dot(incomVectorN);
		double mySpec = std::max<double>(dotprod, 0);
		mySpec = pow(mySpec, 5.0);
		Color specularColor(1, 1, 1);
		specularColor = specularColor * mySpec * 0.8;
		return specularColor * specularColor;
	}
	
	Color getDiffuseAndSpecular(const Color &color, Intersection &intersection){
		Color diffuseColor;
		Color specularColor;

		for (unsigned int i = 0, length = scene->lights.size(); i < length; i++){
			Light3d &light = *(scene->lights[i]);
			Vec3d lightOffset = light.pv - intersection.pos;
			double lightDistance = lightOffset.length();
			Vec3d lightDirection = lightOffset.normalize();
			double dotProduct = intersection.normal.dot(lightDirection);
			if (dotProduct >= 0.0){
				Ray3d shadowRay = Ray3d(intersection.pos, lightDirection, 1);
				if (isShadow(shadowRay, lightDistance)){
					continue;
				}
				diffuseColor = (diffuseColor + (color * dotProduct)) * 0.7;
				specularColor += getSpecularLighting(intersection, light);
			}
		}

		return diffuseColor + specularColor;
	}
};

} // namespace rt