#include "other.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	if (depth <= 0)
		return color(0.0, 0.0, 0.0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);

		return color(0.0, 0.0, 0.0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0)+ t * color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv) {
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>((double)image_width / aspect_ratio);
	const int samples_per_px = 100;
	const int max_depth = 80;

	// World
	hittable_list world;

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera
	camera cam;

	// Render	
	const std::string out_file = argc > 1 ? argv[1] : "output/image.ppm";
	std::ofstream file(out_file);

	file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			color px_clr(0.0, 0.0, 0.0);
			for (int s = 0; s < samples_per_px; s++) {
				double u = (i + random_double()) / (image_width - 1);
				double v = (j + random_double()) / (image_height - 1);

				ray r = cam.get_ray(u, v);
				px_clr += ray_color(r, world, max_depth);
			}
			write_color(file, px_clr, samples_per_px);
		}
	}

	std::cout << "\nDone.\n";
	file.close();
}
