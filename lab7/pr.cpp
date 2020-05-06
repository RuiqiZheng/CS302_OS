#include <bits/stdc++.h>
using namespace std;
#ifdef Wavator
int DEBUG_COUNT;
auto ROOT = "/home/wavator/Desktop/CS302_OS/lab7/Data";
vector<string> TestFiles = {"sample.in", "1.in", "2.in", "3.in"};
char FILEPATH[50];
#define debug(...) cerr << "DEBUG_" << ++DEBUG_COUNT << ": " << #__VA_ARGS__ << " = " << __VA_ARGS__ << endl
#else
#define debug(...) 99
#endif
typedef vector<int> vi;

class Solver {
public:

    Solver(int K, int A, int N, vi &order) :K(K), A(A), N(N), order(order){}

    void solve() {
        int hit = 0;
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
        auto ans = round(static_cast<double>(hit) / static_cast<double>(N) * 10000) / 100.;
        printf("Hit ratio =%s%.2f%%\n", ans < 10? " 0": " ", ans);
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
                        for (auto it = rev_MIN[iter.first].begin(); it != rev_MIN[iter.first].end() && flag;
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
    //TODO
    int algorithm_clock() const {
        int hit = 0;

        return hit;
    }
    //TODO
    int algorithm_second_chance() const{
        int hit = 0;

        return hit;
    }
};

int main() {
    istream::sync_with_stdio(false);
    cin.tie(nullptr);
    int K, A, N;
    #ifdef Wavator
    int T = TestFiles.size();
    for (int t = 0; t < T; ++t) {
        auto TestFile = TestFiles[t];
        sprintf(FILEPATH, "%s/%s", ROOT, TestFile.c_str());
        ifstream _stream;
        _stream.open(FILEPATH);
        _stream >> K >> A >> N;
        vi order(N);
        for (int i = 0; i < N; ++i) {
            _stream >> order[i];
        }
        puts(TestFile.c_str());
        for (int i = 0; i < 5; ++i) {
            printf("A = %d:\t", i);
            auto solver = new Solver(K, i, N, order);
            solver->solve();
            delete solver;
        }
    }
    #else
    cin >> K >> A >> N;
    vi order(N);
    for (int i = 0; i < N; ++i) {
        cin >> order[i];
    }
    (new Solver(K, A, N, order))->solve();
    #endif
    return 0;
}


