#pragma once

struct rtti_image {
	Color *data;
	rtti_image(int width, int height)
	: data(new Color[width*height]) {}
	~rtti_image(){ delete[] data; }
};