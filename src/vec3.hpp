#pragma once

#include "other.hpp"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
	vec3() : e{0, 0, 0} {}
	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](size_t i) const { return e[i]; }
	double& operator[](size_t i) { return e[i]; }

	vec3& operator+=(const vec3& v) {
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1/t;
	}

	double length() const {
		return sqrt(this->length_squared());
	}

	double length_squared() const {
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	}

	bool near_zero() const {
		// return true if the vector is close to 0 in all directions
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
private:
	double e[3];
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// vec3 utility functions
inline std::ostream& operator<<(std::ostream& stream, const vec3& vec) {
	return stream << vec[0] << ' ' << vec[1] << ' ' << vec[2];
}

inline vec3 operator+(const vec3& a, const vec3& b) {
	return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

inline vec3 operator-(const vec3& a, const vec3& b) {
	return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline vec3 operator*(const vec3& a, const vec3& b) {
	return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline vec3 operator*(const double t, const vec3& vec) {
	return vec3(vec[0] * t, vec[1] * t, vec[2] * t);
}

inline vec3 operator*(const vec3& vec, const double t) {
	return t * vec;
}

inline vec3 operator/(const vec3& vec, const double t) {
	return (1/t) * vec;
}

inline double dot(const vec3& a, const vec3& b) {
	return a[0] * b[0] 
		+ a[1] * b[1]
		+ a[2] * b[2];

}

inline vec3 cross(const vec3& a, const vec3& b) {
	return vec3(a[1] * b[2] - a[2] * b[1],
				a[2] * b[0] - a[0] * b[2],
				a[0] * b[1] - a[1] * b[0]);
}

inline vec3 unit_vector(const vec3& vec) {
	return vec / vec.length();
}

// random vectors

inline vec3 random_in_unit_sphere() {
	while (true) {
		vec3 p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // in the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_unit_disk() {
	while (true) {
		vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

// reflection and refractioin

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etap_over_eta) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etap_over_eta * (uv + cos_theta * n);
	vec3 r_out_par = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_par;
}