#ifndef STACK_LIST_HPP_HUD2HABX
#define STACK_LIST_HPP_HUD2HABX

#include <cstddef>
#include <stdexcept>

namespace stack {
using std::size_t;

template<typename T>
class list {
private:
	struct list_node {
		T value;
		list_node *next;
	};

	list_node *head;
	size_t len;

public:
	list()
	{
		len = 0;
		head = nullptr;
	}

	list(const list &lst)
	{
		len = lst.len;

		list_node **p = &head;
		const list_node *iter = lst.head;

		while (iter != nullptr) {
			list_node *copy = new list_node(iter->value, nullptr);
			*p = copy;
			p = &copy->next;
			iter = iter->next;
		}
	}

	~list()
	{
		list_node *iter = head;
		while (iter != nullptr) {
			list_node *next = iter->next;
			delete iter;
			iter = next;
		}
	}

	void push(const T &value)
	{
		list_node *node = new list_node(value, head);
		head = node;
		len++;
	}

	void push(T &&value)
	{
		list_node *node = new list_node(value, head);
		head = node;
		len++;
	}

	void pop()
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		list_node *new_head = head->next;
		delete head;
		head = new_head;
		len--;
	}

	T &top()
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		return head->value;
	}

	const T &top() const
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		return head->value;
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
#endif /* end of include guard: list_HPP_HUD2HABX */
