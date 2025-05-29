// https://www.nowcoder.com/practice/1ae8d0b6bb4e4bcdbf64ec491f63fc37

#include <iostream>
#include <queue>
#include <algorithm>
#include <array>

using namespace std;

static const int MAXN = 10000;
static array<array<int, 2>, MAXN> lines;

struct cmp_pq
{
    bool operator()(int a, int b)
    {
        return a > b;
    }
};
    
priority_queue<int, vector<int>, cmp_pq> pq; // 建立小根堆（存储后端值）

int compute(int n); // 返回最大重合线段数

int main() {
    // 输入
    int n; // n
    cin >> n;
    for (int i = 0; i < n; i++) 
    {
        cin >> lines[i][0] >> lines[i][1];
    }

    auto cmp = [](const array<int, 2> a, const array<int, 2> b) {
        return a[0] < b[0]; // ???????????????????????????????????????????
    };

    // 左端从大到小排序
    sort(lines.begin(), lines.begin() + n, cmp);

    int ans = compute(n);

    cout << ans << '\n';
    return 0;
}

int compute(int n) 
{
    int numbers = 0;
    // 遍历，判断是否弹出（用top()（右端）和下一个的左端比较）
    for (int i = 0; i < n; i++) 
    {
        while (!pq.empty() && pq.top() <= lines[i][0]) 
        {
            pq.pop();
        }

        // 加入下一个的右端到堆里面
        pq.push(lines[i][1]);

        // 使用max每次找出最大的
        numbers = max(numbers, (int) pq.size());
    }

    return numbers;
}
