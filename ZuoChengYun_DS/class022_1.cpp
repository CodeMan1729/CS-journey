#include<iostream>
#include<array>

using namespace std;

static const int MAX = 100000;

static array<int, MAX> arr;
static array<int, MAX> help;

long merge(int l, int m, int r) {
    int i = l;
    int j = m + 1;
    long sum = 0;
    int k = l; // in help array

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            sum += arr[i] * (r - j + 1);
            help[k++] = arr[i++];
        } else {
            help[k++] = arr[j++];
        }
    }

    while (i <= m) {
        help[k++] = arr[i++];
    }
    while (j <= r) {
        help[k++] = arr[j++];
    }

    for (int w = l; w <= r; w++) {
        arr[w] = help[w];
    }

    return sum;
}

long smallSum(int l, int r) {
    if (l == r) {
        return 0;
    }

    int m = (l + r) / 2;
    return smallSum(l, m) + smallSum(m + 1, r) + merge(l, m, r);
}

int main() {
    int num;
    cin >> num;

    for (int i = 0; i < num; ++i) {
        cin >> arr[i];
    }

    cout << smallSum(0, num - 1) << '\n';
}
