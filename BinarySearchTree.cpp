#include <iostream>
#include <string>
#include <functional>
#include <utility>

//Declaration for Point2D class
class Point2D
{
public:
	Point2D() {}
	Point2D(int x, int y) { coordinate.first = x; coordinate.second = y; }
	bool operator==(const Point2D& point) { return (coordinate.first == point.coordinate.first && coordinate.second == point.coordinate.second); }

	std::pair<int, int> coordinate;
};

std::ostream& operator<<(std::ostream& out, const Point2D& point)
{
	out << "(" << point.coordinate.first << ", " << point.coordinate.second << ")";
	return out;
}

//Declaration for PointOrderx class
class PointOrderx
{
public:
	bool operator()(const Point2D& pointA, const Point2D& pointB) { return pointA.coordinate.first < pointB.coordinate.first; }
};

//Declaration for PointOrdery class
class PointOrdery
{
public:
	bool operator()(const Point2D& pointA, const Point2D& pointB) { return pointA.coordinate.second < pointB.coordinate.second; }
};

//Declaration for TreeNode class
template<typename T, typename compare = std::less<T> >
class TreeNode
{
public:
	void insert_node(TreeNode<T, compare>* new_node);
	void print_nodes()const;
	bool find(T value)const;
	~TreeNode();
private:
	T data;
	TreeNode<T, compare>* left;
	TreeNode<T, compare>* right;
	TreeNode<T, compare>* parent;
	template<typename T, typename compare = std::less<T> > friend class BinarySearchTree;
	template<typename T, typename compare = std::less<T> > friend class TreeIterator;
};

//Declaration for TreeIterator class
template<typename T, typename compare = std::less<T> >
class TreeIterator
{
public:
	TreeIterator<T, compare>& operator++();
	TreeIterator<T, compare> operator++(int n);
	TreeIterator<T, compare>& operator--();
	bool operator!=(const TreeIterator<T, compare>& it)const;
	T operator*()const;

	TreeNode<T, compare>* iterator;
};

//Declaration for BinarySearchTree class
template<typename T, typename compare = std::less<T> >
class BinarySearchTree
{
public:
	BinarySearchTree();
	BinarySearchTree(const BinarySearchTree<T, compare>& copy);
	BinarySearchTree(BinarySearchTree<T, compare>&& copy);
	BinarySearchTree<T, compare>& operator=(BinarySearchTree<T, compare> copy);
	~BinarySearchTree();
	void insert(T data);
	void erase(T data);
	bool count(T data)const;
	void print()const;
	T smallest()const;
	T largest()const;
	TreeIterator<T, compare> begin()const;
	TreeIterator<T, compare> end()const;
private:
	TreeNode<T, compare>* root;
};

template<typename T, typename compare = std::less<T> >
std::ostream& operator<<(std::ostream& out, const TreeIterator<T, compare>& it);

/*  Find the right spot and insert
Parameter: TreeNode pointer
Return: None
*/
template<typename T, typename compare = std::less<T> >
void TreeNode<T, compare>::insert_node(TreeNode<T, compare>* new_node)
{
	if (new_node->data == data)
		return;

	compare cmp;
	if (cmp(new_node->data, data))
	{
		if (left == nullptr)
		{
			left = new_node;
			new_node->parent = this;
		}
		else
			left->insert_node(new_node);
	}
	else if (!cmp(new_node->data, data))
	{
		if (right == nullptr)
		{
			right = new_node;
			new_node->parent = this;
		}
		else
			right->insert_node(new_node);
	}
}

/*  Print out all the value in the tree
Parameter: None
Return: None
*/
template<typename T, typename compare = std::less<T> >
void TreeNode<T, compare>::print_nodes()const
{
	if (left != nullptr)
		left->print_nodes();

	std::cout << data << std::endl;

	if (right != nullptr)
		right->print_nodes();
}

/*  See whether the value exist in the tree
Parameter: int
Return: bool
*/
template<typename T, typename compare = std::less<T> >
bool TreeNode<T, compare>::find(T value)const
{
	if (value == data)
		return true;

	compare cmp;
	if (cmp(value, data))
	{
		if (left == nullptr)
			return false;
		else
			return left->find(value);
	}
	else if (!cmp(value, data))
	{
		if (right == nullptr)
			return false;
		else
			return right->find(value);
	}
}

/*  Destructor for TreeNode
Parameter: None
Return: None
*/
template<typename T, typename compare = std::less<T> >
TreeNode<T, compare>::~TreeNode()
{
	delete left;
	delete right;
}

/*  Default constructor
Parameter: None
Return: None
*/
template<typename T, typename compare = std::less<T> >
BinarySearchTree<T, compare>::BinarySearchTree()
{
	root = nullptr;
}

/*  Destructor for Binary Search Tree
Parameter: None
Return: None
*/
template<typename T, typename compare = std::less<T> >
BinarySearchTree<T, compare>::~BinarySearchTree()
{
	delete root;
}

/*  Insert a value into the tree
Parameter: int
Return: None
*/
template<typename T, typename compare = std::less<T> >
void BinarySearchTree<T, compare>::insert(T data)
{
	TreeNode<T, compare>* new_node = new TreeNode<T, compare>;
	new_node->data = data;
	new_node->left = nullptr;
	new_node->right = nullptr;
	new_node->parent = nullptr;
	if (root == nullptr)
		root = new_node;
	else
		root->insert_node(new_node);
}

/*  Erase a node in the tree
Parameter: int
Return: None
*/
template<typename T, typename compare = std::less<T> >
void BinarySearchTree<T, compare>::erase(T data)
{
	TreeNode<T, compare>* to_be_removed = root;
	TreeNode<T, compare>* parent = nullptr;
	compare  cmp;
	bool found = false;

	while (!found && to_be_removed != nullptr)
	{
		if (to_be_removed->data == data)
		{
			found = true;
			break;
		}
		if (cmp(to_be_removed->data, data))
		{
			parent = to_be_removed;
			to_be_removed = to_be_removed->right;
		}
		else if (!cmp(to_be_removed->data, data))
		{
			parent = to_be_removed;
			to_be_removed = to_be_removed->left;
		}
	}

	if (!found) return;

	if (to_be_removed->left == nullptr || to_be_removed->right == nullptr)
	{
		TreeNode<T, compare>* new_child;
		if (to_be_removed->left == nullptr)
			new_child = to_be_removed->right;
		else
			new_child = to_be_removed->left;

		if (parent == nullptr)
			root = new_child;
		else if (parent->left == to_be_removed)
		{
			parent->left = new_child;
			new_child->parent = parent;
			delete to_be_removed;
		}
		else
		{
			parent->right = new_child;
			new_child->parent = parent;
			delete to_be_removed;
		}
		return;
	}

	TreeNode<T, compare>* largest_parent = to_be_removed;
	TreeNode<T, compare>* largest = to_be_removed->left;
	while (largest->right != nullptr)
	{
		largest_parent = largest;
		largest = largest->right;
	}

	to_be_removed->data = largest->data;
	if (largest_parent == to_be_removed)
	{
		largest_parent->left = largest->left;
		if (largest->left != nullptr)
			largest->left->parent = largest_parent;
	}
	else
	{
		largest_parent->right = largest->left;
		if (largest->left != nullptr)
			largest->left->parent = largest_parent;
	}

	delete largest;
}

/*  See if the data is in the tree
Parameter: int
Return: bool
*/
template<typename T, typename compare = std::less<T> >
bool BinarySearchTree<T, compare>::count(T data)const
{
	if (root == nullptr)
		return false;
	else if (root->find(data))
		return true;
	else
		return false;
}

/*  Print out all the value in the tree
Parameter: None
Return: None
*/
template<typename T, typename compare = std::less<T> >
void BinarySearchTree<T, compare>::print()const
{
	if (root != nullptr)
		root->print_nodes();
}

/*  Return the smallest value in a tree
Parameter: None
Return: int
*/
template<typename T, typename compare = std::less<T> >
T BinarySearchTree<T, compare>::smallest()const
{
	T result;
	for (TreeNode<T, compare>* ptr = root; ptr != nullptr; ptr = ptr->left)
		result = ptr->data;
	return result;
}

/*  Return the largest value in a tree
Parameter: None
Return: int
*/
template<typename T, typename compare = std::less<T> >
T BinarySearchTree<T, compare>::largest()const
{
	T result;
	for (TreeNode<T, compare>* ptr = root; ptr != nullptr; ptr = ptr->right)
		result = ptr->data;
	return result;
}

/*  Begin function for iterating
Parameter: None
Return: TreeIterator
*/
template<typename T, typename compare = std::less<T> >
TreeIterator<T, compare> BinarySearchTree<T, compare>::begin()const
{
	TreeIterator<T, compare> it;
	for (it.iterator = root; it.iterator->left != nullptr; it.iterator = it.iterator->left);
	return it;
}

/*  End function for iterating
Parameter: None
Return: TreeIterator
*/
template<typename T, typename compare = std::less<T> >
TreeIterator<T, compare> BinarySearchTree<T, compare>::end()const
{
	TreeIterator<T, compare> it;
	for (it.iterator = root; it.iterator != nullptr; it.iterator = it.iterator->right);
	return it;
}

/*  Overload operator <<
Parameter: ostream reference, const TreeIterator reference
Return: ostream reference
*/
template<typename T, typename compare = std::less<T> >
std::ostream& operator<<(std::ostream& out, const TreeIterator<T, compare>& it)
{
	out << *it;
	return out;
}

/*	Prefix overload operator ++
Parameter: None
Return: TreeIterator reference
*/
template<typename T, typename compare = std::less<T> >
TreeIterator<T, compare>& TreeIterator<T, compare>::operator++()
{
	TreeNode<T, compare>* temp;
	if (this->iterator == nullptr)
	{
		std::cerr << "Error! Cannot increment null iterator" << std::endl;
		exit(1);
	}
	else
	{
		if (this->iterator->right != nullptr)
		{
			this->iterator = this->iterator->right;
			while (this->iterator->left != nullptr)
				this->iterator = this->iterator->left;
		}
		else
		{
			temp = this->iterator->parent;
			while (temp != nullptr && this->iterator == temp->right)
			{
				this->iterator = temp;
				temp = temp->parent;
			}
			this->iterator = temp;
		}
	}
	return *this;
}

/*  Postfix overload operator ++
Parameter: dummy int
Return: TreeIterator
*/
template<typename T, typename compare = std::less<T> >
TreeIterator<T, compare> TreeIterator<T, compare>::operator++(int n)
{
	TreeNode<T, compare>* temp;
	if (this->iterator == nullptr)
	{
		std::cerr << "Error! Cannot increment null iterator" << std::endl;
		exit(1);
	}
	else
	{
		if (this->iterator->right != nullptr)
		{
			this->iterator = this->iterator->right;
			while (this->iterator->left != nullptr)
				this->iterator = this->iterator->left;
		}
		else
		{
			temp = this->iterator->parent;
			while (temp != nullptr && this->iterator == temp->right)
			{
				this->iterator = temp;
				temp = temp->parent;
			}
			this->iterator = temp;
		}
	}
	return *this;
}

/*  Overload operator --
Parameter: None
Return: TreeIterator reference
*/
template<typename T, typename compare = std::less<T> >
TreeIterator<T, compare>& TreeIterator<T, compare>::operator--()
{
	TreeNode<T, compare>* temp;
	if (this->iterator == nullptr)
	{
		std::cerr << "Error! Cannot increment null iterator" << std::endl;
		exit(1);
	}
	else
	{
		if (this->iterator->right != nullptr)
		{
			this->iterator = this->iterator->left;
			while (this->iterator->right != nullptr)
				this->iterator = this->iterator->right;
		}
		else
		{
			if (this->iterator->left == nullptr)
			{
				temp = this->iterator->parent;
				while (temp != nullptr && this->iterator == temp->left)
				{
					this->iterator = temp;
					temp = temp->parent;
				}
				this->iterator = temp;
			}
			else
			{
				this->iterator = this->iterator->left;
				while (this->iterator->right != nullptr)
					this->iterator = this->iterator->right;
			}
		}
	}
	return *this;
}

/*  Overload operator !=
Parameter: const TreeIterator reference
Return: bool
*/
template<typename T, typename compare = std::less<T> >
bool TreeIterator<T, compare>::operator!=(const TreeIterator<T, compare>& it)const
{
	return this->iterator != it.iterator;
}

/*  Overload operator *
Parameter: const TreeIterator reference
Return: int
*/
template<typename T, typename compare = std::less<T> >
T TreeIterator<T, compare>::operator*()const
{
	return this->iterator->data;
}

/*  Copy constructor for Binary Search Tree
Parameter: const BinarySearchTree reference
Return: None
*/
template<typename T, typename compare = std::less<T> >
BinarySearchTree<T, compare>::BinarySearchTree(const BinarySearchTree<T, compare>& copy)
{
	auto start = std::begin(copy);
	auto stop = std::end(copy);

	while (start != stop)
	{
		this->insert(*start);
		++start;
	}
	std::cout << "Copy made" << std::endl;
}

/*  Move constructor for Binary Search Tree
	Parameter: BinarySearchTree rvalue reference
	Return: None
*/
template<typename T, typename compare = std::less<T> >
BinarySearchTree<T, compare>::BinarySearchTree(BinarySearchTree<T, compare>&& copy)
{
	this->root = copy.root;
	copy.root = nullptr;
	std::cout << "Move performed" << std::endl;
}

/*	Overloaded assignment operator for Binary search tree
	Parameter: BinarySearchTree(copy and swap idiom)
	Return: BinarySearchTree reference
*/
template<typename T, typename compare = std::less<T> >
BinarySearchTree<T, compare>& BinarySearchTree<T, compare>::operator=(BinarySearchTree<T, compare> dummy)
{
	std::swap(this->root, dummy.root);
	return *this;
}

