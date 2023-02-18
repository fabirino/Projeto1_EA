#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void writeVector(vector<int> &vec, int n) {
    int num;
    while (n--) {
        cin >> num;
        vec.push_back(num);
    }
}

int main() {
    int num_codes;
    cin >> num_codes;

    while (num_codes--) {
        int N;
        cin >> N;

        vector<int> lb, cb, lt, ct, qb, db;

        writeVector(lb, N); // 1 < x < N
        writeVector(cb, N); // 1 < x < N
        writeVector(lt, N); // 1 < x < N-1
        writeVector(ct, N); // 1 < x < N-1
        writeVector(qb, 4); // 1 < x < (N/2+1)^2
        writeVector(db, 2); // 1 < x < N

    }

    return 0;
}