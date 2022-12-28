#ifndef STACK_HPP_UBS8ZP6W
#define STACK_HPP_UBS8ZP6W

#include <cstddef>
#include <stdexcept>

namespace stack {
using std::size_t;

template<typename T>
class array {
private:
	T *data;
	size_t cap;
	size_t len;

	void resize(size_t newcap)
	{
		T *new_data = new T[newcap];

		for (size_t i = 0; i < len; ++i) {
			new_data[i] = std::move(data[i]);
		}

		delete[] data;

		data = new_data;
		cap = newcap;
	}

public:
	array()
	{
		cap = 8;
		data = new T[cap];
		len = 0;
	}

	array(const array &arr)
	{
		cap = arr.cap;
		len = arr.len;
		data = new T[cap];

		for (size_t i = 0; i < len; ++i) {
			data[i] = arr.data[i];
		}
	}

	~array()
	{
		delete[] data;
	}

	void push(const T &value)
	{
		if (len >= cap) {
			resize(cap * 2);
		}

		data[len++] = value;
	}

	void push(T &&value)
	{
		if (len >= cap) {
			resize(cap * 2);
		}

		data[len++] = value;
	}

	void pop()
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		len--;
	}

	T &top()
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		return data[len - 1];
	}

	const T &top() const
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		return data[len - 1];
	}

	size_t size() const
	{
		return len;
	}

	bool empty() const
	{
		return len == 0;
	}
};
}  // namespace stack
#endif /* end of include guard: STACK_HPP_UBS8ZP6W */
