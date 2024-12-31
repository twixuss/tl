#pragma once

#include "common.h"

namespace tl {

template <class T, class Allocator = Allocator>
struct LinkedList {
	using Element = T;

	struct Node {
		T value;
		Node *next;
	};
	struct Iterator {
		Node *node;

		Iterator() : node() {}
		Iterator(Node *node) : node(node) {}
		T &operator*() { return node->value; }
		Iterator &operator++() {
			node = node->next;
			return *this;
		}
		Iterator operator++(int) {
			Node *prev = node;
			node = node->next;
			return prev;
		}
		bool operator==(Iterator const &that) const { return node == that.node; }
		bool operator!=(Iterator const &that) const { return node != that.node; }
		T *operator->() { return std::addressof(node->value); }
		explicit operator bool() { return node; }
	};

	LinkedList() = default;

	umm size() const {
		umm result = 0;
		Node *node = head;
		while (node) {
			++result;
			node = node->next;
		}
		return result;
	}

	T &operator[](umm index) {
		Node *node = head;
		while (index) {
			--index;
			node = node->next;
			bounds_check(assert(node));
		}
		return node->value;
	}

	void erase_at(umm index) {
		Node *prev = 0;
		Node *node = head;
		while (index) {
			--index;
			prev = node;
			node = node->next;
			bounds_check(assert(node));
		}

		if (prev) {
			prev->next = node->next;
			if (!node->next)
				tail = prev;
		} else {
			head = node->next;
		}

		allocator.free(node);
	}

	T &add_steal(Node *node) {
		assert_equal(node->next, 0);
		if (head == 0) {
			head = tail = node;
		} else {
			assert(!tail->next);
			tail = tail->next = node;
		}
		return node->value;
	}

	T &add(T value TL_LP) {
		auto &result = add_steal(allocator.template allocate<Node>(TL_LAC));
		memcpy(&result, &value, sizeof(value));
		return result;
	}
	T &add(TL_LPC) {
		auto &result = add_steal(allocator.template allocate<Node>(TL_LAC));
		new (&result) T();
		return result;
	}

	void clear() {
		auto node = head;
		while (node) {
			auto next = node->next;
			allocator.free(node);
			node = next;
		}
		head = 0;
		tail = 0;
	}

	Optional<T> pop() {
		if (!tail)
			return {};

		Node *prev = 0;
		auto node = head;
		while (node != tail) {
			prev = node;
			node = node->next;
		}

		auto result = tail->value;

		allocator.free(tail);

		if (prev) {
			tail = prev;
			tail->next = 0;
		} else {
			head = tail = 0;
		}

		return result;
	}

	umm count() {
		umm result = 0;
		auto node = head;
		while (node) {
			result += 1;
			node = node->next;
		}
		return result;
	}

	T &front() { bounds_check(assert(head); return head->value; )}
	T const &front() const { bounds_check(assert(head); return head->value; )}

	T &back() { bounds_check(assert(head); return tail->value; )}
	T const &back() const { bounds_check(assert(head); return tail->value; )}

	Iterator begin() { return head; }
	Iterator end() { return {}; }

	Allocator allocator = Allocator::current();
	Node *head = 0;
	Node *tail = 0;
};

template <class T, class Allocator>
void free(LinkedList<T, Allocator> &list) {
	auto node = list.head;
	while (node) {
		auto next = node->next;
		list.allocator.free(node);
		node = next;
	}
	list.head = 0;
	list.tail = 0;
}

template <class T, class Allocator>
void erase(LinkedList<T, Allocator> &list, T *value) {
	using Node = typename LinkedList<T, Allocator>::Node;
	Node *node = list.head;
	Node *prev_node = 0;
	while (node) {
		if (value == &node->value) {
			if (prev_node) {
				prev_node->next = node->next;
			}
			if (node == list.head) list.head = list.head->next;
			if (node == list.tail) list.tail = prev_node;
			list.allocator.free(node);
			return;
		}
		prev_node = node;
		node = node->next;
	}
	bounds_check(assert(false));
}

template <class T, class Allocator, class Predicate>
T *find_if(LinkedList<T, Allocator> list, Predicate &&predicate) {
	auto node = list.head;
	while (node) {
		if (predicate(node->value))
			return &node->value;
		node = node->next;
	}
	return 0;
}

template <ForEachFlags flags, class T, class Allocator, class Fn>
bool for_each(LinkedList<T, Allocator> list, Fn &&fn) {
	auto node = list.head;
	while (node) {
		if constexpr (std::is_same_v<decltype(fn(*(T*)0)), ForEachDirective>) {
			auto d = fn(node->value);
			if (d & ForEach_erase) not_implemented();
			if (d & ForEach_erase_unordered) not_implemented();
			if (d & ForEach_break)
				return true;
		} else {
			fn(node->value);
		}
		node = node->next;
	}
	return false;
}

template <class T, class Allocator>
umm count_of(LinkedList<T, Allocator> list) {
	return list.count();
}

}
