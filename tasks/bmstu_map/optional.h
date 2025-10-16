#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
namespace bmstu
{
template <typename K>
struct tree_node
{
  tree_node(K data) : key(data), left(nullptr), right(nullptr), height(1) {}

  K key;
  uint8_t height;
  tree_node* left;
  tree_node* right;
};

template <typename K>
class map
{
   public:
  map() : root_(nullptr), size_(0){};
  ~map() {}
  void insert(const K& key) { this->insert(key, root_); }
  void print() { print_tree_(root_, 1); }
  void remove(const K& key) {this -> remove(key, root_); }
  
  void inorder_print()
    {
        inorder_print(root_);
        std::cout << "\n";
    }

   private:
   void remove (const K& key, tree_node<K>*& node)
   {
    if (node == nullptr)
    {
        return;
    }
    else if(key == node->key)
    {
      --size_;
    if (node->left == nullptr && node->right == nullptr)
    {
        delete node;
        node = nullptr;
    }
    else if (node->left != nullptr && node->right == nullptr)
    {
      auto ptr= node;
      node = node->left;
      delete ptr;
    }
    else if (node->left == nullptr && node->right != nullptr)
    {
      auto ptr= node;
      node = node->right;
      delete ptr;
    } else 
    {
      auto minNode = findMin(node->right);
      node->key = minNode->key;
      removeMin(node->right);
    }
    } else if(key < node->key)
    {
      this->remove(key, node->left);
    }
    else if(key > node->key)
    {
      this->remove(key, node->right);
    }
    balance(node);
   }
  tree_node<K>*& findMin(tree_node<K>*& node)
  {
    if (node == nullptr)
    {
      throw std::runtime_error("Tree is empty");
    }
    else if (node -> left == nullptr)
    {
      return node;
    }
    else
    {
      return findMin(node->left);
    }
  }

  tree_node<K>* insert(const K& key, tree_node<K>*& node)
  {
    if (node == nullptr)
    {
      node = new tree_node<K>(key);
      ++size_;
    }
    else if (key < node->key)
    {
      insert(key, node->left);
    }
    else if (key > node->key)
    {
      insert(key, node->right);
    }
    else
    {
      return node;
    }
    balance(node);
    return node;
  }

  int getBalance(tree_node<K>* node)
  {
    if (node == nullptr)
      return 0;

    return heightOfTree(node->left) - heightOfTree(node->right);
  }

  uint8_t heightOfTree(tree_node<K>* t)
  {
    if (t == nullptr)
    {
      return 0;
    }
    else
    {
      return 1 + std::max(heightOfTree(t->left), heightOfTree(t->right));
    }
  }

  void rotateWithLeftChild(tree_node<K>*& k2)
  {
    auto k1 = k2->left;
    k2->left = k1->right;
    k2->height =
      std::max(heightOfTree(k2->left), heightOfTree(k2->right)) + 1;
    k1->height = std::max(heightOfTree(k1->left), k2->height) + 1;
    k1->right = k2;
    k2 = k1;
  }
  void removeMin(tree_node<K>*& node) {
    if (node->left == nullptr) {
        tree_node<K>* temp = node;
        node = node->right;
        delete temp;
    } else {
        removeMin(node->left);
        if (node != nullptr) {
            balance(node);
        }
      }
    }
  void rotateWithRightChild(tree_node<K>*& k1)
  {
    auto k2 = k1->right;
    k1->right = k2->left;
    k1->height =
      std::max(heightOfTree(k1->left), heightOfTree(k1->right)) + 1;
    k2->height = std::max(heightOfTree(k2->right), k1->height) + 1;
    k2->left = k1;
    k1 = k2;
  }

  void doubleWithLeftChild(tree_node<K>*& k3)
  {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
  }

  void doubleWithRightChild(tree_node<K>*& k1)
  {
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
  }

  void balance(tree_node<K>*& t)
  {
    static int ALLOWED_IMBALANCE = 1;
    if (t == nullptr)
    {
      return;
    }

    if (heightOfTree(t->left) - heightOfTree(t->right) > ALLOWED_IMBALANCE)
    {
      if (heightOfTree(t->left->left) >= heightOfTree(t->left->right))
      {
        this->rotateWithLeftChild(t);
      }
      else
      {
        this->doubleWithLeftChild(t);
      }
    }
    else if (heightOfTree(t->right) - heightOfTree(t->left) >
         ALLOWED_IMBALANCE)
    {
      if (heightOfTree(t->right->right) >= heightOfTree(t->right->left))
      {
        this->rotateWithRightChild(t);
      }
      else
      {
        this->doubleWithRightChild(t);
      }
    }
  }
  void print_tree_(tree_node<K>* node, int space)
  {
    if (node == nullptr)
    {
      return;
    }
    space += size_;
    this->print_tree_(node->right, space);
    for (int i = 0; i < space; ++i)
    {
      std::cout << " ";
    }
    std::cout << node->key << "\n";
    this->print_tree_(node->left, space);
  }
  void inorder_print(tree_node<K>* node){
    if (node == nullptr){
        return;
    }
    inorder_print(node->left);
    std::cout<< "[" << node -> key <<"]";
    inorder_print(node->right);
  }
  tree_node<K>* root_ = nullptr;
  size_t size_ = 0;
};
}
 // namespace bmstu