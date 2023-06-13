#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <memory>
#include <functional>
#include <assert.h>


namespace data {
	template<typename T>
	class Node {
	public:
		virtual ~Node() {}

		virtual bool is_leaf() const { return false; }
	};

	template<typename T>
	class Branch : public Node<T> {
		std::unique_ptr<const Node<T>>  _left_child, _right_child;

	public:
		Branch(std::unique_ptr<const Node<T>> left_child, std::unique_ptr<const Node<T>> right_child)
			: _left_child(std::move(left_child)), _right_child(std::move(right_child)) {
			assert(_left_child != nullptr);
			assert(_right_child != nullptr);
		}

		const Node<T>& left_child() const { return *_left_child; }
		const Node<T>& right_child() const { return *_right_child; }
	};

	template<typename T>
	class Leaf : public Node<T> {
		const T _value;

	public:
		Leaf(const T& value) : _value(value) {}

		const T& value() const { return _value; }
		bool is_leaf() const override { return true; }
	};

	template<typename IN, typename OUT>
	std::unique_ptr<Node<OUT>> map(const Node<IN>& root, std::function<OUT(const IN&)> function) {
		if (root.is_leaf()) {
			const auto& leaf = static_cast<const Leaf<IN>&>(root);
			return std::make_unique<Leaf<OUT>>(function(leaf.value()));
		}
		else {
			const auto& branch = static_cast<const Branch<IN>&>(root);
			std::unique_ptr<Node<OUT>> new_left_child = map(branch.left_child(), function);
			std::unique_ptr<Node<OUT>> new_right_child = map(branch.right_child(), function);
			return std::make_unique<Branch<OUT>>(std::move(new_left_child), std::move(new_right_child));
		}
	}
}

#endif