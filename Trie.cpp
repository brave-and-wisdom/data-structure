#include<iostream>
#include<map>
#include<string>
#include<unordered_map>
using namespace std;
struct Node {
    bool isWord;
    //还是得用指针
    map<char, Node*> next;
    Node(bool _isWord):isWord(_isWord){}
    Node():isWord(false){}
};
class Trie {
private:
Node* root;
int size;
public:
    Trie() {
        root = new Node();
        size = 0;
    }
    
    void insert(string word) {
        Node* cur = root;
        for (auto& ch : word) {
            if (cur->next.count(ch) == 0) {
                //cur.next.insert({ch, Node()});
                cur->next[ch] = new Node();
            }
            cur = cur->next[ch];
        }
        if (cur->isWord == false) {
            cur->isWord = true;
            size++;
        }
    }
    
    bool search(string word) {
        Node* cur = root;
        for (auto& ch : word) {
            if (cur->next.count(ch) == 0) {
                return false;
            } 
            cur = cur->next[ch];
        }
        return cur->isWord;
    }
    
    bool startsWith(string prefix) {
        Node* cur = root;
        for (auto& ch : prefix) {
            if (cur->next.count(ch) == 0) {
                return false;
            } 
            cur = cur->next[ch];
        }
        return true;
    }
};

int main(void) {
    
    string word = "apple";
    string prefix = "app";
    // Trie* obj = new Trie();
    // obj->insert(word);
    // bool param_2 = obj->search(word);
    // bool param_3 = obj->startsWith(prefix);
    Trie trie;
    trie.insert("apple");
    cout << trie.search("apple") << endl;   // 返回 True
    cout << trie.search("app") << endl;     // 返回 False
    cout << trie.startsWith("app") << endl; // 返回 True
    // trie.insert("app");
    // cout << trie.search("app");    
}
