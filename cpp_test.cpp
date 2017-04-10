#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

int max_profit(int k, vector<int>& prices)
{
    int n = static_cast<int>(prices.size());

    // Easiest case, just get every positive profit
    if (k > n / 2) {
        int profit = 0;
        for (int i = 1; i < n; ++i) {
            profit += max(prices[i] - prices[i-1], 0);
        }
        return profit;
    }

    vector<int> buy(k + 1, numeric_limits<int>::max());
    vector<int> profit(k + 1, 0);
    for(int i = 0; i < n; ++i){
        for(int j = 1; j <= k; ++j) {
            buy[j] = min(buy[j], prices[i] - profit[j-1]);
            profit[j] = max(profit[j], prices[i] - buy[j]);
        }
    }
    return profit[k];
}

int main()
{
    vector<int> prices = {0, 3, 2, 5, 3, 4, 3, 7};
    int p = max_profit(2, prices);
    printf("%d\n", p);

    auto f = [](auto n) { return n*2; };
    // printf("%d\n", f(4));

    return 0;
}
