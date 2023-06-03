#ifndef TILT_BENCH_INCLUDE_TILT_PAYLOAD_H_
#define TILT_BENCH_INCLUDE_TILT_PAYLOAD_H_

#include "tilt/builder/tilder.h"
#include "tilt_bench.h"

using namespace tilt;
using namespace tilt::tilder;

class PayloadBench : public Benchmark {
public:
    PayloadBench(dur_t period, int64_t size) : period(period), size(size) {}

private:
    Op query() final
    {
        auto in_sym = _sym("in", tilt::Type(types::INT64, _iter(0, -1)));
        return _Select(in_sym, [](_sym in) {
            return in + _i64(3);
        });
    }

    void init() final
    {
        in_reg = create_reg<int64_t>(size);
        out_reg = create_reg<int64_t>(size);

        SynthData<int64_t> dataset(period, size);
        dataset.fill(&in_reg);
    }

    void execute(intptr_t addr) final
    {
        auto query = (region_t* (*)(ts_t, ts_t, region_t*, region_t*)) addr;
        query(0, period * size, &out_reg, &in_reg);
    }

    void release() final
    {
        release_reg(&in_reg);
        release_reg(&out_reg);
    }

    region_t in_reg;
    region_t out_reg;

    int64_t size;
    dur_t period;
};

class ParallelPayloadBench : public ParallelBenchmark {
public:
    ParallelPayloadBench(int threads, dur_t period, int64_t size)
    {
        for (int i = 0; i < threads; i++) {
            benchs.push_back(new PayloadBench(period, size));
        }
    }
};

#endif  // TILT_BENCH_INCLUDE_TILT_PAYLOAD_H_
