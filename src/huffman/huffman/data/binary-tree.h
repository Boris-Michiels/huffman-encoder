#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <memory>
#include <functional>


namespace data {
	template<typename T>
	class Node {
	public:
		virtual ~Node() {}
	};

	template<typename T>
	class Branch : public Node<T> {
		const std::unique_ptr<const Node<T>>  _left_child, _right_child;

	public:
		Branch(std::unique_ptr<const Node<T>> left_child, std::unique_ptr<const Node<T>> right_child)
			: _left_child(std::move(left_child)), _right_child(std::move(right_child)) {}

		const Node<T>& left_child() const { return *_left_child; }
		const Node<T>& right_child() const { return *_right_child; }
	};

	template<typename T>
	class Leaf : public Node<T> {
		T _value;

	public:
		Leaf(const T& value) : _value(value) {}

		const T& value() const {
			return _value;
		}
	};

	template<typename IN, typename OUT>
	std::unique_ptr<Node<OUT>> map(const Node<IN>& tree, std::function<OUT(const IN&)> function) {
		const auto& node = dynamic_cast<const Leaf<IN>&>(tree);

		if (node) return std::make_unique<Leaf<OUT>>(function(node.value()));
		else {
			node = dynamic_cast<Branch<IN>&>(tree);
			std::unique_ptr<Node<OUT>> new_left_child = map(node.left_child(), function);
			std::unique_ptr<Node<OUT>> new_right_child = map(node.right_child(), function);
			return std::make_unique<Branch<OUT>>(std::move(new_left_child), std::move(new_right_child));
		}
	}
}

#endif