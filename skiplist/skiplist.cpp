#include<iostream>
#include<string>
#include<set>
#include<ctime>
using namespace std;
template<typename T>
struct Less {
    bool operator () (const T & a , const T & b) const {
        return a < b;
    }
};
template<typename K, typename V,typename Comp = Less<K>>
class skip_list {
private:
    struct skip_list_node {
        int level;
        const K key;
        V value;
        skip_list_node** forward;   //指针数组。forward[i]指向i层的后继
        skip_list_node() :key{ 0 }, value{ 0 }, level{ 0 }, forward{0} {}
        skip_list_node(K k, V v, int l, skip_list_node* nxt = nullptr) :key(k), value(v), level(l) {
            forward = new skip_list_node * [level + 1];
            for (int i = 0; i <= level; ++i) forward[i] = nxt;  //初始化，指向next节点
        }
        ~skip_list_node() { delete[] forward; }
//        ~skip_list_node() {
//            for (int i=0; i<=level; ++i) delete forward[i];
//            delete[] forward;
//        }
    };
    using node = skip_list_node;
    void init() {
        srand((uint32_t)time(NULL));
        level = length = 0;
        head->forward = new node * [MAXL + 1];  //head节点初始化为MAXL层
        for (int i = 0; i <= MAXL; i++)
            head->forward[i] = tail;
    }
    int randomLevel() {
        // rand()&S 小于 PS 的概率为 PS/S=1/4，因此每个节点的层数的期望值为4。
        int lv = 1; while ((rand() & S) < PS) ++lv;
        return MAXL > lv ? lv : MAXL;
    }
    int level;
    int length;
    static const int MAXL = 32;     //跳表的最大高度
    static const int P = 4;         //每个节点维护的后继节点指针数目。为减小内存，通常小于MAXL
    static const int S = 0xFFFF;    //65535
    static const int PS = S / P;    //每个水平区间包含的节点数
    static const int INVALID = INT_MAX;
    node* head, * tail;
    Comp less;
    node* find(const K& key, node** update) {   //update记录每层查找的节点
        node* p = head;
        for (int i = level; i >= 0; i--) {      //每次查找时，从head节点上层至下层查找
            while (p->forward[i] != tail && less(p->forward[i]->key, key)) {
                p = p->forward[i];
            }
            update[i] = p;                      //记录每层查到的节点
        }
        p = p->forward[0];                      //此时 p.key >= key
        return p;
    }
public:
    struct Iter {
        node* p;
        Iter() : p(nullptr) {};
        Iter(node* rhs) : p(rhs) {}
        node* operator ->()const { return (p);}
        node& operator *() const { return *p;}
        bool operator == (const Iter& rhs) { return rhs.p == p;}
        bool operator != (const Iter& rhs) {return !(rhs.p == p);}
        void operator ++() {p = p->forward[0];}
        void operator ++(int) { p = p->forward[0]; }
    };

    node& operator [] (size_t n) {
        if (n>=length) throw std::out_of_range("Index out of range");
        auto it = head->forward[0];
        for (int i=0;i<n;++i) it=it->forward[0];
        return *it;
    }

    skip_list() : head(new node()), tail(new node()), less{Comp()} {
        init();
    }
    skip_list(Comp _less) : head(new node()), tail(new node()),  less{_less} {
        init();
    }
    void insert(const K& key, const V& value) {
        node * update[MAXL + 1];                    //update节点初始化为MAXL层
        node* p = find(key,update);
        if (p->key == key) {
            p->value = value;
            return;
        }
        int lv = randomLevel();
        if (lv > level) {
            lv = ++level;
            update[lv] = head;
        }
        node * newNode = new node(key, value, lv);  //新节点高度初始化为lv层
        for (int i = lv; i >= 0; --i) {
            p = update[i];
            newNode->forward[i] = p->forward[i];
            p->forward[i] = newNode;
        }
        ++length;                                   //节点个数+1
    }

    bool erase(const K& key) {
        node* update[MAXL + 1];
        node* p = find(key, update);
        if (p->key != key)return false;
        for (int i = 0; i <= p->level; ++i) {
            update[i]->forward[i] = p->forward[i];
        }
        delete p;                                                   //删除查找到的节点
        while (level > 0 && head->forward[level] == tail) --level;  //清理多余层
        --length;                                                   //节点个数-1
        return true;
    }
    Iter find(const K&key) {
        node* update[MAXL + 1];
        node* p = find(key, update);
        if (p == tail)return tail;
        if (p->key != key)return tail;
        return Iter(p);
    }
    bool count(const K& key) {
        node* update[MAXL + 1];
        node* p = find(key, update);
        if (p == tail)return false;
        return key == p->key;
    }
    Iter end() {
        return Iter(tail);
    }
    Iter begin() {
        return Iter(head->forward[0]);
    }
};
int main()
{
    {
        //使用lambda
        auto cmp = [](const string& a, const string& b) {return a.length() < b.length(); };
        skip_list < string, int, decltype(cmp)> list(cmp);
        list.insert("aab", 1321);
        list.insert("hello", 54342);
        list.insert("world", 544);
        for (auto it = list.begin(); it != list.end(); it++) {
            cout << it->key << " " << it->value << endl;
        }
    }

    cout << "==================================" << endl;

    {
        //使用仿函数
        struct cmp {
            bool operator()(int a, int b) {
                return a > b;
            }
        };
        skip_list < int, int, cmp> list{};
        for (int i = 1; i <= 10; i++)list.insert(rand()%20, rand());
        for (auto it = list.find(10); it != list.end(); it++) {
            cout << it->key << " " << it->value << endl;
        }
    }

    cout << "==================================" << endl;

    {
        //默认小于号
        skip_list<int, int>list;
        list.insert(1, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        list.insert(5, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        for (auto it = list.begin(); it != list.end();it++) {
            cout << it->key << " " << it->value << endl;
        }

    }

    {
        //可以添加 T && 实现move语义
    }

}