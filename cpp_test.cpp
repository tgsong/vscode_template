#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

namespace {
template<typename Range>
int ssize(const Range& r)
{
    return static_cast<int>(size(r));
}
} // namespace

namespace llist {
struct ListNode {
    int val;
    ListNode* next;
    // unique_ptr<ListNode> next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// template<typename Range>
// unique_ptr<ListNode> make_list(const Range& r)
// {
//     auto dummy = make_unique<ListNode>(0);
//     ListNode* curr = dummy.get();
//     ListNode* prev = curr;
//     for (int v : r) {
//         curr->next = make_unique<ListNode>(v);
//         curr = curr->next.get();
//     }
//
//     return move(prev->next);
// }

void reorderList(ListNode*& curr, ListNode* pre_tail, ListNode* tail)
{
    if (!tail) return;

    reorderList(curr, pre_tail->next, tail->next);

    if (curr == tail || curr == pre_tail) return;

    auto next = curr->next;
    curr->next = tail;
    tail->next = next;
    pre_tail->next = nullptr;
    curr = next;
}

void reorderList(ListNode* head)
{
    if (!head || !head->next || !head->next->next) return;

    auto curr = head;
    auto pre_tail = ListNode(0);
    pre_tail.next = head;
    auto tail = head;
    reorderList(curr, &pre_tail, tail);
}
} // namespace llist

namespace stock {
int max_profit(int k, vector<int>& prices)
{
    int n = static_cast<int>(prices.size());

    // Easiest case, just get every positive profit
    if (k > n / 2) {
        int profit = 0;
        for (int i = 1; i < n; ++i) {
            profit += max(prices[i] - prices[i - 1], 0);
        }
        return profit;
    }

    vector<int> buy(k + 1, numeric_limits<int>::max());
    vector<int> profit(k + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= k; ++j) {
            buy[j] = min(buy[j], prices[i] - profit[j - 1]);
            profit[j] = max(profit[j], prices[i] - buy[j]);
        }
    }
    return profit[k];
}
} // namespace stock

namespace str {
vector<string> continous_substring(string_view s)
{
    vector<string> result;
    if (s.size() <= 1) return result;

    for (int i = 0, j = 1; j < ssize(s); ++j) {
        if (s[j] - s[j - 1] != 1) {
            i = j;
        } else {
            for (int k = j - 1; k >= i; --k) {
                result.emplace_back(s.substr(k, j - k + 1));
            }
        }
    }

    return result;
}
} // namespace str

namespace graph {
template<typename T>
using node_list = unordered_set<T>;

vector<node_list<int>> make_graph(int n, const vector<pair<int, int>>& edges)
{
    vector<node_list<int>> graph(n);
    for (const auto& p : edges) {
        graph[p.first].emplace(p.second);
    }

    return graph;
}

bool topo_sort_impl(const vector<node_list<int>>& graph, int node, vector<int8_t>& curr_path,
                    vector<int8_t>& visited, vector<int>& result)
{
    if (visited[node]) return true;

    curr_path[node] = visited[node] = true;
    for (auto& neighbors : graph[node]) {
        if (curr_path[neighbors] || !topo_sort_impl(graph, neighbors, curr_path, visited, result))
            return false;
    }
    curr_path[node] = false;
    result.push_back(node);

    return true;
}

vector<int> topo_sort(int num_nodes, const vector<pair<int, int>>& edges)
{
    vector<int> result;

    auto graph = make_graph(num_nodes, edges);
    vector<int8_t> curr_path(num_nodes, false);
    vector<int8_t> visited(num_nodes, false);
    for (int i = 0; i < num_nodes; ++i) {
        if (!visited[i] && !topo_sort_impl(graph, i, curr_path, visited, result)) return {};
    }

    return result;
}

vector<int> get_target(int num_nodes, const vector<pair<int, int>>& edges, int target)
{
    vector<int> result;
    auto graph = make_graph(num_nodes, edges);
    vector<int8_t> visited(num_nodes, false);
    vector<int8_t> curr_path(num_nodes, false);
    topo_sort_impl(graph, target, visited, curr_path, result);

    return result;
}
} // namespace graph

namespace math {
template<typename T>
class Matrix {
public:
    template<typename Range>
    Matrix(int rows, int cols, const Range& r)
        : m_data(begin(r), end(r)), m_rows{rows}, m_cols{cols}
    {
    }

    T& operator()(int i, int j) { return m_data[i * m_cols + j]; }
    const T& operator()(int i, int j) const { return m_data[i * m_cols + j]; }

    int size() const { return ssize(m_data); }
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }

    void transpose()
    {
        auto& m = *this;
        for (int i = 0; i < m_rows; ++i) {
            for (int j = i; j < m_cols; ++j) swap(m(i, j), m(j, i));
        }
    }

private:
    vector<T> m_data;
    int m_rows;
    int m_cols;
};

template<typename T>
ostream& operator<<(ostream& os, const Matrix<T>& m)
{
    int rows = m.rows();
    int cols = m.cols();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            os << m(i, j) << " ";
        }
        os << endl;
    }

    return os;
}

bool isPalindrome(int x)
{
    if (x < 0) return false;

    int rev_half = 0;
    int slow = x;
    int fast = x;
    while (fast) {
        rev_half = rev_half * 10 + slow % 10;
        slow /= 10;
        fast /= 100;
    }

    return slow == rev_half || slow == rev_half / 10;
}
} // namespace math

namespace bin_tree {
template<typename Iter>
bool verifyPreorder(Iter first, Iter last, typename Iter::value_type min,
                    typename Iter::value_type max)
{
    if (first == last) return true;

    auto root_val = *first;
    if (root_val > max || root_val < min) return false;

    auto right_sub = find_if(first + 1, last, [root = *first](int n) { return n > root; });

    return verifyPreorder(first + 1, right_sub, min, root_val) &&
           verifyPreorder(right_sub, last, root_val, max);
}

bool verifyPreorder(const vector<int>& preorder)
{
    if (preorder.empty()) return true;

    return verifyPreorder(begin(preorder), end(preorder), numeric_limits<int>::min(),
                          numeric_limits<int>::max());
}
} // namespace bin_tree

namespace lc {
bool is_palindrome(const string& s)
{
    auto first = 0;
    auto last = ssize(s) - 1;
    while (first < last) {
        if (s[first++] != s[last--]) return false;
    }

    return true;
}

vector<vector<int>> palindromePairs(vector<string>& words)
{
    vector<vector<int>> result;

    // hash map of reverse word to index
    unordered_map<string, int> hmap;
    int idx = 0;
    for (auto& w : words) {
        hmap.emplace(string(w.rbegin(), w.rend()), idx++);
    }

    // Deal with empty string case
    auto iter = hmap.find("");
    if (iter != hmap.end()) {
        for (int i = 0; i < ssize(words); ++i) {
            if (is_palindrome(words[i]) && iter->second != i) result.push_back({iter->second, i});
        }
    }

    for (int i = 0; i < ssize(words); ++i) {
        auto& w = words[i];
        for (int j = 0; j < ssize(w); ++j) {
            auto left = w.substr(0, j);
            auto right = w.substr(j, w.size() - j);

            // When j = 0, left is empty and if words[i] is palindrome, this will insert {i,
            // idx_of_empty_str} to results
            auto iter_left = hmap.find(left);
            if (iter_left != hmap.end() && iter_left->second != i && is_palindrome(right)) {
                result.push_back({i, iter_left->second});
            }

            auto iter_right = hmap.find(right);
            if (iter_right != hmap.end() && iter_right->second != i && is_palindrome(left)) {
                result.push_back({iter_right->second, i});
            }
        }
    }

    return result;
}
} // namespace lc

int main()
{
    // Max profit of a stock with at most k transactions
    vector<int> prices = {0, 3, 2, 5, 3, 4, 3, 7};
    int p = stock::max_profit(2, prices);
    printf("%d\n", p);

    // Continuous substring
    string s1 = "BCDCDEFX";
    auto csub = str::continous_substring(s1);
    for (auto& s : csub) cout << s << " ";
    cout << endl;

    // Course schedule
    auto num_courses = 11;
    vector<pair<int, int>> edges = {{1, 0}, {2, 0}, {3, 0}, {4, 1}, {4, 2}, {5, 3},
                                    {7, 4}, {7, 5}, {7, 6}, {9, 8}, {10, 8}};
    auto seq = graph::get_target(num_courses, edges, 7);
    for (auto& n : seq) cout << n << " ";
    cout << endl;

    // Matrix transpose
    vector<int> data(25);
    iota(begin(data), end(data), 1);
    math::Matrix<int> m{5, 5, data};
    m.transpose();
    cout << m << endl;

    // reorder list
    auto n1 = llist::ListNode(1);
    auto n2 = llist::ListNode(2);
    auto n3 = llist::ListNode(3);
    auto n4 = llist::ListNode(4);
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    llist::reorderList(&n1);

    // verify a valid preorder of a bst
    bin_tree::verifyPreorder({1, 2});

    math::isPalindrome(1001);

    vector<string> words = {"abcd", "dcba"};
    auto result = lc::palindromePairs(words);

    return 0;
}
