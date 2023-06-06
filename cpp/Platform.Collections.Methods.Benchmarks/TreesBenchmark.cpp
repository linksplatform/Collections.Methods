#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <Platform.Collections.Methods.h>
#include "../Platform.Collections.Methods.Tests/Trees.h"

using namespace Platform::Collections::Methods::Tests;

static void BM_RecursionlessSizeBalancedTree(benchmark::State& state) {
    const int _n = state.range(0);

    RecursionlessSizeBalancedTree<10000, std::uint32_t> recursionlessSizeBalancedTree;

    for (auto _ : state) {
        TestExtensions::TestMultipleCreationsAndDeletions(recursionlessSizeBalancedTree,
            [&]() -> std::size_t { return recursionlessSizeBalancedTree.Allocate(); },
            [&](std::uint32_t link) -> auto { recursionlessSizeBalancedTree.Free(link); },
            &recursionlessSizeBalancedTree.Root,
            [&]() -> std::size_t { return recursionlessSizeBalancedTree.Count(); },
            _n);
    }

    state.SetItemsProcessed(state.iterations() * _n);
}

static void BM_SizeBalancedTree(benchmark::State& state) {
    const int _n = state.range(0);

    SizeBalancedTree<10000, std::uint32_t> sizeBalancedTree;

    for (auto _ : state) {
        TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree,
            [&]() -> std::size_t { return sizeBalancedTree.Allocate(); },
            [&](std::uint32_t link) -> auto { sizeBalancedTree.Free(link); },
            &sizeBalancedTree.Root,
            [&]() -> std::size_t { return sizeBalancedTree.Count(); },
            _n);
    }

    state.SetItemsProcessed(state.iterations() * _n);
}

static void BM_SizedAndThreadedAVLBalancedTree(benchmark::State& state) {
    const int _n = state.range(0);

    SizedAndThreadedAVLBalancedTree<10000, std::uint32_t> avlTree;

    for (auto _ : state) {
        TestExtensions::TestMultipleCreationsAndDeletions(avlTree,
            [&]() -> std::size_t { return avlTree.Allocate(); },
            [&](std::uint32_t link) -> auto { avlTree.Free(link); },
            &avlTree.Root,
            [&]() -> std::size_t { return avlTree.Count(); },
            _n);
    }

    state.SetItemsProcessed(state.iterations() * _n);
}

// Define the benchmarks
BENCHMARK(BM_RecursionlessSizeBalancedTree)->Arg(2000);
BENCHMARK(BM_SizeBalancedTree)->Arg(2000);
BENCHMARK(BM_SizedAndThreadedAVLBalancedTree)->Arg(2000);

// Run the benchmarks
BENCHMARK_MAIN();
