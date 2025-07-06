#include "handler.hpp"
#include <iostream>


void test_prefix_tree(const std::vector<std::string> &data, PrefixTree &prefix_tree) {


    std::vector<std::string> results;
    std::string prefix {"app"};

    if (prefix_tree.start_with_prefix(prefix)) {
        results = prefix_tree.collector_by_prefix(prefix);
    }
    std::cout << "results: {" << results.size() << "}" << std::endl;
    for (std::string &str: results)
        std::cout << str << std::endl;
    
    results.clear();
    std::string word{"beef"};

    if (prefix_tree.search(word)) {
        bool tmp = prefix_tree.remove(word);

        prefix = "be";
        results = prefix_tree.collector_by_prefix(prefix);

        std::cout << "\n";
        std::cout << "results: {" << results.size() << "}" << std::endl;
        for (std::string &str: results)
            std::cout << str << std::endl;
    }

    results.clear();
    prefix = "cs";

    results = prefix_tree.collector_by_prefix(prefix);
    std::cout << "results: {" << results.size() << "}" << std::endl;
    for (std::string &str: results)
        std::cout << str << std::endl;

}

int main() {

    const std::vector<std::string> data {"bad", "go", "gone", "goodbye", "bee", "beef", "bear", "money", "no", "noon", "apple", "app", "apperol"};

    std::vector<std::string> results;

    Handler tree_handler(data);

    tree_handler.init();
    PrefixTree tree = tree_handler.get();

    std::string prefix {"go"};
    results = tree_handler.collect(prefix);

    std::cout << "results: {" << results.size() << "}" << std::endl;
    for (std::string &str: results)
        std::cout << str << std::endl;
    
    std::string word {"bark"};
    tree_handler.insert(word);
    word = "banana";
    tree_handler.insert(word);

    prefix = "ba";
    results = tree_handler.collect(prefix);

    std::cout << "results: {" << results.size() << "}" << std::endl;
    for (std::string &str: results)
        std::cout << str << std::endl;

    test_prefix_tree(data, tree);
    
    return 0;
}