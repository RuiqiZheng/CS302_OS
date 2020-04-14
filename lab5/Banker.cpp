#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>

#ifdef DEBUG
int DEBUG_COUNT = 0;
#define dbg(x) std::cerr << x << ' '
#define debug(...) std::cerr << "DBG_" << ++DEBUG_COUNT << ": "#__VA_ARGS__ << "=" << __VA_ARGS__ << std::endl
#else
#define dbg(x) 99
#define debug(...) 99
#endif

void debug_hash(const std::unordered_map<int, std::vector<int> > &h) {
    for (const auto &p: h) {
        debug("PID:");
        debug(p.first);
        debug("--------");
        for (auto x: p.second) {
            dbg(x);
        }
        debug("--------");
    }
}

//new option
void do_new_option(std::unordered_map<int, std::vector<int> > &max_req,
        std::unordered_map<int, std::vector<int> > &allocate, std::unordered_map<int, std::vector<int> > &need,
        const std::vector<int> &request, int pid) {
    for (int request_num: request) {
        max_req[pid].push_back(request_num);
        need[pid].push_back(request_num);
        allocate[pid].push_back(0);
    }
}

//try to do this request
bool try_request(std::vector<int> &max_q,
        std::unordered_map<int, std::vector<int> > &allocate, std::unordered_map<int, std::vector<int> > &need,
        const std::vector<int> &request, int pid) {
    std::unordered_map<int, std::vector<int> > tmpa, tmpn;
    std::set<int> finishpd;
    for (auto &n: need) {
        for (int i = 0; i < (int) request.size(); ++i) {
            tmpa[n.first].push_back(n.first == pid ? allocate[n.first][i] + request[i] : allocate[n.first][i]);
            tmpn[n.first].push_back(n.first == pid ? n.second[i] - request[i] : n.second[i]);
        }
    }
    debug_hash(tmpa);
    debug_hash(tmpn);
    //safe check:
    std::vector<int> currallo(request.size());
    for (int i = 0; i < request.size(); i++) {
        currallo[i] = max_q[i] - request[i];
    }
    for (int i = 0; i < tmpn.size(); i++) {
        bool check = false;
        for (auto item : tmpn) {
            if (finishpd.count(item.first) == 0) {
                bool it = true;
                for (int j = 0; j < request.size(); j++) {
                    it &= (item.second[j] <= currallo[j]);
                    currallo[j] += tmpa[item.first][j];
                }
                if (it) {
                    finishpd.insert(item.first);
                } else {
                    for (int j = 0; j < request.size(); j++) {
                        currallo[j] -= tmpa[item.first][j];
                    }
                }
                check |= it;
            }
        }

    }
    if (finishpd.size() != tmpn.size()) {
        return false;
    }
    //true, merge the attempt
    for (int i = 0; i < request.size(); i++) {
        max_q[i] -= request[i];
    }
    need = tmpn;
    allocate = tmpa;
    return true;
}

int main() {
#ifdef DEBUG
    freopen("/home/wavator/Desktop/CS302_OS/lab5/sample.in", "r", stdin);
    freopen("/home/wavator/Desktop/CS302_OS/lab5/result.out", "w", stdout);
#endif
    int r;
    std::cin >> r;
    std::vector<int> max_q((unsigned) r);
    for (int i = 0; i < r; ++i) {
        std::cin >> max_q[i];
    }
    std::vector<int> resources;
    resources.assign(max_q.begin(), max_q.end());
    std::vector<int> request = std::vector<int>((unsigned) r, 0);
    int pid;
    std::string s;
    std::unordered_map<int, std::vector<int> > max_req, allocation, need;
    while (std::cin >> pid >> s) {
        for (char &c: s) {
            c = (char) tolower(c);
        }
        if (s == "terminate") {
            if (max_req.find(pid) == max_req.end()) {
                puts("NOT OK");
            } else {
                for (int i = 0; i < r; i++) {
                    max_q[i] += allocation[pid][i];
                }
                max_req.erase(pid);
                allocation.erase(pid);
                need.erase(pid);
                puts("OK");
            }
        } else if (s == "new") {
            for (int i = 0; i < r; ++i) {
                std::cin >> request[i];
            }
            bool checknew = (max_req.find(pid) == max_req.end());
            debug(checknew);
            if (checknew) {
                for (int i = 0; i < r && checknew; ++i) {
                    checknew &= (resources[i] >= request[i]);
                    debug(resources[i]);
                    debug(request[i]);
                }
            }
            puts(checknew ? "OK" : "NOT OK");
            if (checknew) {
                do_new_option(max_req, allocation, need, request, pid);
            }
        } else if (s == "request") {
            for (int i = 0; i < r; ++i) {
                std::cin >> request[i];
            }
            bool checkreq = true;
            for (int i = 0; i < r && checkreq; ++i) {
                checkreq &= (max_req[pid][i] >= allocation[pid][i] + request[i]);
                checkreq &= (max_q[i] >= request[i]);
            }
            puts(checkreq && try_request(max_q, allocation, need, request, pid) ? "OK" : "NOT OK");
        }
    }
    return 0;
}
