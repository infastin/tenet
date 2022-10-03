#ifndef GRAPH_HPP_T4ANCUOH
#define GRAPH_HPP_T4ANCUOH

#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

namespace task2 {

template<typename T>
class graph {
public:
	class node {
	protected:
		T value;
		std::vector<node *> links;

	public:
		node(const T &value)
		{
			this->value = value;
		}

		auto begin() { return links.begin(); }

		auto cbegin() const { return links.cbegin(); }

		auto end() { return links.end(); }

		auto cend() const { return links.cend(); }

		size_t degree() { return links.size(); }

		T &data() { return value; }

		node *find(const T &value)
		{
			for (node *link : links) {
				if (link->value == value)
					return link;
			}

			return nullptr;
		}

		node *find(node *_node)
		{
			for (node *link : links) {
				if (link == _node)
					return link;
			}

			return nullptr;
		}

		void insert_dir(node *n)
		{
			if (find(n) != nullptr)
				return;

			links.push_back(n);
		}

		void insert_nodir(node *n)
		{
			insert_dir(n);
			n->insert_dir(this);
		}
	};

private:
	std::vector<node *> nodes;

public:
	~graph()
	{
		for (node *n : nodes) {
			delete n;
		}
	}

	graph(std::initializer_list<T> vertices = {})
	{
		for (T v : vertices) {
			insert(v);
		}
	}

	graph(std::initializer_list<std::pair<T, T>> edges = {})
	{
		for (std::pair<T, T> e : edges) {
			insert_dir(e.first, e.second);
		}
	}

	auto begin() { return nodes.begin(); }

	auto cbegin() const { return nodes.cbegin(); }

	auto end() { return nodes.end(); }

	auto cend() const { return nodes.cend(); }

	size_t size() { return nodes.size(); }

	node *find(const T &value)
	{
		for (node *n : nodes) {
			if (n->data() == value)
				return n;
		}

		return nullptr;
	}

	node *insert(const T &value)
	{
		node *f = find(value);
		if (f != nullptr)
			return f;

		node *n = new node(value);
		nodes.push_back(n);

		return n;
	}

	std::pair<node *, node *> insert_dir(const T &v1, const T &v2)
	{
		node *n1 = insert(v1);
		node *n2 = insert(v2);

		n1->insert_dir(n2);

		return { n1, n2 };
	}

	std::pair<node *, node *> insert_nodir(const T &v1, const T &v2)
	{
		node *n1 = insert(v1);
		node *n2 = insert(v2);

		n1->insert_dir(n2);
		n2->insert_dir(n1);

		return { n1, n2 };
	}
};
}  // namespace task2
#endif /* end of include guard: GRAPH_HPP_T4ANCUOH */
