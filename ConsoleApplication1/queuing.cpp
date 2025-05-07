#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>   // For timing and random seed
#include <random>   // For random number generation
#include <type_traits> // For std::is_integral and std::is_floating_point

// 使用 std 命名空间 以保持简洁，但在实际项目中请谨慎使用
using namespace std;

// 假设 SortingMachine 类和 printVector 函数与之前定义的一样
// ... (SortingMachine 类定义， printVector 函数定义) ...
// 为了完整性，这里简化列出 SortingMachine 的相关部分

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
            string method_name = getMethodName();
            // cout << "Sorting list using " << method_name << "..." << endl; // 演示函数会打印
            (this->*currentSortMethod)(data); // 调用选定的排序方法
            sortCount++;
            // cout << "Sorting complete." << endl; // 演示函数会打印
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
        case BUBBLE_SORT: currentSortMethod = &SortingMachine<T>::bubbleSortImpl; break;
        case INSERTION_SORT: currentSortMethod = &SortingMachine<T>::insertionSortImpl; break;
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
        if (currentSortMethod == &SortingMachine<T>::bubbleSortImpl) return "Bubble Sort";
        if (currentSortMethod == &SortingMachine<T>::insertionSortImpl) return "Insertion Sort";
        return "Unknown Sort Method";
    }

    void bubbleSortImpl(vector<T>& data) {
        size_t n = data.size();
        // --- 添加对向量大小的检查 ---
        if (n < 2) {
            return; // 如果元素少于 2 个，无需排序，直接返回
        }
        // --------------------------

        for (size_t i = 0; i < n - 1; ++i) {
            // 内层循环的边界 n - i - 1 在 n >= 2 时是正确的
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
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

template<typename T>
void printVector(const vector<T>& data) {
    for (const auto& elem : data) {
        cout << elem << " ";
    }
    cout << endl;
}


// --- 新增的函数：生成随机数据的 Vector ---
template<typename T>
vector<T> generateRandomVector(size_t length, double max_number) {
    vector<T> data;
    if (length == 0) {
        return data; // 返回空 vector
    }

    data.reserve(length); // 预留内存以提高效率

    // 使用当前时间作为种子
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine rng(seed); // 随机数引擎

    // 根据数据类型选择合适的分布
    if constexpr (is_integral_v<T>) { // 如果 T 是整数类型 (int, long, short, char 等)
        // 确保 max_number 是非负的，并且转换为 T 类型
        T max_val = static_cast<T>(max(0.0, max_number));
        // 均匀分布在 [0, max_val] 范围内
        uniform_int_distribution<T> dist(0, max_val);

        for (size_t i = 0; i < length; ++i) {
            data.push_back(dist(rng));
        }

    }
    else if constexpr (is_floating_point_v<T>) { // 如果 T 是浮点类型 (float, double, long double)
        // 确保 max_number 是非负的
        double max_val = max(0.0, max_number);
        // 均匀分布在 [0.0, max_val] 范围内
        uniform_real_distribution<T> dist(0.0, static_cast<T>(max_val));

        for (size_t i = 0; i < length; ++i) {
            data.push_back(dist(rng));
        }

    }
    else {
        // 如果 T 是不支持随机生成的其他类型，可以抛出异常或打印错误
        cerr << "Error: Data type not supported for random generation." << endl;
        // return data; // 返回空 vector 或
        // throw runtime_error("Unsupported data type for random generation");
    }

    return data;
}

// 新增的演示函数，用于封装数据生成、排序和输出过程
template<typename T>
void fullSortingDemo(typename SortingMachine<T>::SortType sortType, size_t length, double max_number, const string& description) {
    cout << "--- " << description << " ---" << endl;

    // 1. 输入 / 数据生成
    cout << "Generating vector with length " << length << " and max value " << max_number << "..." << endl;
    vector<T> data = generateRandomVector<T>(length, max_number); // 调用生成函数

    // 确保数据已生成且不为空（除非 length == 0）
    if (length > 0 && data.empty()) {
        cerr << "Error: Failed to generate data." << endl;
        return;
    }
    if (length == 0) {
        cout << "Generated empty vector." << endl;
    }


    cout << "Original vector: ";
    printVector(data);

    // 2. 初始化 SortingMachine 并调用排序
    SortingMachine<T> sorter(sortType);

    // --- 设置开始时间戳 ---
    auto start_time = chrono::steady_clock::now();

    // --- 调用需要测量时长的代码块 ---
    sorter.sort(data); // 排序是原地进行的，data 会被修改

    // --- 设置结束时间戳 ---
    auto end_time = chrono::steady_clock::now();

    // --- 计算时长并转换为毫秒 ---
    auto duration = end_time - start_time;
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(duration);


    // 3. 输出结果
    cout << "Sorted vector: ";
    printVector(data);
    cout << "Sort count for this demonstration: " << sorter.getSortCount() << endl;
    cout << "Sorting took: " << duration_ms.count() << " milliseconds." << endl;
    cout << "\n"; // 添加空行以便区分不同的演示
}


// --- 示例用法 (main 函数变得更简洁) ---
int main() {
    // 使用 fullSortingDemo 来进行各种演示

    // 演示冒泡排序，生成 10000 个 int 类型随机数，上限 10000
    fullSortingDemo<int>(SortingMachine<int>::BUBBLE_SORT, 10000, 10000, "Bubble Sort (int, 10000 elements, max 10000)");

    cout << "--------------------" << endl;

    // 演示插入排序，生成 10000 个 int 类型随机数，上限 10000
    fullSortingDemo<int>(SortingMachine<int>::INSERTION_SORT, 10000, 10000, "Insertion Sort (int, 10000 elements, max 10000)");

    cout << "--------------------" << endl;

    // 演示插入排序，生成 5000 个 double 类型随机数，上限 1000.5
    fullSortingDemo<double>(SortingMachine<double>::INSERTION_SORT, 5000, 1000.5, "Insertion Sort (double, 5000 elements, max 1000.5)");

    cout << "--------------------" << endl;

    // 演示冒泡排序对少量 float 数据
    fullSortingDemo<float>(SortingMachine<float>::BUBBLE_SORT, 50, 100.0, "Bubble Sort (float, 50 elements, max 100)");

    cout << "--------------------" << endl;

    // 演示生成空 vector
    fullSortingDemo<int>(SortingMachine<int>::BUBBLE_SORT, 0, 100, "Bubble Sort (int, 0 elements)");


    return 0;
}