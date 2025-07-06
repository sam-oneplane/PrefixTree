#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
/*
TriaNode 
*/

class TrieNode {
public:
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool terminal; // if true signal a word in the tree node 
    
    TrieNode() : terminal(false) {}
};

class PrefixTree {

    std::shared_ptr<TrieNode> root;
    mutable std::mutex mu;

    void collect_words(std::shared_ptr<TrieNode> node, 
                        const std::string &prefix, 
                        std::vector<std::string>& results) {
        if (!node) // end of tree
            return;

        if (node->terminal)  // prefix found as a word
            results.push_back(prefix);
        
        for (const auto & [chr, child_node]: node->children) 
            collect_words(child_node, prefix+chr, results);
    }

    bool remove_halper(std::shared_ptr<TrieNode> &node, const std::string &word, int index) {
        // stop cratiria
        if (index == word.length()){ //rechead the end of word
            if (!node->terminal)
                return false;
            
            node->terminal = false;
                
            return node->children.empty();
        }

        char c = word[index];
        auto it = node->children.find(c);

        if (it == node->children.end()) // children keys  != c
            return false;

        bool should_delete = remove_halper(it->second, word, index+1) ;
        // if should delete erase the unordered_map child entry points to node
        if (should_delete) {
            node->children.erase(it);
            // erase only if not end of other word and end of tree
            return !node->terminal && node->children.empty();
        }
        return false;
    }

    bool will_update_trie(const std::string &str, std::shared_ptr<TrieNode> &current) {
        for (char c : str) {
            if (current->children.find(c) == current->children.end()) // didnt find child with key of char c; return false
                return false;
            current = current->children[c];

        }
        return true;
    }

public:

    PrefixTree(): root(std::make_shared<TrieNode>()){};

    PrefixTree(const PrefixTree& other) {root = other.root;}

    void insert(const std::string & word) {
        // obtain raw pointer to Node owned by Root
        std::shared_ptr<TrieNode> current = root;
        std::cout << "inserting " << word << std::endl;
        for (char c : word) {
            std::lock_guard<std::mutex> guard(mu);
            if (current->children.find(c) ==  current->children.end()) { // didn't find c make entry with unique_ptr
                current->children[c] = std::make_shared<TrieNode>();
            }
            // advance current to new entry
            current = current->children[c] ;
        }
        current->terminal = true;
    }

    bool start_with_prefix(const std::string &prefix) {
        
        std::shared_ptr<TrieNode> current  = root;

        std::lock_guard<std::mutex> guard(mu);
        return will_update_trie(prefix, current);
    }

    bool search(const std::string & word) {
        std::shared_ptr<TrieNode> current = root;

        std::lock_guard<std::mutex> guard(mu);
        if (!will_update_trie(word, current))
            return false;
        return current->terminal;
    }


    std::vector<std::string> collector_by_prefix(const std::string &prefix) {

        std::vector<std::string> results;
        std::shared_ptr<TrieNode> current = root;

        std::lock_guard<std::mutex> guard(mu);
        std::cout << "collecting by " << prefix << std::endl;
        if (!will_update_trie(prefix, current))
            return results;
        collect_words(current, prefix, results);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        return results;
    }

    bool remove(const std::string & word) {
        std::lock_guard<std::mutex> guard(mu);
        std::cout << "removing " << word << std::endl;
        return remove_halper(root, word, 0);
    }

};
