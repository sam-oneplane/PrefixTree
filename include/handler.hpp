#include <thread>
#include <functional>
#include <string>
#include <iostream>
#include <memory>
#include <future>
#include "trie.hpp"


class Thread {
    std::thread t;
public:
    
    /* constructor that creates the thread with perfect forwarding*/
    template <typename Callable>
    Thread(Callable func): t(std::forward<Callable>(func)) {}
    ~Thread() {
    	if (t.joinable())
            t.join();
        std::cout << "joining thread\n";
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = default;
    Thread& operator=(Thread&&) = default;
};


class Init {
    const std::vector<std::string> words;
public:
    Init(const std::vector<std::string> &data): words(data) {};

    void operator()(PrefixTree &prefix_tree) {
        
        for (const std::string &word: words){
            prefix_tree.insert(word);
        }
    }
};


class Insert {
    
public:
    Insert() = default  ;

    void operator()(PrefixTree &prefix_tree, const std::string &word) {
        
        prefix_tree.insert(word);
    }
};


class CollectByPrefix {

public:
    CollectByPrefix() = default;  

    void operator()(std::promise<std::vector<std::string>> &&results,
                    PrefixTree &prefix_tree, 
                    const std::string &prefix) {
        // set the value to be retrived by the future         
        results.set_value( prefix_tree.collector_by_prefix(prefix));
    }
    
};

class Handler {

    PrefixTree trie;
    Init init_trie;
    Insert insert_trie;
    CollectByPrefix collect_by_prefix;
    

public:


    Handler(const std::vector<std::string> &words): init_trie(words){}

    void init() {
         // Threads will be automatically joined when destructors are called
        std::cout << "Init running... waiting for completion\n";

        Thread init_thread([this]() { init_trie(trie); });
    }

    void insert(const std::string &word) {
        Thread insert_thread([this, &word]() { insert_trie(trie, word); });
    }

    const std::vector<std::string> collect(const std::string &prefix) {
        // 1. create a promise object 
        std::promise<std::vector<std::string>> results;
        // 2. get future associated with promise
        std::future<std::vector<std::string>> future_obj = results.get_future();

        Thread collect_from_trie( [this, &prefix, &results]() { collect_by_prefix(std::move(results), trie, prefix); } );
        // 3. get value from future. block until promise sets the value
        return future_obj.get();
    }  
    
    PrefixTree  get()  {return trie;}

    ~Handler() {std::cout << "Handler terminating...\n";}
};


