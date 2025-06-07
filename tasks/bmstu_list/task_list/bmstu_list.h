#include <cstddef>
#include <iterator>
#include <ostream>
#include "abstract_iterator.h"

namespace bmstu
{
template <typename T>
class list
{private:
	struct node
	{
		T data;
		node* next;
		node* prev;
		node() : next(nullptr), prev(nullptr) {}
		node(node* p, const T& d, node* n) : data(d), prev(p), next(n) {}
	};
	node* head_;
	node* tail_;
	size_t size_;

public:
	class iterator	: public abstract_iterator<iterator, T, std::bidirectional_iterator_tag>{
	private: 
		node* current_node;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::bidirectional_iterator_tag;

		iterator(node* node) : current_node(node) {}

		reference operator*() const override {
			return current_node->data;
		}
		pointer operator->() const override{
			return &(current_node->data);
		}

		iterator operator++(int) override {
			iterator temp = *this;
			current_node = current_node->next;
			return temp;

		}

		iterator& operator++ () override{
			current_node = current_node->next;
			return *this;
		}

		iterator operator--(int) override {
			iterator temp = *this;
			current_node = current_node->prev;
			return temp;
		}

		iterator& operator--() override {
			current_node = current_node->prev;
			return *this;
		}

		iterator& operator+=(const difference_type& n) override {
			if (n>0) {
				for (difference_type i = 0; i <n; ++i) {
					current_node = current_node->next;
				}
			} else if (n < 0) {
				for (difference_type i = 0; i < -n; ++i) {
					current_node = current_node ->prev;
				}
			}
			return *this;
		}

		iterator& operator-=(const difference_type& n) override{
			return *this += (-n);
		}

		iterator operator+(const difference_type& n) const override{
			iterator temp = *this;
			temp += n;
			return temp;
		}

		iterator operator-(const difference_type& n) const override {
			iterator temp = *this;
			temp -= n;
			return temp;
		}

		difference_type operator-(const iterator& other) const override {
			difference_type count = 0;
			node* temp = other.current_node;
			while (temp != current_node && temp != nullptr) {
				temp = temp->next;
				++count;
			}
			if (temp ==current_node) {
				return count;
			}
			return -count;
		}

		bool operator==(const iterator& other) const override {
			return current_node == other.current_node;
		}

		bool operator!=(const iterator& other) const override {
			return !(*this == other);
		}

		explicit operator bool() const override {
			return current_node != nullptr;
		}
	};

	using const_iterator = iterator;

	list() {
		head_ = new node();
		tail_ = new node();
		head_->next = tail_;
		tail_->prev = head_;
		size_ = 0;
	}

	template <typename it>
	list(it begin, it end)
	{	head_ = new node();
		tail_ = new node();
		head_->next = tail_;
		tail_->prev = head_;
		size_ = 0;
		for (auto iter = begin; iter != end; ++iter) {
			push_back(*iter);
		}
	}

	list(std::initializer_list<T> values) {
		head_ = new node();
		tail_ = new node();
		head_->next = tail_;
		tail_->prev = head_;
		size_ = 0;
		for (const auto& val: values) {
			oush_back(val);
		}
	}

	list(const list& other) {
		head_ = new node();
		tail_ = new node();
		head_->next = tail_;
		tail_->prev = head_;
		size_= 0;
		for (const auto& val : other) {
			push_back(val);
		}
	}

	list(list&& other) : head_(other.head_), tail_(other.tail_), size_(other.size_) {
		other.head_ = new node();
		other.tail_ = new node();
		other.head_->next = other.tail_;
		other.tail_->prev = other.head_;
		other.size_ = 0;
	}

	~list() {
		struct node* current = head_;
		while (current != nullptr) {
			node* next = current->next;
			delete current;
			current = next;
		}
	}

	list& operator=(const list& other) {
		if (this !=&other) {
			clear();
			for (const auto& val : other) {
				push_back(val);
			}
		}
		return *this;
	}
	
	list& operator=(list&& other) noexcept {
		if (this != other) {
			node* current = head_;
			while (current != nullptr) { 
				node* next = current->next;
				delete current;
				current = next;
		}
		head_ = other.head_;
		tail_ = other.tail_;
		size_ = other.size_;
		other.head_ = new node();
		other.tail_ = new node();
		other.head_->next = other.tail_;
		other.tail_->prev = other.head_;
		other.size_ = 0;
		}
		return *this;
	}

	void clear() {
		node* current = head_->next;
		while (current != tail_)
		{
			node* next = current->next;
			delete current;
			current = next;
		}
		head_->next = tail_;
		tail_->prev = head_;
		size_ = 0;
		
	}

	size_t size() const {
		return size_;
	}
	bool empty() const noexcept {
		return size_ == 0;
	}

	void swap(list& other) noexcept {
		std::swap(head_, other.head_);
		std::swap(tail_, other.tail_);
		std::swap(size_, other.size_);
	}

	friend void swap(list& l, list& r) {
		l.swap(r);
	}

	iterator begin() noexcept {
		return iterator(head_->next);
	}

	iterator end() noexcept {
		return iterator(tail_);
	}

	const_iterator begin () const noexcept {
		return const_iterator(head_->next);
	}

	const_iterator end () const noexcept {
		return const_iterator(tail_);
	}

	const_iterator cbegin() const noexcept {
		return const_iterator(head_->next);
	}

	const_iterator cend() const noexcept {
		return const_iterator(tail_);
	}
	void push_back (const T& value) {
		node* last = tail_->prev;
		node* new_last = new node(last, value, tail_);
		last->next = new_last;
		tail_->prev = new_last;
		++size_;
	}

	void push_front(const T& value) {
		node* first = head_->next;
		node* new_first = new node(head_, value, first);
		head_->next = new_first;
		first->prev = new_first;
		++size_;
	}

	iterator insert(const_iterator pos, const T& value) {
		node* current = pos.current_node;
		node* prev = current->prev;
		node* new_node = new node(prev, value, current);
		prev->next = new_node;
		++size_;
		return iterator (new_node);
	}

	const T& operator[](size_t pos) {
		node* current = head_->next;
		for (size_t i = 0; i < pos; ++i) {
			current = current->next;
		}
		return current->data;;
	}

	T& operator[](size_t pos) {
		node* current = head_->nextt;
		for (size_t i = 0; i < pos; ++i) {
			current = current->next;
		}
		returrn current->data;
	}

	friend bool operator==(const list& l, const list& r) {
		if (l.size_ != r.size_) return false;
		auto it1 = l.begin();
		auto it2 - r.begin();
		while (it1 !=l.end()) {
			if (*it1 != *it2) return false;
			++it1;
			++it2;
		}
		return true
	}
	friend std::ostream& operator<<(std::ostream& os, const list& other) {
		for (const auto& cal : other) {
			os << val << "";
		}
		return os;
	}
	friend bool operator!=(const list& l,  const list& r) {
		return!(l==r);
	}

	friend auto operator<=>(const list& l, const list& r) {
		auto it1 = l.begin();
		auto it2 = r.begin();
		while (it1 !=l.end() && it2 !=r.end()){
			if (*it1 < *it2) return std::strong_ordering::less;
			if (*it2 < *it1) return std::strong_ordering::greater;
			++it1;
			++it2;
		}
		if (it1 ==l.end() && it2 !=r.end()) return std::strong_ordering::less;
		if (it1 !=l.end() && it2 ==r.end()) return std::strong_ordering::greater
		return std::strong_ordering::equal;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other) {
		for (const auto& val : other) {
			os << val << " ";
		}
		return os;
	}
private:
static bool lexicographical_compare_(const list<T>& l, const list<T>& r) {
	auto it1 =l.begin();
	auto it2 =r.begin();
	while (it1 != l.end() && it2 !=r.end()) {
		if (*it1 < *it2) return true;
		if (*it2 < *it1) return false;
		++it1;
		++it2;
	}
	return (it1 == l.end() && it2 != r.end)
}

	
#pragma endregion
#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_last = new node(tail_->prev_node_, value, tail_);
		tail_->prev_node_ = new_last;
		last->next_node_ = new_last;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		// адрес реального последнего элемента
		node* first = head_->next_node_;
		node* new_first = new node(head_, value, first);
		head_->next_node_ = new_first;
		first->prev_node_ = new_first;
		++size_;
	}

#pragma endregion

	bool empty() const

		noexcept
	{
		return (size_ == 0u);
	}

	~list() {}

	void clear() {}

	size_t size() const { return 0; }

	void swap(list& other)

		noexcept
	{
	}

	friend void swap(list& l, list& r) { l.swap(r); }

#pragma region iterators

	iterator begin()

		noexcept
	{
		return iterator{head_->next_node_};
	}

	iterator end()

		noexcept
	{
		return iterator{tail_};
	}

	const_iterator begin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator end() const

		noexcept
	{
		return const_iterator{tail_};
	}

	const_iterator cbegin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator cend() const

		noexcept
	{
		return const_iterator{tail_};
	}

#pragma endregion

	T operator[](size_t pos) const {}


	friend bool operator==(const list& l, const list& r) { return true; }

	friend bool operator!=(const list& l, const list& r) { return false; }

	friend auto operator<=>(const list& lhs, const list& rhs) { return true; }

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		return iterator{nullptr};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		return "123";
	}

	size_t size_ = 0;
};
}  // namespace bmstu