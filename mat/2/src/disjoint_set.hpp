#ifndef DISJOINT_SET_HPP_IDJWLI0Q
#define DISJOINT_SET_HPP_IDJWLI0Q

#include <cstddef>
#include <unordered_map>
#include <vector>

template<typename T>
class DisjointSet {
  public:
	void make_set(T value)
	{
		size_t index = ancestors.size();
		ancestors.push_back(index);
		lut.insert({ value, index });
	}

  private:
	std::vector<size_t> ancestors;
	std::unordered_map<T, size_t> lut;
};

#endif /* end of include guard: DISJOINT_SET_HPP_IDJWLI0Q */
