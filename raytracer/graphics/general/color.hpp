#pragma once

// sadly we cannot inherit from vec3f because of the reference overhead 
// while r,g,b are referencing x,y,z
struct Color {
	float r, g, b;
	Color(float r = 0, float g = 0, float b = 0): r(r), g(g), b(b) {}
	Color(const Color &cpy): r(cpy.r), g(cpy.g), b(cpy.b) {}
	static Color fromByteRGB(unsigned char r, unsigned char g, unsigned char b){
		return Color(r/255.f, g/255.f, b/255.f);
	}
	double luminosity(){
		return sqrt(r*r+g*g+b*b)/1.7320508075688772935274463415059;
	}
	// operators: 
	// Color = Color&
	inline Color &operator = (const Color rhs){ 
		r = rhs.r; g = rhs.g; b = rhs.b;
		return *this;
	}
	// Color *= float
	inline Color &operator *= (float a){ 
		r *= a; g *= a; b *= a;
		return *this;
	}
	// Color *= Color
	inline Color &operator*=(const Color c){
		r *= c.r; g *= c.g; b *= c.b;
		return *this;
	}
	// Color * Color
	inline Color operator*(const Color c) const {
		return Color(r*c.r, g*c.g, b*c.b);
	}
	// Color * float
	inline Color operator*(float a) const {
		return Color(r*a, g*a, b*a);
	}
	// float * Color
	friend inline Color operator*(float a, const Color c){
		return Color(c.r*a, c.g*a, c.b*a);
	}
	// Color += float
	inline Color &operator += (float a){
		r += a; g += a; b += a;
		return *this;
	}
	// Color + float
	inline Color operator + (float a) const {
		return Color(r + a, g + a, b + a);
	}
	// Color += Color
	inline Color &operator += (const Color rhs){
		r += rhs.r; g += rhs.g; b += rhs.b;
		return *this;
	}
	// Color + Color
	inline Color operator + (const Color c) const {
		return Color(r + c.r, g + c.g, b + c.b);
	}
	// Color -= float
	inline Color &operator -= (float a){
		r -= a; g -= a; b -= a;
		return *this;
	}
	// Color - float
	inline Color operator - (float a) const {
		return Color(r - a, g - a, b - a);
	}
};