#include <bits/stdc++.h>
using namespace std;
int main() {
    //assert(freopen("data/2.in","r",stdin));
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);
    int n, m, s;
    vector<int> l, r;
    cin >> s >> m >> n;
    for (int i = 0, j; i < n; ++i) {
        cin >> j;
        (j > s? r: l).push_back(j);
    }
    sort(l.rbegin(), l.rend());
    sort(r.rbegin(), r.rend());
    if (!l.empty()) {
        cout << s << ' ';
    }
    for (auto &x: l) {
        cout << x << ' ';
    }
    for (auto &x: r) {
        cout << x << ' ';
    }
    int ans = l.empty()? 0: s - l.back();
    if (!r.empty()) {
        ans += r[0] - (l.empty()? s: l.back()) + r[0] - r.back();
    }
    cout << '\n' << ans << endl;
    return 0;
}
