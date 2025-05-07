#include <vector>
#include <iostream>
#include <algorithm> // For std::swap
#include <string>
#include <stdexcept>

// 声明使用 std 命名空间
using namespace std;

// 定义 SortingMachine 类（与之前相同）
template<typename T>
class SortingMachine {
public:
    enum SortType {
        BUBBLE_SORT = 1,
        INSERTION_SORT = 2,
        DEFAULT_SORT = BUBBLE_SORT
    };

    SortingMachine(SortType type = DEFAULT_SORT) : sortCount(0) {
        setSortingType(type);
    }

    void sort(vector<T>& data) {
        if (currentSortMethod != nullptr) {
            string method_name = getMethodName(); // 获取方法名以便输出
            cout << "Sorting using " << method_name << "..." << endl;
            (this->*currentSortMethod)(data);
            sortCount++;
            // cout << "Sorting complete. Total sort count for this machine: " << sortCount << endl; // 这里的输出可以移到演示函数中更集中
        }
        else {
            cerr << "Error: No sorting method is selected!" << endl;
        }
    }

    int getSortCount() const {
        return sortCount;
    }

    void setSortingType(SortType type) {
        switch (type) {
        case BUBBLE_SORT:
            currentSortMethod = &SortingMachine<T>::bubbleSortImpl;
            // cout << "Sorting method set to Bubble Sort." << endl; // 这里的输出可以移到演示函数中
            break;
        case INSERTION_SORT:
            currentSortMethod = &SortingMachine<T>::insertionSortImpl;
            // cout << "Sorting method set to Insertion Sort." << endl; // 这里的输出可以移到演示函数中
            break;
        default:
            cerr << "Warning: Unknown sorting type (" << type << "). Defaulting to Bubble Sort." << endl;
            currentSortMethod = &SortingMachine<T>::bubbleSortImpl;
            break;
        }
    }

private:
    using SortingMethodPtr = void (SortingMachine<T>::*)(vector<T>&);

    SortingMethodPtr currentSortMethod;
    int sortCount;

    string getMethodName() const {
        if (currentSortMethod == &SortingMachine<T>::bubbleSortImpl) {
            return "Bubble Sort";
        }
        else if (currentSortMethod == &SortingMachine<T>::insertionSortImpl) {
            return "Insertion Sort";
        }
        return "Unknown Sort Method";
    }

    void bubbleSortImpl(vector<T>& data) {
        size_t n = data.size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
    }

    void insertionSortImpl(vector<T>& data) {
        size_t n = data.size();
        for (size_t i = 1; i < n; ++i) {
            T key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
    }
};

// 辅助函数：打印 vector
template<typename T>
void printVector(const vector<T>& data) {
    for (const auto& elem : data) {
        cout << elem << " ";
    }
    cout << endl;
}

// 新增的演示函数，用于封装排序和输出过程
template<typename T>
void demonstrateSorting(vector<T>& data, typename SortingMachine<T>::SortType sortType, const string& description) {
    cout << "--- " << description << " ---" << endl;

    // 1. 输入 / 准备
    cout << "Original vector: ";
    printVector(data);

    // 2. 初始化 SortingMachine 并调用排序
    // (初始化 SortingMachine 和调用 sort 可以看作是这里的“调用函数”步骤的内部实现)
    SortingMachine<T> sorter(sortType); // 初始化 SortingMachine，指定排序类型
    sorter.sort(data);                  // 调用 sort 方法

    // 3. 输出
    cout << "Sorted vector: ";
    printVector(data);
    cout << "Sort count for this demonstration: " << sorter.getSortCount() << endl;
    cout << "\n"; // 添加空行以便区分不同的演示
}


// --- 示例用法 (main 函数变得更简洁) ---
int main() {
    // 定义需要排序的数据
    vector<int> numbers1 = { 5, 2, 8, 1, 9, 4, 7, 3, 6 };
    vector<int> numbers2 = { 10, 0, 5, -3, 12 };
    vector<double> numbers3 = { 3.14, 1.618, 2.718, 0.577 };

    // 1. & 2. 初始化 & 输入数据 (已在上面定义) + 3. 调用演示函数 & 4. 输出结果
    demonstrateSorting(numbers1, SortingMachine<int>::BUBBLE_SORT, "Bubble Sort Demonstration (int)");
    demonstrateSorting(numbers2, SortingMachine<int>::INSERTION_SORT, "Insertion Sort Demonstration (int)");
    demonstrateSorting(numbers3, SortingMachine<double>::INSERTION_SORT, "Insertion Sort Demonstration (double)"); // 演示对 double 类型排序

    // 注意：每次调用 demonstrateSorting 都会创建一个新的 SortingMachine 对象，
    // 所以每次的 sort count 都是从 0 开始计算的（只计算在该演示函数中调用 sort 的次数）。
    // 如果你想统计总的排序次数，需要在 main 函数中创建一个 SortingMachine 对象，并多次调用其 sort 方法。

    cout << "--- Demonstrating cumulative sort count ---" << endl;
    vector<int> numbers_cumulative = { 100, 50, 75, 25 };
    SortingMachine<int> cumulativeSorter(SortingMachine<int>::BUBBLE_SORT); // 创建一个对象用于多次排序

    cout << "Original vector for cumulative sort: ";
    printVector(numbers_cumulative);
    cumulativeSorter.sort(numbers_cumulative);
    cout << "Sorted vector: ";
    printVector(numbers_cumulative);
    cout << "Cumulative sort count: " << cumulativeSorter.getSortCount() << endl;

    vector<int> another_vector = { 1, 2, 3, 4, 5 };
    cout << "\nOriginal vector for cumulative sort (second call): ";
    printVector(another_vector);
    cumulativeSorter.setSortingType(SortingMachine<int>::INSERTION_SORT); // 改变排序方法
    cumulativeSorter.sort(another_vector);
    cout << "Sorted vector: ";
    printVector(another_vector);
    cout << "Cumulative sort count (after second call): " << cumulativeSorter.getSortCount() << endl; // 累加的次数

    return 0;
}