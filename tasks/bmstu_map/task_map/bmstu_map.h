/*
 * ЗАДАНИЕ: Реализация std::map на основе AVL-дерева
 *
 * Цель: Создать полноценный аналог std::map с использованием
 * самобалансирующегося AVL-дерева в качестве внутренней структуры данных.
 *
 * Что нужно реализовать:
 *
 * 1. AVL Tree (avl_balanced_tree):
 *    - insert() - вставка пары ключ-значение с балансировкой
 *    - remove() - удаление по ключу с балансировкой
 *    - find() - поиск узла по ключу
 *    - balance() - балансировка дерева (проверка высот и вызов поворотов)
 *    - Повороты: rotateWithLeftChild, rotateWithRightChild,
 *                doubleWithLeftChild, doubleWithRightChild
 *    - Вспомогательные: heightOfTree(), findMinPtr()
 *
 * 2. Iterator (map::iterator):
 *    - Конструктор для инициализации (найти самый левый узел для begin)
 *    - operator*() - разыменование (вернуть std::pair<const K, V>)
 *    - operator++() - переход к следующему элементу в in-order обходе
 *    - Использовать std::stack для обхода дерева
 *
 * 3. Map (map):
 *    - Все публичные методы уже реализованы и используют AVL дерево
 *    - После реализации AVL дерева и итератора, map будет работать полностью
 *
 * Особенности AVL дерева:
 * - Для каждого узла разность высот левого и правого поддеревьев <= 1
 * - После вставки/удаления может потребоваться 1-2 поворота для балансировки
 * - 4 случая дисбаланса: LL, LR, RR, RL (Left-Left, Left-Right, и т.д.)
 *
 * Тестирование:
 * - Запустите тесты: ./tasks/bmstu_map/bmstu_map
 * - Все 18 тестов должны пройти успешно после полной реализации
 */

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <stack>
#include <stdexcept>
#include <utility>
#include "abstract_iterator.h"

namespace bmstu
{
// ==================== AVL Tree Node ====================
template <typename K, typename V>
struct tree_node
{
	tree_node(const K& k, const V& v)
		: key(k), value(v), left(nullptr), right(nullptr), height(1)
	{
	}

	K key;
	V value;
	uint8_t height;
	tree_node* left;
	tree_node* right;
};

// ==================== AVL Balanced Tree ====================
// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
// Реализуйте самобалансирующееся AVL-дерево с поддержкой вставки, удаления и
// поиска по ключам. Дерево должно автоматически балансироваться после каждой
// операции.
template <typename K, typename V>
class avl_balanced_tree
{
   public:
	avl_balanced_tree() : root_(nullptr), size_(0) {}
	~avl_balanced_tree() { clear(root_); }

	void insert(const K& key, const V& value)
	{
		this->insert(key, value, root_);
	}

	void remove(const K& key) { this->remove(key, root_); }

	tree_node<K, V>* find(const K& key) { return this->find(key, root_); }

	const tree_node<K, V>* find(const K& key) const
	{
		return this->find(key, root_);
	}

	bool contains(const K& key) const { return find(key) != nullptr; }

	size_t size() const { return size_; }

	bool empty() const { return size_ == 0; }

	tree_node<K, V>* get_root() { return root_; }

	const tree_node<K, V>* get_root() const { return root_; }

	void print() { print_tree_(root_, 1); }

	void inorder_print()
	{
		inorder_print(root_);
		std::cout << "\n";
	}

   private:
	tree_node<K, V>* insert(const K& key,
							const V& value,
							tree_node<K, V>*& node)
	{  // ==================== Вставка ====================

		if (node == nullptr)
		{
			node = new tree_node<K, V>(key, value);
			++size_;
			return node;
		}

		if (key < node->key)
		{
			node->left = insert(key, value, node->left);
		}

		else if (key > node->key)
		{
			insert(key, value, node->right);
		}

		else
		{
			// Ключ уже существует - обновляем значение
			node->value = value;
			return node;
		}

		balance(node);
		return node;
		// TODO: Реализовать вставку узла в AVL дерево ready
		// 1. Если node == nullptr, создать новый узел 1
		// 2. Рекурсивно вставить в левое или правое поддерево в зависимости от
		// ключа 1
		// 3. Обновить значение, если ключ уже существует 1
		// 4. Вызвать balance(node) для балансировки 1
		return nullptr;
	}

	void remove(const K& key, tree_node<K, V>*& node)
	{
		// ==================== Удаление ====================
		if (node == nullptr)
			return;

		if (key < node->key)
		{
			remove(key, node->left);
		}

		else if (key > node->key)
		{
			remove(key, node->right);
		}

		else
		{
			// Найден узел
			if (node->left != nullptr && node->right != nullptr)
			{
				// Два ребенка: берем минимум из правого поддрева
				tree_node<K, V>* minNode = findMinPtr(node->right);
				node->key = minNode->key;
				node->value = minNode->value;
				remove(node->key, node->right);
			}
			else
			{
				// 0 или ребенок
				tree_node<K, V>* oldNode = node;
				node = (node->left != nullptr) ? node->left : node->right;
				delete oldNode;
				--size_;
			}
		}

		if (node != nullptr)
			balance(node);
		// TODO: Реализовать удаление узла из AVL дерева ready
		// 1. Найти узел с заданным ключом 1
		// 2. Обработать 4 случая:
		//    - Узел без детей (лист)  1
		//    - Узел с одним ребенком (левым или правым) 1
		//    - Узел с двумя детьми (заменить на минимальный из правого 1
		//    поддерева)
		// 3. Декрементировать size_ 1
		// 4. Вызвать balance(node) для балансировки 1
	}

	tree_node<K, V>* find(const K& key, tree_node<K, V>* node) const
	{
		if (!node)
		{
			return nullptr;
		}
		if (key < node->key)
		{
			return find(key, node->left);
		}
		if (key > node->key)
		{
			return find(key, node->right);
		}
		return node;
		// TODO: Реализовать поиск узла по ключу ready
		// Рекурсивно искать в левом или правом поддереве 1
	}

	tree_node<K, V>* findMinPtr(tree_node<K, V>* node)
	{
		if (!node)
		{
			return nullptr;
		}
		while (node->left)
		{
			node = node->left;
		}
		return node;
		// TODO: Найти узел с минимальным ключом в поддереве ready
		// Подсказка: идти влево, пока возможно 1
	}

	uint8_t heightOfTree(tree_node<K, V>* t)
	{
		if (!t)
		{
			return 0;
		}
		return t->height;
		// TODO: Вычислить высоту дерева ready
		// Высота пустого дерева = 0 1
		// Высота дерева = 1 + max(высота левого, высота правого) 1
	}

	void rotateWithLeftChild(tree_node<K, V>*& k2)
	{
		tree_node<K, V>* k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;

		k2->height =
			1 + std::max(heightOfTree(k2->left), heightOfTree(k2->right));
		k1->height =
			1 + std::max(heightOfTree(k1->left), heightOfTree(k1->right));

		k2 = k1;
		// TODO: Реализовать правый поворот (rotation with left child) ready
		// Используется для балансировки Left-Left случая 1
	}

	void rotateWithRightChild(tree_node<K, V>*& k1)
	{
		tree_node<K, V>* k2 = k1->right;
		k1->right = k2->left;

		k1->height =
			1 + std::max(heightOfTree(k1->left), heightOfTree(k1->right));
		k2->height =
			1 + std::max(heightOfTree(k2->left), heightOfTree(k2->right));

		k1 = k2;
		// TODO: Реализовать левый поворот (rotation with right child) ready
		// Используется для балансировки Right-Right случая 1
	}

	void doubleWithLeftChild(tree_node<K, V>*& k3)
	{
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
		// TODO: Реализовать двойной поворот Left-Right ready
		// Сначала левый поворот на левом ребенке, затем правый поворот на узле
		// 1
	}

	void doubleWithRightChild(tree_node<K, V>*& k1)
	{
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
		// TODO: Реализовать двойной поворот Right-Left ready
		// Сначала правый поворот на правом ребенке, затем левый поворот на узле
		// 1
	}

	void balance(tree_node<K, V>*& t)
	{
		if (t == nullptr)
			return;

		if (heightOfTree(t->left) - heightOfTree(t->right) > 1)
		{
			if (heightOfTree(t->left->left) >= heightOfTree(t->left->right))
				rotateWithLeftChild(t);	 // LL
			else
				doubleWithLeftChild(t);	 // LR
		}
		else if (heightOfTree(t->right) - heightOfTree(t->right->left))
		{
			if (heightOfTree(t->right->right) >= heightOfTree(t->right->left))
				rotateWithRightChild(t);  // RR
			else
				doubleWithRightChild(t);  // RL
		}

		t->height = 1 + std::max(heightOfTree(t->left), heightOfTree(t->right));
		// TODO: Реализовать балансировку AVL дерева ready
		// 1. Вычислить разность высот левого и правого поддеревьев 1
		// 2. Если разность > 1, выполнить соответствующие повороты: 1
		//    - Left-Left: rotateWithLeftChild 1
		//    - Left-Right: doubleWithLeftChild 1
		//    - Right-Right: rotateWithRightChild 1
		//    - Right-Left: doubleWithRightChild 1
	}

	void inorder_print(tree_node<K, V>* node)
	{
		if (node == nullptr)
		{
			return;
		}
		inorder_print(node->left);
		std::cout << "[" << node->key << ":" << node->value << "] ";
		inorder_print(node->right);
	}

	void clear(tree_node<K, V>* node)
	{
		if (node != nullptr)
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	void print_tree_(tree_node<K, V>* node, int space)
	{
		if (node == nullptr)
		{
			return;
		}
		space += 5;
		this->print_tree_(node->right, space);
		for (int i = 0; i < space; ++i)
		{
			std::cout << " ";
		}
		std::cout << node->key << ":" << node->value << "\n";
		this->print_tree_(node->left, space);
	}

	tree_node<K, V>* root_ = nullptr;
	size_t size_ = 0;
};

// ==================== Map Class ====================
// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
// Используя реализованное AVL-дерево, создайте полноценный аналог std::map
// с поддержкой вставки, удаления, поиска и итерации по элементам в порядке
// возрастания ключей.
template <typename K, typename V>
class map
{
   public:
	using key_type = K;
	using mapped_type = V;
	using value_type = std::pair<const K, V>;

	// ==================== Iterator ====================
	// ЗАДАНИЕ ДЛЯ СТУДЕНТОВ:
	// Реализуйте итератор для обхода элементов map в порядке возрастания ключей
	// (in-order обход AVL дерева). Итератор должен поддерживать операции
	// инкремента и разыменования.
	struct iterator : public abstract_iterator<iterator,
											   std::pair<const K, V>,
											   std::bidirectional_iterator_tag>
	{
		tree_node<K, V>* current_;
		std::stack<tree_node<K, V>*> stack_;
		mutable typename iterator::value_type pair_cache_;

		iterator() : current_(nullptr), pair_cache_(K{}, V{}) {}

		explicit iterator(tree_node<K, V>* root, bool is_end = false)
			: current_(nullptr), pair_cache_(K{}, V{})
		{
			// TODO: Реализовать инициализацию итератора
			// Для begin(): нужно найти самый левый узел
			// Для end(): current_ должен остаться nullptr
		}

		typename iterator::reference operator*() const override
		{
			// TODO: Реализовать разыменование
			// Нужно вернуть std::pair<const K, V> с текущим ключом и значением
			return pair_cache_;
		}

		typename iterator::pointer operator->() const override
		{
			// TODO: Реализовать оператор стрелки
			return &pair_cache_;
		}

		iterator& operator++() override
		{
			if (!current_)
			{
				return *this;
			}
			if (current_->right)
			{
				tree_node<K, V>* node = current_->right;
				while (node)
				{
					stack_.push(node);
					node = node->left;
				}
			}

			if (!stack_.empty())
			{
				current_ = stack_.top();
				stack_.pop();
			}
			else
			{
				current_ = nullptr;
			}

			// TODO: Реализовать переход к следующему элементу (in-order)
			// Подсказка: используйте стек для обхода дерева
			return *this;
		}

		iterator operator++(int) override
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		iterator& operator--() override
		{
			// Необязательно для bidirectional_iterator в этой реализации
			return *this;
		}

		iterator operator--(int) override
		{
			// Необязательно для bidirectional_iterator в этой реализации
			return *this;
		}

		iterator& operator+=(
			const typename iterator::difference_type& n) override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator& operator-=(
			const typename iterator::difference_type& n) override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator operator+(
			const typename iterator::difference_type& n) const override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		iterator operator-(
			const typename iterator::difference_type& n) const override
		{
			// Не требуется для bidirectional_iterator
			return *this;
		}

		bool operator==(const iterator& other) const override
		{
			return current_ == other.current_;
		}

		bool operator!=(const iterator& other) const override
		{
			return current_ != other.current_;
		}

		explicit operator bool() const override { return current_ != nullptr; }

		typename iterator::difference_type operator-(
			const iterator& other) const override
		{
			// Не требуется точное вычисление для bidirectional_iterator
			return 0;
		}
	};

	map() = default;
	~map() = default;

	void insert(const K& key, const V& value) { tree_.insert(key, value); }

	void insert(const value_type& pair)
	{
		tree_.insert(pair.first, pair.second);
	}

	V& operator[](const K& key)
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			tree_.insert(key, V());
			node = tree_.find(key);
		}
		return node->value;
	}

	V* find(const K& key)
	{
		auto node = tree_.find(key);
		return node ? &node->value : nullptr;
	}

	const V* find(const K& key) const
	{
		auto node = tree_.find(key);
		return node ? &node->value : nullptr;
	}

	V& at(const K& key)
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			throw std::out_of_range("Key not found in map");
		}
		return node->value;
	}

	const V& at(const K& key) const
	{
		auto node = tree_.find(key);
		if (node == nullptr)
		{
			throw std::out_of_range("Key not found in map");
		}
		return node->value;
	}

	// Удаление
	void erase(const K& key) { tree_.remove(key); }

	// Проверка наличия ключа
	bool contains(const K& key) const { return tree_.contains(key); }

	// Размер и проверка на пустоту
	size_t size() const { return tree_.size(); }

	bool empty() const { return tree_.empty(); }

	// Очистка
	void clear()
	{
		tree_.~avl_balanced_tree();
		new (&tree_) avl_balanced_tree<K, V>();
	}

	void print() { tree_.print(); }

	void inorder_print() { tree_.inorder_print(); }

	// Итераторы
	iterator begin() { return iterator(tree_.get_root(), false); }

	iterator end() { return iterator(tree_.get_root(), true); }

   private:
	avl_balanced_tree<K, V> tree_;
};

}  // namespace bmstu