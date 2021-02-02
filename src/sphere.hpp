#pragma once

#include "hittable.hpp"

class sphere : public hittable {
public:
	sphere() {}
	sphere(const point3& cen, double r) : center(cen), radius(r) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		vec3 oc = r.origin() - center;
		double a = r.direction().length_squared();
		double half_b = dot(oc, r.direction());
		double c = oc.length_squared() - radius * radius;

		double discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		auto sqrt_d = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		double root = (-half_b - sqrt_d) / a;
		if (root < t_min || t_max < root) {
			root = (-half_b + sqrt_d) / a;
			if (root < t_min || t_max < root) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

private:
	point3 center;
	double radius;

};