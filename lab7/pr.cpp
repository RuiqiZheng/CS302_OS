#include <bits/stdc++.h>
using namespace std;
#ifdef Wavator
int DEBUG_COUNT;
static auto ROOT = "/home/wavator/Desktop/CS302_OS/lab7/Data";
static vector<string> TestFiles = {"sample.in", "1.in", "2.in", "3.in"};
char FILEPATH[50];
#define debug(...) cerr << "DEBUG_" << ++DEBUG_COUNT << ": " << #__VA_ARGS__ << " = " << __VA_ARGS__ << endl
#else
#define debug(...) 99
#endif
namespace Quick_in {
    const int LEN = (1 << 21) + 1;
    char ibuf[LEN], *iH, *iT;
    int f, c;
#define gc() (iH==iT?(iT=(iH=ibuf)+fread(ibuf,1,LEN,stdin),(iH==iT?EOF:*iH++)):*iH++)
    template<class T>
    inline void sc(T &x) {
        for (f = 1, c = gc(); c < '0' || c > '9'; c = gc()) if (c == '-') f = -1;
        for (x = 0; c <= '9' && c >= '0'; c = gc()) x = x * 10 + (c & 15);
        x *= f;
    }
}

using Quick_in::sc;

typedef vector<int> vi;

class Solver {
public:

    Solver(int K, int A, int N, vi &order) :K(K), A(A), N(N), order(order){}

    void solve() {
        int hit;
        switch (A) {
            //0-FIFO, 1-LRU, 2-Min, 3-Clock, 4-Second chance
            case 0:
                hit = algorithm_fifo();
                break;
            case 1:
                hit = algorithm_lru();
                break;
            case 2:
                hit = algorithm_min();
                break;
            case 3:
                hit = algorithm_clock();
                break;
            case 4:
                hit = algorithm_second_chance();
                break;
            default:
                fprintf(stderr, "Invalid input variable A: %d\n", A);
                return;
        }
        auto ans = ceil(static_cast<double>(hit) / static_cast<double>(N) * 10000) / 100.;
        if (N == 0) {
            ans = 100;
        }
#ifdef Wavator
        printf("Hit ratio =%s%.2f%%\t", ans < 10? " 0": " ", ans);
#else
        printf("Hit ratio =%s%.2f%%\n", ans < 10? " 0": " ", ans);
#endif
    }
private:
    int K, A, N;
    vi order;
    int algorithm_fifo() const {
        int hit = 0;
        list<int> q;
        unordered_set<int> curr;
        for (const int &i: order) {
            if (curr.count(i)) {
                ++hit;
            } else {
                if (q.size() == K) {
                    curr.erase(q.front());
                    q.pop_front();
                }
                q.push_back(i);
                curr.insert(i);
            }
        }
        return hit;
    }

    int algorithm_lru() const {
        int hit = 0;
        unordered_map<int, list<int>::iterator> last_ite;
        list<int> q;
        for (const int &i: order) {
            if (last_ite.count(i)) {
                ++hit;
                q.erase(last_ite[i]);
            } else {
                if (q.size() == K) {
                    last_ite.erase(q.front());
                    q.pop_front();
                }
            }
            q.push_back(i);
            last_ite[i] = --q.end();
        }
        return hit;
    }

    int algorithm_min() const {
        static const int INF = 0x3f3f3f3f;
        int hit = 0;
        unordered_map<int, list<int>::iterator> last_ite;
        list<int> q;
        unordered_map<int, list<int>> rev_MIN;
        for (int i = 0; i < N; ++i) {
            rev_MIN[order[i]].push_back(i);
        }
        for (int i = 0; i < N; ++i) {
            rev_MIN[order[i]].push_back(INF);
        }
        for (int i = 0; i < N; ++i) {
            if (last_ite.count(order[i])) {
                ++hit;
            } else {
                if (q.size() == K) {
                    int pos_mx = -1, mx = -1;
                    for (auto &iter : last_ite) {
                        bool flag = true;
                        for (list<int>::iterator it = rev_MIN[iter.first].begin(); it != rev_MIN[iter.first].end() && flag;
                             ) {
                            if (*it < i) {
                                rev_MIN[iter.first].pop_front();
                                it = rev_MIN[iter.first].begin();
                                continue;
                            }
                            if (*it > mx) {
                                mx = *it;
                                pos_mx = iter.first;
                            }
                            flag = false;
                        }
                    }
                    //debug(pos_mx);
                    q.erase(last_ite[pos_mx]);
                    last_ite.erase(pos_mx);
                }
                q.push_back(order[i]);
                last_ite[order[i]] = --q.end();
            }
        }
        return hit;
    }

    int algorithm_clock() const {
        int hit = 0;
        vi idx(N + 1, -1), clk(N + 1, 0);
        int pg = 1;
        unordered_set<int> ht;
        for (const int &i: order) {
            if (ht.count(i)) {
                ++hit;
                int id = -1;
                for (int j = 0; j < idx.size() && (~id) == 0; ++j) {
                    if (i == idx[j]) {
                        id = j;
                    }
                }
                assert(~id);
                clk[id] = 1;
            } else {
                for(;;) {
                    if (clk[pg] == 0) {
                        ht.erase(idx[pg]);
                        ht.insert(idx[pg] = i);
                        clk[pg++] = 1;
                        break;
                    }
                    clk[pg++] = 0;
                    while (pg > K) {
                        pg -= K;
                    }
                }
                while (pg > K) {
                    pg -= K;
                }
            }
        }
        return hit;
    }

    int algorithm_second_chance() const{
        int hit = 0;
        int fifo(K >> 1), lru(K - fifo);
        unordered_set<int> set_f;
        list<int> queue_f;
        unordered_map<int, list<int>::iterator> map_l;
        list<int> ls_l;
        for (const int &i: order) {
            unordered_set<int>::iterator set_it = set_f.find(i);
            unordered_map<int, list<int>::iterator>::iterator map_it = map_l.find(i);
            int x = (int(set_it != set_f.end()) << 1) | int(map_it != map_l.end());
            if (x & 1) {
                ls_l.erase(map_it->second);
                queue_f.push_back(map_it->first);
                set_f.insert(map_it->first);
                map_l.erase(map_it);
                set_f.erase(queue_f.front());
                ls_l.push_back(queue_f.front());
                map_l[queue_f.front()] = --ls_l.end();
                queue_f.pop_front();
                ++hit;
            } else if (x >> 1) {
                ++hit;
            } else if (set_f.size() < fifo) {
                set_f.insert(i);
                queue_f.push_back(i);
            } else if (set_f.size() == fifo) {
                if (ls_l.size() < lru) {
                    ls_l.push_back(queue_f.front());
                    map_l.emplace(queue_f.front(), --ls_l.end());
                    set_f.erase(queue_f.front());
                    queue_f.pop_front();
                    queue_f.push_back(i);
                    set_f.insert(i);
                } else if (ls_l.size() == lru){
                    map_l.erase(ls_l.front());
                    ls_l.pop_front();
                    ls_l.push_back(queue_f.front());
                    map_l.emplace(queue_f.front(), --ls_l.end());
                    set_f.erase(queue_f.front());
                    queue_f.pop_front();
                    queue_f.push_back(i);
                    set_f.insert(i);
                }
            }
        }
        return hit;
    }
};

int main() {
    int K, A, N;
    #ifdef Wavator
    istream::sync_with_stdio(false);
    int T = TestFiles.size();
    for (int t = 0; t < T; ++t) {
        auto TestFile = TestFiles[t];
        sprintf(FILEPATH, "%s/%s", ROOT, TestFile.c_str());
        ifstream _stream;
        _stream.open(FILEPATH);
        auto read_st_time = clock();
        _stream >> K >> A >> N;
        vi order(N);
        for (int i = 0; i < N; ++i) {
            _stream >> order[i];
        }
        auto read_time = double(clock() - read_st_time) / CLOCKS_PER_SEC;
        puts(TestFile.c_str());
        for (int i = 0; i < 5; ++i) {
            clock_t st_time = clock();
            printf("A = %d:\t", i);
            auto solver = new Solver(K, i, N, order);
            solver->solve();
            delete solver;
            clock_t run_time = double(clock() - st_time) / CLOCKS_PER_SEC;
            printf("run time: %.3fs\n", (double)run_time + read_time);
        }
    }
    #else
    scanf("%d%d%d", &K, &A, &N);
    vi order(N);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &order[i]);
    }
    (new Solver(K, A, N, order))->solve();
    #endif
    return 0;
}


