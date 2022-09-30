#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<mutex>
#include<fstream>
#include <functional>
#include<vector>
#include<string>
using namespace std;
template<typename K, typename V>
class Node{
public:
    K m_key;
    V m_value;
    Node<K,V> *left, *right;
    int height;

    Node(K key, V value):m_key(key), 
                        m_value(value), 
                        left(nullptr), 
                        right(nullptr),
                        height(1){}
};

template<typename K, typename V, typename Compared = std::less<K>>
class AVLTree{
private:
    Node<K,V>* root;
    int size;
    Compared comp;
public:
    AVLTree():root(nullptr), size(0){}
    int getSize(){
        return size;
    }

    bool empty(){
        return size == 0;
    }

    bool isBST(){
        vector<K> keys;
        inOrder(root, keys);
        for (int i = 1; i < keys.size(); i++) {
            if (comp(keys[i], keys[i-1]))//keys[i] < keys[i-1]
                return false;
        }
        return true;
    }

    bool isBalanced(){
        return isBalanced(root);
    }

    void add(K key, V value) {
        root = add(root, key, value);
    }

    bool contains(K key) {
        return getNode(root, key) != nullptr;
    }

    V get(K key) {
        Node<K, V>* node = getNode(root, key);
        return node == nullptr ? 0 : node->m_value;
    }

    V remove(K key) {
        Node<K, V>* node = getNode(root, key);
        if (node != nullptr) {
            root = remove(root, key);
            return node->m_value;
        }
        return 0;
    }

private:
    bool isBalanced(Node<K,V>* node){
        if (node == nullptr)
            return true;
        
        int balanceFactor = getBalanceFactor(node);
        if (abs(balanceFactor) > 1)
            return false;
        return isBalanced(node->left) && isBalanced(node->right);
    }

    void inOrder(Node<K,V>*node, vector<K> keys) {
        if (node == nullptr)
            return;
        
        keys.push_back(node->m_key);
        inOrder(node->left, keys);
        inOrder(node->right, keys);
    }

    int getHeight(Node<K, V>* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalanceFactor(Node<K, V>* node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    Node<K, V>* rightRotate(Node<K,V>* y) {
        Node<K, V>* x = y->left;
        Node<K, V>* T = x->right;

        x->right = y;
        y->left = T;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    Node<K, V>* leftRotate(Node<K, V>* y) {
        Node<K, V>* x = y->right;
        Node<K, V>* T = x->left;

        x->left = y;
        y->right = T;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        
        return x;
    }

    Node<K, V>* add(Node<K, V>* node, K key, V value) {
        if (node == nullptr) {
            size++;
            return new Node<K,V>(key, value);
        }

        if (comp(key, node->m_key)) { //key < m_key
            node->left = add(node->left, key, value);
        } else if (comp(node->m_key, key)) {//m_key < key
            node->right = add(node->right, key, value);
        } else {
            node->m_value = value;
        }

        //update height
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

        //caculate balance factor
        int balanceFactor = getBalanceFactor(node);

        //LL
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
            return rightRotate(node);
        }

        //RR
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
            return leftRotate(node);
        }

        //LR
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        //RL
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node<K, V>* getNode(Node<K, V>* node, K key) {
        if (node == nullptr)
            return nullptr;

        if (comp(key, node->m_key)) {//key < node->m_key
            return getNode(node->left, key);
        } else if (comp(node->m_key, key)) {//node->m_key < key
            return getNode(node->left, key);
        } else {
            return node;
        }
    }

    Node<K, V>* minmum(Node<K, V>* node) {
        if (node->left == nullptr)
            return node;
        return minmum(node->left);
    }

    Node<K, V>* removeMin(Node<K, V>* node) {
        if (node->left == nullptr) {
            Node<K, V>* rightNode = node->right;
            node->right = nullptr;
            size--;
            // delete node;
            // node = nullptr;
            return rightNode;
        }
        return node->left = removeMin(node->left);
    }
    
    Node<K, V>* remove(Node<K, V>* node, K key) {
        if (node == nullptr)
            return nullptr;
        
        Node<K, V>* retNode;
        if (comp(key, node->m_key)) {//key < node->m_key
            node->left = remove(node->left, key);
            retNode = node;
        } else if (comp(node->m_key, key)) {//m_key < key
            node->right = remove(node->right, key);
            retNode = node;
        } else {//key == m_key
            if (node->left == nullptr) {
                Node<K, V>* rightNode = node->right;
                node->right = nullptr;
                // delete node;
                // node = nullptr;
                size--;
                retNode = rightNode;
            } else if (node->right ==nullptr) {
                Node<K, V>* leftNode = node->left;
                node->left = nullptr;
                // delete node;
                // node = nullptr;
                size--;
                retNode = leftNode;
            } else {
                Node<K, V>* successor = minmum(node->right);
                successor->right = removeMin(node->right);
                successor->left = node->left;
                retNode = successor;
            }
        }
        if (retNode == nullptr)
            return nullptr;
        
        //updata height
        retNode->height = max(getHeight(retNode->left), getHeight(retNode->right)) + 1;

        //caculate balance factor
        int balanceFactor = getBalanceFactor(retNode);

        //
        //LL
        if (balanceFactor > 1 && getBalanceFactor(retNode->left) >= 0){
            return rightRotate(retNode);
        }

        //RR
        if (balanceFactor < -1 && getBalanceFactor(retNode->right) <= 0){
            return leftRotate(retNode);
        }

        //LR
        if (balanceFactor > 1 && getBalanceFactor(retNode->left) < 0) {
            retNode->left =  leftRotate(retNode);
            return rightRotate(retNode);
        }

        //RL
        if (balanceFactor < -1 && getBalanceFactor(retNode->right) > 0) {
            retNode->right = rightRotate(retNode->right);
            return leftRotate(retNode);
        }
        return retNode;
    }
    
};

template<typename K>
struct cmp{
    bool operator()(K & a, K & b){
        return a < b;
    }
};
int main(void){
    vector<string> words = {"hello", "hello", "hello",
    "happy", "happy", "key", "key", "delete", "delete"};

    AVLTree<string, int, cmp<string>> tree;
    for (auto & str : words) {
        if (tree.contains(str)) {
            int a = tree.get(str) + 1;
            tree.add(str, a);
        } else {
            tree.add(str, 1);
        }
    }

    cout << "is BST:" << tree.isBST() << endl;
    cout << "is balance:" << tree.isBalanced() << endl;
    
    for (auto & str : words) {
        tree.remove(str);
        if (!tree.isBalanced() || !tree.isBST()) {
            cout << "Error" << endl;
        }
    }
}
