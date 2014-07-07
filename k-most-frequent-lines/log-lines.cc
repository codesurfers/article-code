#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <functional>

using std::cout;
using std::endl;
using std::cerr;
using std::pair;
using std::string;

typedef std::map<string, int> FreqMap;

FreqMap file_line_freq(string filename) {
    FreqMap freq_map;
    std::ifstream instream(filename);

    if (!instream.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }
    else {
        string line;

        while (getline(instream, line)) {
            freq_map[line]++;
        }
        return freq_map;
    }
}

bool pair_less_than(pair<string, int> a, pair<string, int> b) {
    return a.second < b.second;
}

bool pair_greater_than(pair<string, int> a, pair<string, int> b) {
    return a.second > b.second;
}

void print_most_frequent_lines(FreqMap freq_map, int k) {

    int heap_size;

    if (k < freq_map.size()) {
        heap_size = k;
    }
    else {
        heap_size = freq_map.size();
    }

    std::vector<pair<string, int>> min_heap_container;
    min_heap_container.reserve(heap_size);

    auto kth_place_iter = std::next(freq_map.begin(), heap_size);

    std::copy(freq_map.begin(),
              kth_place_iter,
              std::back_inserter(min_heap_container));
              
    std::make_heap(min_heap_container.begin(),
                   min_heap_container.end(),
                   pair_greater_than);

    for (auto iter = kth_place_iter; iter != freq_map.end(); ++iter) {
        if (pair_greater_than(*iter, min_heap_container[0])) {
            std::pop_heap(min_heap_container.begin(),
                          min_heap_container.end());
            min_heap_container.pop_back();

            min_heap_container.push_back(*iter);
            std::push_heap(min_heap_container.begin(),
                           min_heap_container.end());
        }
    }

    std::vector<pair<string, int>>
        max_heap_container = min_heap_container;

    pair<string, int> elt;

    for (int i = 0; i < heap_size; ++i) {
        std::make_heap(max_heap_container.begin(),
                       max_heap_container.end(),
                       pair_less_than);
                     
        elt = max_heap_container[0];
        cout << elt.second << " : " << elt.first << endl;
        
        std::pop_heap(max_heap_container.begin(), max_heap_container.end());
        max_heap_container.pop_back();
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cerr << "Filename is required." << cout;
        exit(1);
    }
    else {
        FreqMap log_line_map = file_line_freq(string(argv[1]));
        int n = 10;

        if (argc == 3) {
            n = atoi(argv[2]);
        }
        print_most_frequent_lines(log_line_map, n);
    }

    return 0;
}
