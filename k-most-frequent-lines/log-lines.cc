#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
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

void print_most_frequent_lines(FreqMap freq_map, int n) {

    std::priority_queue<pair<string, int>,
                        std::vector<pair<string, int>>,
                        std::function<decltype(pair_less_than)>>
        freq_queue(pair_less_than);

    for (FreqMap::iterator iter = freq_map.begin();
         iter != freq_map.end(); ++iter) {
        freq_queue.push(*iter);
    }

    pair<string, int> elt;

    for (int i = 0; i < n && !freq_queue.empty(); ++i, freq_queue.pop()) {
        elt = freq_queue.top();
        cout << elt.second << " : " << elt.first << endl;
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
