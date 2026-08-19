#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "dep/nob.h"

char *temp_parent_name(char const *path) {
    char *r = temp_strdup(path);
    char *a = strrchr(r, '\\');
    char *b = strrchr(r, '/');

    char *c = 0;
    if (a && b) c = max(a, b);
    else if (a) c = a;
    else if (b) c = a;

    if (c) *c = '\0';
    return path;
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    

    #define relative(path) temp_sprintf("%s/%s", nob_get_current_dir_temp(), path)

    Cmd cmd = {0};
    cmd_append(&cmd,
        "g++",
        "-o", "./bin/tests.exe",
        relative("src/tests.cpp"),
        relative("src/tests/allocation.cpp"),
        relative("src/tests/array.cpp"),
        relative("src/tests/c_preprocessor.cpp"),
        relative("src/tests/c_tokenizer.cpp"),
        relative("src/tests/common.cpp"),
        relative("src/tests/compiler.cpp"),
        relative("src/tests/coroutine.cpp"),
        relative("src/tests/definitions.cpp"),
        relative("src/tests/file.cpp"),
        relative("src/tests/fly_string.cpp"),
        relative("src/tests/function.cpp"),
        relative("src/tests/math.cpp"),
        relative("src/tests/sorted_list.cpp"),
        relative("src/tests/stream.cpp"),
        relative("src/tests/string.cpp"),
        relative("src/tests/thread_pool.cpp"),
        relative("src/tests/utf8.cpp"),
        "-include", relative("include/tl/mingw-avx-alignment-workaround.h"),
        "-municode",
        "-ggdb",
        "-std=c++23",
        "-I./include",
        "-mavx2", "-mlzcnt",
        "-fmax-errors=1",
        "-Wno-literal-suffix",
        "-ld3d11",
        "-ld3d12",
        "-ldxgi",
        "-ld3dcompiler",
        "-ldbghelp",
        "-lstdc++",
        "-lgcc_eh",
        "-lwinmm",
        "-ldwmapi",
        "-lgdi32",
        "-lole32",
        "-lshlwapi",
        "-lcomctl32",
        "-luuid",
        "-lsecur32",
        "-lws2_32",
        "-lopengl32"
    );
    if (!cmd_run(&cmd))
        return 1;

    return 0;
}

