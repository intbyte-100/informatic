import ctypes
import time
import os



LIB_NAME = "lib.so"
LIB_PATH = os.path.join(os.path.dirname(__file__), LIB_NAME)


median_lib = ctypes.CDLL(LIB_PATH)

median_lib.test_median_n_times.argtypes = (
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_int,
    ctypes.c_int,
)
median_lib.test_median_n_times.restype = ctypes.c_double


def c_time(numbers, iterations):
    n = len(numbers)
    if n == 0 or iterations <= 0:
        return 0.0
    c_array = (ctypes.c_double * n)(*numbers)
    return float(median_lib.test_median_n_times(c_array, n, iterations))


def py_median(numbers):
    s = (numbers)
    n = len(s)
    if n == 0:
        return 0.0
    if n % 2 == 0:
        return (s[n // 2 - 1] + s[n // 2]) / 2.0
    return s[n // 2]


def py_time(numbers, iterations):
    if iterations <= 0:
        return 0.0
    start = time.perf_counter()
    
    for _ in range(iterations):
        py_median(numbers)
        
    return time.perf_counter() - start


def main():
    numbers = list(range(1000))
    tests = [1000, 10000, 100000]

    print("-" * 60)
    for count in tests:
        c_t = c_time(numbers, count)
        py_t = py_time(numbers, count)
        print("iterations={:>6}: c function time:  {:.6f} s".format(count, c_t))
        print("iterations={:>6}: python time:      {:.6f} s".format(count, py_t))
        print("-" * 60)


if __name__ == "__main__":
    main()
