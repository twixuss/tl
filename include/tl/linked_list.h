#pragma once

#include "common.h"
#if TL_DEBUG
#include "list.h" // debug_check
#endif

namespace tl {

template <class T, class Allocator = Allocator>
struct LinkedList {
	using Element = T;
	using Iterator = T *;

	struct Node {
		T value;
		Node *prev = 0;
		Node *next = 0;
	};
	
	Allocator allocator = Allocator::current();
	Node *head = 0;
	Node *tail = 0;

	umm count() const {
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
	
	// Use `unlink` if freeing is unwanted.
	void erase(Node *node) {
		unlink(node);
		allocator.free(node);
	}

	void erase(T *value) {
		Node *node = (Node *)((u8 *)value - offsetof(Node, value));
		erase(node);
	}

	void erase_at(umm index) {
		Node *node = head;
		while (index) {
			--index;
			node = node->next;
			bounds_check(assert(node));
		}

		erase(node);
	}

	Node *unlink(Node *node) {
		auto old_head = head;
		auto old_tail = tail;

		debug_check();
		defer { debug_check(); };
		
		if (node->prev) {
			if (node->next) {
				node->prev->next = node->next;
				node->next->prev = node->prev;
			} else {
				node->prev->next = 0;
				tail = node->prev;
			}
		} else {
			if (node->next) {
				node->next->prev = 0;
				head = node->next;
			} else {
				head = 0;
				tail = 0;
			}
		}

		node->prev = 0;
		node->next = 0;

		return node;
	}

	// node must be not linked
	T &add(Node *node) {
		debug_check();
		defer { debug_check(); };

		assert_equal(node->prev, 0, "node is referenced, unlink first");
		assert_equal(node->next, 0, "node is referenced, unlink first");

		if (head == 0) {
			head = node;
			tail = node;
		} else {
			tail->next = node;
			node->prev = tail;
			tail = node;
		}
		return node->value;
	}

	T &add(T value TL_LP) {
		auto &result = add(allocator.template allocate<Node>(TL_LAC));
		memcpy(&result, &value, sizeof(value));
		return result;
	}
	T &add(TL_LPC) {
		auto &result = add(allocator.template allocate<Node>(TL_LAC));
		new (&result) T();
		return result;
	}

	void clear() {
		debug_check();
		defer { debug_check(); };

		auto node = head;
		while (node) {
			auto next = node->next;
			allocator.free(node);
			node = next;
		}
		head = 0;
		tail = 0;
	}
	
	Optional<T> pop_first() {
		debug_check();
		defer { debug_check(); };

		if (!head)
			return {};

		auto result = head->value;

		allocator.free(head);

		head = head->next;

		if (head) {
			head->prev = 0;
		} else {
			tail = 0;
		}

		return result;
	}

	Optional<T> pop_last() {
		debug_check();
		defer { debug_check(); };

		if (!tail)
			return {};

		auto result = tail->value;

		allocator.free(tail);

		tail = tail->prev;

		if (tail) {
			tail->next = 0;
		} else {
			head = 0;
		}

		return result;
	}

	Optional<T> pop() { return pop_last(); }
	
	void free() {
		debug_check();
		defer { debug_check(); };

		auto node = head;
		while (node) {
			auto next = node->next;
			allocator.free(node);
			node = next;
		}
		head = 0;
		tail = 0;
	}
	
	template <ForEachFlags flags = 0>
	bool for_each(std::invocable<Iterator> auto &&in_fn) {
		debug_check();
		defer { debug_check(); };

		auto fn = wrap_foreach_fn<Iterator>(in_fn);
		auto node = head;
		while (node) {
			auto d = fn(&node->value);
			switch (d & ForEach_erase_mask) {
				case ForEach_erase:
				case ForEach_erase_unordered:
					erase(node);
					break;
			}
			if (d & ForEach_break)
				return true;

			node = node->next;
		}
		return false;
	}
	
	auto &front(this auto &&self) { bounds_check(assert(self.head)); return self.head->value; }
	auto &back(this auto &&self) { bounds_check(assert(self.head)); return self.tail->value; }

	
	struct CppIterator {
		Node *node;

		CppIterator() : node() {}
		CppIterator(Node *node) : node(node) {}
		T &operator*() { return node->value; }
		CppIterator &operator++() {
			node = node->next;
			return *this;
		}
		CppIterator operator++(int) {
			Node *prev = node;
			node = node->next;
			return prev;
		}
		bool operator==(CppIterator const &that) const { return node == that.node; }
		bool operator!=(CppIterator const &that) const { return node != that.node; }
		T *operator->() { return std::addressof(node->value); }
		explicit operator bool() { return node; }
	};
	
	CppIterator begin() { return head; }
	CppIterator end() { return {}; }


	template <bool is_const>
	struct Iter {
		LinkedList *list = 0;
		Node *node = 0;
		bool should_advance = true;
		bool reverse = false;

		explicit operator bool() {
			return node;
		}
		void next() {
			if (should_advance) {
				node = reverse ? node->prev : node->next;
			}
		}
		auto &operator*() {
			return node->value;
		}
		auto pointer() { return &node->value; }

		void erase() requires(!is_const) {
			auto next_node = reverse ? node->prev : node->next;
			list->erase(node);
			node = next_node;
			should_advance = false;
		}
	};

	struct IterOptions {
		bool reverse = false;
	};

	auto iter(this auto &&self, IterOptions options = {}) {
		Iter<tl_self_const> result = {(LinkedList *)&self};
		result.reverse = options.reverse;
		result.node = options.reverse ? self.tail : self.head;
		return result;
	}

	void debug_check() {
		#if 0
		if (head) {
			assert(tail);
		} else {
			assert(!tail);
		}

		List<Node *, DefaultAllocator> nodes;
		Node *node = head;
		Node *prev = 0;
		while (node) {
			assert(node->prev == prev);
			nodes.add(node);
			prev = node;
			node = node->next;
		}

		node = tail;
		Node *next = 0;
		umm i = nodes.count - 1;
		while (node) {
			assert(node->next == next);
			assert(node == nodes[i--]);
			next = node;
			node = node->prev;
		}
		#endif
	}
};

}

#ifdef TL_ENABLE_TESTS

TL_TEST {
	using namespace tl;

	LinkedList<int> list;
	list.add(1);
	list.add(4);
	list.add(9);
	list.add(16);
	assert(list.count() == 4);
	assert(find(list, 1));
	assert(find(list, 4));
	assert(find(list, 9));
	assert(find(list, 16));

	list.erase(find(list, 4));
	assert(list.count() == 3);
	assert(find(list, 1));
	assert(!find(list, 4));
	assert(find(list, 9));
	assert(find(list, 16));
};

#endif