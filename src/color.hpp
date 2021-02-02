#pragma once

#include "other.hpp"

#include <iostream>

void write_color(std::ostream& stream, color px_clr, int samples_per_px) {
	double r = px_clr.x();
	double g = px_clr.y();
	double b = px_clr.z();

	// divide the color by the number of samples  and gamma-correct for gamma=2.0
	double scale = 1.0 / samples_per_px;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// write the translated [0,255] value of each color component
	stream << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		   << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		   << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}