#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map <int, int> map;

class Node
{
public:
	int key;
	Node* left;
	Node* right;
	int height;
	int lCount;
	int rCount;
};

int height(Node* N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

Node* newNode(int key)
{
	Node* node = new Node();
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	node->lCount = 0;
	node->rCount = 0;
	return(node);
}

Node* rightRotate(Node* y)
{
	Node* x = y->left;
	Node* T2 = x->right;

	x->right = y;
	y->left = T2;

	if (T2 == NULL)
		y->lCount = 0;
	else
		y->lCount = T2->lCount + T2->rCount + 1;
	x->rCount = y->lCount + y->rCount + 1;

	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}

Node* leftRotate(Node* x)
{
	Node* y = x->right;
	Node* T2 = y->left;

	y->left = x;
	x->right = T2;

	if (T2 == NULL)
		x->rCount = 0;
	else
		x->rCount = T2->lCount + T2->rCount + 1;
	y->lCount = x->lCount + x->rCount + 1;

	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}

int getBalance(Node* N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key)
{
	if (node == NULL)
		return(newNode(key));

	if (key <= node->key) {
		node->lCount++;
		node->left = insert(node->left, key);
	}
	else if (key > node->key) {
		node->rCount++;
		node->right = insert(node->right, key);
	}
	else
		return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

Node* minValueNode(Node* node)
{
	Node* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

Node* deleteNode(Node* root, int key)
{
	if (root == NULL)
		return root;

	if (key < root->key) {
		root->lCount--;
		root->left = deleteNode(root->left, key);
	}
	else if (key > root->key) {
		root->rCount--;
		root->right = deleteNode(root->right, key);
	}
	else
	{
		if ((root->left == NULL) || (root->right == NULL))
		{
			Node* temp = root->left ?
				root->left :
				root->right;

			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else
				*root = *temp;

			free(temp);
		}
		else
		{
			Node* temp = minValueNode(root->right);

			root->key = temp->key;

			root->rCount--;

			root->right = deleteNode(root->right, temp->key);
		}
	}

	if (root == NULL)
		return root;

	root->height = 1 + max(height(root->left),
		height(root->right));

	int balance = getBalance(root);

	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	if (balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

void kthSmallest(Node* root, int k)
{
	if (root == NULL)
		return;

	if (root->lCount + 1 == k) {
		cout << root->key;
		cout << "\n";
		return;
	}

	if (root->lCount + 1 > k) {
		kthSmallest(root->left, k);
	}
	else {
		kthSmallest(root->right, k - (root->lCount + 1));
	}

}

void findSuc(Node* root, Node*& suc, int key)
{
	if (root == NULL)  return;

	if (root->key == key)
	{
		if (root->right != NULL)
		{
			Node* tmp = root->right;
			if (root->right->key != tmp->key) {
				while (tmp->left)
					tmp = tmp->left;
			}
			else {
				tmp = suc;
			}


			if (suc == NULL) {
				suc = tmp;
				if (map[key] == 1)
					return;
			}
			else if (suc->key > tmp->key)
				suc = tmp;
		}
	}

	if (root->key > key)
	{
		suc = root;
		findSuc(root->left, suc, key);
	}
	else
	{
		findSuc(root->right, suc, key);
	}
}

void findPre(Node* root, Node*& pre, int key)
{
	if (root == NULL)  return;

	if (root->key == key)
	{
		if (root->left != NULL)
		{
			Node* tmp = root->left;
			if (root->left->key != tmp->key) {
				while (tmp->right)
					tmp = tmp->right;
			}
			else {
				tmp = pre;
			}

			if (pre == NULL) {
				pre = tmp;
				if (map[key] == 1)
					return;
			}
			else if (pre->key > tmp->key)
				pre = tmp;
		}
	}

	if (root->key >= key)
	{
		findPre(root->left, pre, key);
	}
	else
	{
		pre = root;
		findPre(root->right, pre, key);
	}
}

void findOrder(Node* r, int n, Node*& order, int p) {
	if (r == NULL)
		return;

	if (r->key == n) {
		p += r->lCount + 1;

		if (order == NULL) {
			order = new Node();
			order->key = p;
			if (map[n] == 1)
				return;
		}
		else if (order->key > p) {
			order->key = p;
		}
		p -= r->lCount + 1;
	}

	if (n <= r->key) {
		findOrder(r->left, n, order, p);
	}
	else {
		p += r->lCount + 1;
		findOrder(r->right, n, order, p);
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	int operation;

	while (cin >> operation) {
		Node* root = NULL;

		int num;
		int position = 0;
		Node* order = NULL;
		Node* pre = NULL;
		Node* suc = NULL;

		for (int i = 0; i < operation; i++) {
			int move;
			cin >> move;

			switch (move) {
			case 1:
				cin >> num;

				root = insert(root, num);
				map[num]++;
				break;
			case 2:
				cin >> num;

				if (map.find(num) != map.end()) {
					if (map[num] > 0) {
						root = deleteNode(root, num);
						map[num]--;
					}
				}
				break;
			case 3:
				cin >> num;

				position = 0;
				order = NULL;
				findOrder(root, num, order, position);

				cout << order->key;
				cout << "\n";
				break;
			case 4:
				cin >> num;

				kthSmallest(root, num);
				break;
			case 5:
				cin >> num;

				pre = NULL;

				findPre(root, pre, num);

				cout << pre->key;
				cout << "\n";
				break;
			case 6:
				cin >> num;

				suc = NULL;

				findSuc(root, suc, num);

				cout << suc->key;
				cout << "\n";
				break;
			}
		}
		map.clear();
	}
	return 0;
}