#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, s;
    vector<int> l, r;
    scanf("%d%d%d", &s, &m, &n);
    for (int i = 0, j; i < n; ++i) {
        scanf("%d", &j);
        (j > s ? r : l).push_back(j);
    }
    sort(l.rbegin(), l.rend());
    sort(r.rbegin(), r.rend());
    if (!l.empty()) {
        printf("%d ",s);
    }
    for (auto &x: l) {
        printf("%d ", x);
    }
    for (auto &x: r) {
        printf("%d ", x);
    }
    int ans = l.empty()? 0: s - l.back();
    if (!r.empty()) {
        ans += r[0] - (l.empty()? s: l.back()) + r[0] - r.back();
    }
    printf("\n%d\n", ans);
    return 0;
}
