cc_library(
    name = "LFThreadPool",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    srcs = ["src/LFThreadPool.cpp"],
)


cc_binary(
    name = "LFThreadPool_benchmark",       # 性能测试目标名称
    srcs = ["internal/benchmark/lockfree_vs_mutex.cpp"],  # 性能测试代码
    deps = [
        ":LFThreadPool",                  # 被测试的库
        "@com_google_benchmark//:benchmark",  # Google Benchmark 库
    ],
)