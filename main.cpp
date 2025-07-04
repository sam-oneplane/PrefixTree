#include "trie.hpp"
#include <iostream>


int main() {

    PrefixTree prefix_tree;

    std::vector<std::string> data {"bad", "go", "goodbye", "bee", "beef", "bear", "money", "no", "noon", "apple", "app", "apperol"};

    for (std::string &str: data) 
        prefix_tree.insert(str);

    std::vector<std::string> results;
    std::string prefix {"app"};

    if (prefix_tree.start_with_prefix(prefix)) {
        results = prefix_tree.collector_by_prefix(prefix);
    }

    for (std::string &str: results)
        std::cout << str << std::endl;
    
    results.clear();
    std::string word{"beef"};

    if (prefix_tree.search(word)) {
        bool tmp = prefix_tree.remove(word);

        prefix = "be";
        results = prefix_tree.collector_by_prefix(prefix);

        std::cout << "\n";
        for (std::string &str: results)
            std::cout << str << std::endl;
    }

    results.clear();
    prefix = "cs";

    results = prefix_tree.collector_by_prefix(prefix);
    std::cout << "results: {" << results.size() << "}" << std::endl;
    for (std::string &str: results)
        std::cout << str << std::endl;
    
    return 0;
}