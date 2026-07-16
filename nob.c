#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "dep/nob.h"

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    cmd_append(&cmd,
        "g++",
        "-o", "./bin/tests.exe",
        "./src/tests.cpp",
        "./src/tests/",
        "./src/tests/allocation.cpp",
        "./src/tests/array.cpp",
        "./src/tests/c_preprocessor.cpp",
        "./src/tests/c_tokenizer.cpp",
        "./src/tests/common.cpp",
        "./src/tests/compiler.cpp",
        "./src/tests/coroutine.cpp",
        "./src/tests/definitions.cpp",
        "./src/tests/file.cpp",
        "./src/tests/fly_string.cpp",
        "./src/tests/function.cpp",
        "./src/tests/math.cpp",
        "./src/tests/simd.cpp",
        "./src/tests/sorted_list.cpp",
        "./src/tests/stream.cpp",
        "./src/tests/string.cpp",
        "./src/tests/thread_pool.cpp",
        "./src/tests/utf8.cpp",
        "-std=c++23",
        "-I./include",
        "-mavx2", "-mlzcnt",
        "-fmax-errors=1",
        "-Wno-literal-suffix"
    );
    if (!cmd_run(&cmd))
        return 1;

    return 0;
}

