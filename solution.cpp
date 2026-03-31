#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int degree[MAXN];
bool removed[MAXN];
int color[MAXN];
bool inOddCycle[MAXN];
int n, m;

// Check if a connected component is bipartite
bool checkBipartite(int start, vector<int>& component) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    component.push_back(start);

    bool isBipartite = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (removed[v]) continue;

            if (color[v] == -1) {
                color[v] = 1 - color[u];
                q.push(v);
                component.push_back(v);
            } else if (color[v] == color[u]) {
                isBipartite = false;
            }
        }
    }

    return isBipartite;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
        degree[x]++;
        degree[y]++;
    }

    // Remove all leaf nodes (degree 1) iteratively
    // These nodes cannot be in any cycle
    queue<int> q;
    memset(removed, false, sizeof(removed));

    for (int i = 1; i <= n; i++) {
        if (degree[i] <= 1) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (removed[u]) continue;
        removed[u] = true;

        for (int v : graph[u]) {
            if (!removed[v]) {
                degree[v]--;
                if (degree[v] <= 1) {
                    q.push(v);
                }
            }
        }
    }

    // Now check remaining nodes for odd cycles
    memset(color, -1, sizeof(color));
    memset(inOddCycle, false, sizeof(inOddCycle));

    for (int i = 1; i <= n; i++) {
        if (!removed[i] && color[i] == -1) {
            vector<int> component;
            bool isBipartite = checkBipartite(i, component);

            // If not bipartite, the component has odd cycles
            if (!isBipartite) {
                for (int node : component) {
                    inOddCycle[node] = true;
                }
            }
        }
    }

    // Count nodes that cannot be invited
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (!inOddCycle[i]) {
            count++;
        }
    }

    cout << count << endl;

    return 0;
}
