#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>

// 性能测试：提交任务到线程池
static void BM_ThreadPoolSubmit(benchmark::State& state) {
    std::vector<int> data(state.range(0)); // 创建指定大小的数组
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (auto& elem : data) {
        elem = dis(gen); // 填充随机数据
    }

    for (auto _ : state) {
        std::vector<int> temp = data; // 每次迭代复制数据
        std::sort(temp.begin(), temp.end()); // 排序
        benchmark::DoNotOptimize(temp); // 防止编译器优化
    }
}

BENCHMARK(BM_ThreadPoolSubmit)->Arg(1000)->Arg(10000)->Arg(100000);

BENCHMARK_MAIN();