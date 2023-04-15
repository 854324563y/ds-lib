#include "skiplist.h"
#include <chrono>
using namespace std;
int main() {
    {
        auto start = std::chrono::high_resolution_clock::now();

        skip_list<double, int>list;
        int64_t v;
        for (int i=0 ; i<100000 ; ++i) {
            list.insert(rand(), rand()%20);
        }
        for (auto it = list.begin(); it != list.end();it++) {
            //cout << it->key << " " << it->value << endl;
            v += it->value;
        }
        cout << "total value : "<<v <<endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Time elapsed: " << elapsed_seconds << "ms\n";
    }

    cout << "==================================" << endl;

    {
        auto start = std::chrono::high_resolution_clock::now();

        skip_list<double, int>list;
        int64_t v;
        for (int i=0 ; i<100000 ; ++i) {
            list.insert(std::move(rand()), std::move(rand()%20));
        }
        for (auto it = list.begin(); it != list.end();it++) {
            //cout << it->key << " " << it->value << endl;
            v += it->value;
        }
        cout << "total value : "<<v <<endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Use move semantic. Time elapsed: " << elapsed_seconds << "ms\n";
    }

    return 0;
}
