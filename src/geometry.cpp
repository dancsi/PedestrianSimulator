#include "geometry.h"

bool project(vec_t point, line_t line, vec_t& proj) // vraca da li se projekcija zaista nalazi na duzi
{
	vec_t line_dir = line.q - line.p; line_dir.normalize();
	proj = line.p + line_dir.dot(point - line.p) * line_dir;
	std::pair<float, float> x = std::minmax(line.p.x, line.q.x), y = std::minmax(line.p.y, line.q.y);
	return x.first <= proj.x && proj.x <= x.second && y.first <= proj.y && proj.y <= y.second;
}