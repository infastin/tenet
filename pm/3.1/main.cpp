#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

struct Point {
	int x;
	int y;

	double cross(const Point &p) const
	{
		return x * p.y + y * p.x;
	}

	double dist2(const Point &p) const
	{
		return std::pow(x - p.x, 2) + std::pow(y - p.y, 2);
	}
};

std::vector<Point>::iterator find_min(std::vector<Point> &points)
{
	auto min = points.begin();

	for (auto iter = min + 1; iter != points.end(); ++iter) {
		if (min->y < iter->y) {
			continue;
		}

		if (min->y > iter->y || min->x > iter->x) {
			min = iter;
		}
	}

	return min;
}

double cross_three_points(const Point &p1, const Point &p2, const Point &p3)
{
	Point v1 = {
		.x = p2.x - p1.x,
		.y = p2.y - p1.y,
	};

	Point v2 = {
		.x = p3.x - p2.x,
		.y = p3.y - p2.y,
	};

	return v1.cross(v2);
}

std::vector<Point> graham(const std::vector<Point> &cpoints)
{
	std::vector<Point> points(cpoints);

	auto min_iter = find_min(points);
	Point min = *min_iter;

	std::iter_swap(points.begin(), min_iter);

	std::sort(points.begin(), points.end(), [&min](const Point &p1, const Point &p2) -> bool {
		double p1s = min.cross(p1);
		double p2s = min.cross(p2);

		if (p1s < p2s) {
			return true;
		} else if (p1s == p2s) {
			return min.dist2(p1) < min.dist2(p2);
		} else {
			return false;
		}
	});

	std::vector<Point> stack;

	stack.push_back(points[0]);
	stack.push_back(points[1]);

	for (size_t i = 2; i < points.size(); ++i) {
		while (stack.size() > 1 && cross_three_points(stack[stack.size() - 1], stack[stack.size() - 2], points[i]) > 0) {
			stack.pop_back();
		}

		stack.push_back(points[i]);
	}

	return stack;
}

int main()
{
	std::vector<Point> points;

	points.push_back({ .x = 1, .y = 1 });
	points.push_back({ .x = -2, .y = 0 });
	points.push_back({ .x = 2, .y = 1 });
	points.push_back({ .x = 1, .y = 2 });
	points.push_back({ .x = -2, .y = 1 });
	points.push_back({ .x = 2, .y = 3 });

	std::vector<Point> convex = graham(points);

	for (auto &p : convex) {
		printf("%d %d\n", p.x, p.y);
	}

	printf("%lf\n", cross_three_points({ .x = 2, .y = 0 }, { .x = 0, .y = 0 }, { .x = 2, .y = 2 }));

	return 0;
}
