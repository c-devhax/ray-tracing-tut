#pragma once

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::make_shared;

class hittable_list : public hittable {
public:
	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> obj) {
		add(obj);
	}

	void clear() {
		objects.clear();
	}

	void add(std::shared_ptr<hittable> object) {
		objects.push_back(object);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;

		for (const auto& object: objects) {
			if (object->hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}
private:
	std::vector<std::shared_ptr<hittable>> objects;
};