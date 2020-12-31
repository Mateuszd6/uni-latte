//
// TODO: Remove this crap!
//
#if 0
#include <bits/stdc++.h>
using namespace std;

int t, n, m, a;

// Stack to store the
// visited vertices in
// the Topological Sort
stack<int> s;

// Store Topological Order
vector<int> tsort;

// Adjacency list to store edges
vector<int> adj[int(1e5) + 1];

// To ensure visited vertex
vector<int> visited(int(1e5) + 1);

// Function to perform DFS
void dfs(int u)
{
    // Set the vertex as visited
    visited[u] = 1;

    for (auto it : adj[u]) {

        // Visit connected vertices
        if (visited[it] == 0)
            dfs(it);
    }

    // Push into the stack on
    // complete visit of vertex
    s.push(u);
}

// Function to check and return
// if a cycle exists or not
bool check_cycle()
{
    // Stores the position of
    // vertex in topological order
    unordered_map<int, int> pos;
    int ind = 0;

    // Pop all elements from stack
    while (!s.empty()) {
        pos[s.top()] = ind;

        // Push element to get
        // Topological Order
        tsort.push_back(s.top());

        ind += 1;

        // Pop from the stack
        s.pop();
    }

    for (int i = 0; i < n; i++) {
        for (auto it : adj[i]) {

            // If parent vertex
            // does not appear first
            if (pos[i] > pos[it]) {

                // Cycle exists
                return true;
            }
        }
    }

    // Return false if cycle
    // does not exist
    return false;
}

// Function to add edges
// from u to v
void addEdge(int u, int v)
{
    adj[u].push_back(v);
}
#endif

static void
tsort_dfs(int vert, i32* exts, b8* visited, i32* order)
{
    printf("Visiting %d\n", vert);
    visited[vert] = 1;
    if (exts[vert] != -1 && !visited[exts[vert]])
        tsort_dfs(exts[vert], exts, visited, order);

    array_push(order, vert);
}

static i32*
sort_classes(i32* exts, mm n_verts)
{
    // exts[i] == -1 means a func does not exstends any type, otherwise it's a type id
    b8* visited = calloc(n_verts, sizeof(b8));
    i32* pos = calloc(n_verts, sizeof(i32));
    i32* order = 0;
    array_reserve(order, n_verts);

    for (mm i = 0; i < n_verts; i++) {
        if (visited[i] == 0) {
            tsort_dfs((i32)i, exts, visited, order);
        }
    }

    for (mm i = 0, n_order = array_size(order); i < n_order; ++i)
        pos[order[i]] = (i32)i;

    printf("Final order: [ ");
    for (mm i = 0, n_order = array_size(order); i < n_order; ++i)
        printf("%d ", order[i]);
    printf("]\n");

    for (mm i = 0; i < n_verts; ++i)
    {
        if (exts[i] != -1 && pos[i] < pos[exts[i]])
        {
            d_type t_child = g_types[i];
            d_type t_parent = g_types[exts[i]];

            error(t_child.lnum, "Inheritance graph cycle. \"%s\" inherits after \"%s\" creating a cycle",
                  t_child.name, t_parent.name);
            note(t_parent.lnum, "\"%s\" declared here", t_parent.name);
            break;
        }
    }

    free(pos);
    free(visited);
    return order;
}
