#!/bin/python3
import subprocess
import sys


def run_tests(runs=10, threads=32, datalength=10_000_000):
    time = 0
    for _ in range(runs):
        command = f"./main payload {datalength} {threads}"
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        throughput = float(result.stdout.split(",")[3])
        time += throughput
    return time / runs


def setup_test(ctype, ttype, etype):
    command = f"cmake -DCTYPE={ctype} -DTTYPE={ttype} -DETYPE={etype} ."
    subprocess.run(command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run("make", stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def run():
    runs = int(sys.argv[1]) if len(sys.argv) > 1 else 10 
    threads = int(sys.argv[2]) if len(sys.argv) > 2 else 32
    datalength = int(sys.argv[3]) if len(sys.argv) > 3 else 10_000_000

    tests = [
        ("int8_t", "types::INT8", "_i8"),
        ("int16_t", "types::INT16", "_i16"),
        ("int32_t", "types::INT32", "_i32"),
        ("int64_t", "types::INT64", "_i64"),
    ]

    print("throughput[M/s], threads, datatype, datalength")
    for ctype, ttype, etype in tests:
        setup_test(ctype, ttype, etype)
        avg_time = run_tests(runs, threads, datalength)
        print(f"{avg_time}, {threads}, {ctype}, {datalength}")


run()
