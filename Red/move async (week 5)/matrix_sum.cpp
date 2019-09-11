#include "test_runner.h"
#include <vector>
#include <future>

#include <numeric>
using namespace std;

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // Реализуйте эту функцию
    vector <future<int64_t>> futures;
    int64_t result = 0;
    int value = matrix[0].size();
    for (int k=0; k<4; k++) {
        futures.push_back(
                    async( [&matrix, value, k] {
            int64_t result = 0;
            for( int i=k*value/4; i< (k+1)*value/4; i++) {
                result += accumulate(matrix[i].begin(), matrix[i].end(), 0);
            }
            return result; })
                    ) ;
    }
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
