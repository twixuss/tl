#include <tl/string.h>
#include <tl/console.h>
#include <tl/time.h>
#include <tl/random.h>
#define NOMINMAX
#include <WINDOWs.h>

using namespace tl;

bool junk = true;

// get_char_and_advance_utf8:   3658.184 ms  3655.398 ms  3617.65 ms
// get_char_and_advance_utf8_2: 1993.767 ms  1990.212 ms  1996.991 ms

__declspec(noinline) void utf8_test() {
    umm size = 1024*1024*64;
    auto src = (utf8 *)VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    auto dst = (utf32 *)VirtualAlloc(0, size*4, MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    xorshift32 rand;

    src[size-1]=0;
    src[size-2]=0;
    src[size-3]=0;
    src[size-4]=0;
    src[size-5]=0;

    for (int i = 0; i < 2; ++i) {
        {
            switch (i) {
                case 0:
                    print("Generating only ascii ...\n");
                    break;
                case 1:
                    print("Generating different sizes ...\n");
                    break;
            }
            auto c = src;
            while (c < src + size - 5) {
                switch (i) {
                    case 0:
                        write_utf8(&c, U'A');
                        break;
                    case 1:
                        switch ((next(rand) >> 8) % 4) {
                            case 0: write_utf8(&c, U'A'); *c = 0; break;
                            case 1: write_utf8(&c, U'Я'); *c = 0; break;
                            case 2: write_utf8(&c, U'☺'); *c = 0; break;
                            case 3: write_utf8(&c, U'􈶈'); *c = 0; break;
                        }
                        break;
                }
            }
        }

        print("Testing get_char_and_advance_utf8 ... ");
        float avg = 0;
        for (int i = 0; i < 11; ++i) {
            auto timer = create_precise_timer();
            defer { if (i) avg += get_time(timer); };
            auto s = src;
            auto d = dst;
            while (*s) {
                *d++ = get_char_and_advance_utf8(&s).value_unchecked();
            }
        }
        print("{} ms\n", avg/10*1000);

        print("Testing get_char_and_advance_utf8_fast ... ");
        avg = 0;
        for (int i = 0; i < 11; ++i) {
            auto timer = create_precise_timer();
            defer { if (i) avg += get_time(timer); };
            auto s = src;
            auto d = dst;
            while (*s) {
                *d++ = get_char_and_advance_utf8_fast(&s);
            }
        }
        print("{} ms\n", avg/10*1000);
    }
}
