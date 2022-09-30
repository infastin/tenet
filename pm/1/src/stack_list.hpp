#ifndef STACK_LIST_HPP_HUD2HABX
#define STACK_LIST_HPP_HUD2HABX

#include <cstddef>
#include <stdexcept>

namespace task1 {
using std::size_t;

template<typename T>
class stack_list {
private:
	struct node {
		T value;
		node *next;
	};

	node *head;
	size_t len;

public:
	stack_list()
	{
		len = 0;
		head = nullptr;
	}

	stack_list(const stack_list &lst)
	{
		head = nullptr;
		len = lst.len;

		node **p = &head;
		const node *iter = lst.head;

		while (iter != nullptr) {
			node *copy = new node(iter->value, nullptr);
			*p = copy;
			p = &copy->next;
			iter = iter->next;
		}
	}

	~stack_list()
	{
		node *iter = head;
		while (iter != nullptr) {
			node *next = iter->next;
			delete iter;
			iter = next;
		}
	}

	void push(const T &value)
	{
		node *_node = new node(value, head);
		head = _node;
		len++;
	}

	void push(T &&value)
	{
		node *_node = new node(value, head);
		head = _node;
		len++;
	}

	void pop()
	{
		if (len == 0)
			throw std::logic_error("empty stack");

		node *new_head = head->next;
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
}  // namespace task1
#endif /* end of include guard: list_HPP_HUD2HABX */
