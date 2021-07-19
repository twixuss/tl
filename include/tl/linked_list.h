#pragma once

#include "common.h"

namespace tl {

template <class T>
struct LinkedList {
	using ValueType = T;

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
		bool operator!() { return !node; }
	};

	LinkedList() = default;
	/*
	void clear() {
		Node *node = head;
		for (;;) {
			if (node == 0)
				break;
			defer { DEALLOCATE(Allocator, node); };
			node = node->next;
		}
		head = tail = 0;
	}
	*/
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
			bounds_check(node);
		}
		return node->value;
	}

	T &add(T value = {}) {
		if (head == 0) {
			head = tail = allocator.allocate<Node>();
		} else {
			if (!tail->next) {
				tail->next = allocator.allocate<Node>();
			}
			tail = tail->next;
		}

		memcpy(&tail->value, &value, sizeof(value));

		return tail->value;
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

	T &front() { bounds_check(head); return head->value; }
	T const &front() const { bounds_check(head); return head->value; }

	T &back() { bounds_check(head); return tail->value; }
	T const &back() const { bounds_check(head); return tail->value; }

	Iterator begin() { return head; }
	Iterator end() { return {}; }

	Allocator allocator = current_allocator;
	Node *head = 0;
	Node *tail = 0;
};

template <class T>
void free(LinkedList<T> &list) {
	auto node = list.head;
	while (node) {
		auto next = node->next;
		list.allocator.free(node);
		node = next;
	}
	list.head = 0;
	list.tail = 0;
}

template <class T>
void erase(LinkedList<T> &list, T *value) {
	using Node = typename LinkedList<T>::Node;
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
	bounds_check(false);
}

template <class T, class Predicate>
T *find_if(LinkedList<T> list, Predicate &&predicate) {
	auto node = list.head;
	while (node) {
		if (predicate(node->value))
			return &node->value;
		node = node->next;
	}
	return 0;
}

}
