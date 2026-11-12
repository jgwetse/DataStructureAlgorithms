#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

const int N = 20;

const string NAMES[N] = {
    "BLANTYRE", "MANGOCHI", "NTCHEU", "SALIMA", "ZOMBA",
    "CHIKWAWA", "NSANJE", "THYOLO", "DEDZA", "LILONGWE",
    "NKHOTAKOTA", "NKHATA BAY", "KASUNGU", "MZIMBA", "MCHINJI",
    "MZUZU", "RUMPHI", "KARONGA", "CHITIPA", "NTCHISI"
};

const int MATRIX[N][N] = {
    {  0, 130, 164, 128, 182, 132,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {130,   0,   0,   0, 162,   0,   0,   0, 188,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {164,   0,   0,   0, 170,   0,   0,   0, 189,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {128,   0,   0,   0,   0,   0,   0,   0,   0, 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {182, 162, 170,   0,   0,   0,   0, 125,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {132,   0,   0,   0,   0,   0, 170,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0, 170,   0, 155,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  0,   0,   0,   0, 125,   0, 155,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  0, 188, 189,   0,   0,   0,   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  0,   0,   0, 144,   0,   0,   0,   0, 178,   0,   0,   0, 145,   0, 110,   0,   0,   0,   0, 130},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0, 100},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0, 139,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0, 145,   0,   0,   0, 135,   0,   0,   0,   0,   0, 139},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 135,   0,   0, 111,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 139,   0, 111,   0,   0, 146,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 146,   0, 118,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 118,   0, 163,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 163,   0,   0},
    {  0,   0,   0,   0,   0,   0,   0,   0,   0, 130, 100,   0, 139,   0,   0,   0,   0,   0,   0,   0}
};

void dijkstra(int source, vector<int> &dist, vector<int> &prev) {
    dist.assign(N, INT_MAX);
    prev.assign(N, -1);
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int d = pq.top().first, u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;

        for (int v = 0; v < N; v++) {
            if (MATRIX[u][v] == 0) continue;
            int alt = dist[u] + MATRIX[u][v];
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push(make_pair(alt, v));
            }
        }
    }
}

// Asks for a place until a valid one is entered. Returns -1 on EXIT.
int askPlace(const string &prompt) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) return -1;
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        if (line == "EXIT") return -1;
        for (int i = 0; i < N; i++)
            if (NAMES[i] == line) return i;
        cout << "\"" << line << "\" is not in the data. Try again.\n";
    }
}

int main() {
    while (true) {
        int start = askPlace("Starting point: ");
        if (start == -1) break;
        int dest = askPlace("Destination: ");
        if (dest == -1) break;

        vector<int> dist, prev;
        dijkstra(start, dist, prev);

        if (dist[dest] == INT_MAX) {
            cout << "No route found.\n\n";
            continue;
        }

        vector<int> path;
        for (int at = dest; at != -1; at = prev[at])
            path.insert(path.begin(), at);

        // Route with the weight of each leg
        for (size_t i = 0; i < path.size(); i++) {
            cout << (i ? " -> " : "") << NAMES[path[i]];
            if (i + 1 < path.size())
                cout << " (" << MATRIX[path[i]][path[i + 1]] << ")";
        }
        cout << "\n\n";

        // Weight for every step, then the total
        cout << "Weights:\n";
        for (size_t i = 0; i + 1 < path.size(); i++)
            cout << "  " << NAMES[path[i]] << " -> " << NAMES[path[i + 1]]
                 << ": " << MATRIX[path[i]][path[i + 1]] << "\n";
        cout << "Total weight: " << dist[dest] << "\n\n";
    }
    return 0;
}