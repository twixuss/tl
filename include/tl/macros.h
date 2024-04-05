

// 
// Count number of variadic arguments
//
// https://stackoverflow.com/a/36015150
//

#ifdef _MSC_VER // Microsoft compilers

#   define TL_GET_ARG_COUNT(...)  TL__EXPAND_ARGS_PRIVATE(TL__ARGS_AUGMENTER(__VA_ARGS__))

#   define TL__ARGS_AUGMENTER(...) unused, __VA_ARGS__
#   define TL__EXPAND(x) x
#   define TL__EXPAND_ARGS_PRIVATE(...) TL__EXPAND(TL__GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#   define TL__GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#else // Non-Microsoft compilers

#   define TL_GET_ARG_COUNT(...) TL__GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define TL__GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#endif

#define TL_WRAP0(X)
#define TL_WRAP1(X,_0) X(_0)
#define TL_WRAP2(X,_0,_1) X(_0)X(_1)
#define TL_WRAP3(X,_0,_1,_2) X(_0)X(_1)X(_2)
#define TL_WRAP4(X,_0,_1,_2,_3) X(_0)X(_1)X(_2)X(_3)
#define TL_WRAP5(X,_0,_1,_2,_3,_4) X(_0)X(_1)X(_2)X(_3)X(_4)
#define TL_WRAP6(X,_0,_1,_2,_3,_4,_5) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)
#define TL_WRAP7(X,_0,_1,_2,_3,_4,_5,_6) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)
#define TL_WRAP8(X,_0,_1,_2,_3,_4,_5,_6,_7) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)
#define TL_WRAP9(X,_0,_1,_2,_3,_4,_5,_6,_7,_8) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)
#define TL_WRAP10(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)
#define TL_WRAP11(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)
#define TL_WRAP12(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)
#define TL_WRAP13(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)
#define TL_WRAP14(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)
#define TL_WRAP15(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)
#define TL_WRAP16(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)
#define TL_WRAP17(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)
#define TL_WRAP18(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)
#define TL_WRAP19(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)
#define TL_WRAP20(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)
#define TL_WRAP21(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)
#define TL_WRAP22(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)
#define TL_WRAP23(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)
#define TL_WRAP24(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)
#define TL_WRAP25(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)
#define TL_WRAP26(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)
#define TL_WRAP27(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)X(_26)
#define TL_WRAP28(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)X(_26)X(_27)
#define TL_WRAP29(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)X(_26)X(_27)X(_28)
#define TL_WRAP30(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)X(_26)X(_27)X(_28)X(_29)
#define TL_WRAP31(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) X(_0)X(_1)X(_2)X(_3)X(_4)X(_5)X(_6)X(_7)X(_8)X(_9)X(_10)X(_11)X(_12)X(_13)X(_14)X(_15)X(_16)X(_17)X(_18)X(_19)X(_20)X(_21)X(_22)X(_23)X(_24)X(_25)X(_26)X(_27)X(_28)X(_29)X(_30)

#define TL_WRAP(X, ...) CONCAT(TL_WRAP,TL_GET_ARG_COUNT(__VA_ARGS__))(X, __VA_ARGS__)

#define TL_PREFIX0(X)
#define TL_PREFIX1(X,_0) X _0
#define TL_PREFIX2(X,_0,_1) X _0 X _1
#define TL_PREFIX3(X,_0,_1,_2) X _0 X _1 X _2
#define TL_PREFIX4(X,_0,_1,_2,_3) X _0 X _1 X _2 X _3
#define TL_PREFIX5(X,_0,_1,_2,_3,_4) X _0 X _1 X _2 X _3 X _4
#define TL_PREFIX6(X,_0,_1,_2,_3,_4,_5) X _0 X _1 X _2 X _3 X _4 X _5
#define TL_PREFIX7(X,_0,_1,_2,_3,_4,_5,_6) X _0 X _1 X _2 X _3 X _4 X _5 X _6
#define TL_PREFIX8(X,_0,_1,_2,_3,_4,_5,_6,_7) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7
#define TL_PREFIX9(X,_0,_1,_2,_3,_4,_5,_6,_7,_8) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8
#define TL_PREFIX10(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9
#define TL_PREFIX11(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10
#define TL_PREFIX12(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11
#define TL_PREFIX13(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12
#define TL_PREFIX14(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13
#define TL_PREFIX15(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14
#define TL_PREFIX16(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15
#define TL_PREFIX17(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16
#define TL_PREFIX18(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17
#define TL_PREFIX19(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18
#define TL_PREFIX20(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19
#define TL_PREFIX21(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20
#define TL_PREFIX22(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21
#define TL_PREFIX23(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22
#define TL_PREFIX24(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23
#define TL_PREFIX25(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24
#define TL_PREFIX26(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25
#define TL_PREFIX27(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25 X _26
#define TL_PREFIX28(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25 X _26 X _27
#define TL_PREFIX29(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25 X _26 X _27 X _28
#define TL_PREFIX30(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25 X _26 X _27 X _28 X _29
#define TL_PREFIX31(X,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) X _0 X _1 X _2 X _3 X _4 X _5 X _6 X _7 X _8 X _9 X _10 X _11 X _12 X _13 X _14 X _15 X _16 X _17 X _18 X _19 X _20 X _21 X _22 X _23 X _24 X _25 X _26 X _27 X _28 X _29 X _30

#define TL_PREFIX(X, ...) CONCAT(TL_PREFIX,TL_GET_ARG_COUNT(__VA_ARGS__))(X, __VA_ARGS__)


#define TL_SKIP1LAST0()
#define TL_SKIP1LAST1(_0)
#define TL_SKIP1LAST2(_0,_1) _0
#define TL_SKIP1LAST3(_0,_1,_2) _0,_1
#define TL_SKIP1LAST4(_0,_1,_2,_3) _0,_1,_2
#define TL_SKIP1LAST5(_0,_1,_2,_3,_4) _0,_1,_2,_3
#define TL_SKIP1LAST6(_0,_1,_2,_3,_4,_5) _0,_1,_2,_3,_4
#define TL_SKIP1LAST7(_0,_1,_2,_3,_4,_5,_6) _0,_1,_2,_3,_4,_5
#define TL_SKIP1LAST8(_0,_1,_2,_3,_4,_5,_6,_7) _0,_1,_2,_3,_4,_5,_6
#define TL_SKIP1LAST9(_0,_1,_2,_3,_4,_5,_6,_7,_8) _0,_1,_2,_3,_4,_5,_6,_7
#define TL_SKIP1LAST10(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9) _0,_1,_2,_3,_4,_5,_6,_7,_8
#define TL_SKIP1LAST11(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9
#define TL_SKIP1LAST12(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10
#define TL_SKIP1LAST13(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11
#define TL_SKIP1LAST14(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12
#define TL_SKIP1LAST15(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13
#define TL_SKIP1LAST16(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14
#define TL_SKIP1LAST17(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15
#define TL_SKIP1LAST18(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16
#define TL_SKIP1LAST19(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17
#define TL_SKIP1LAST20(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18
#define TL_SKIP1LAST21(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19
#define TL_SKIP1LAST22(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20
#define TL_SKIP1LAST23(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21
#define TL_SKIP1LAST24(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22
#define TL_SKIP1LAST25(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23
#define TL_SKIP1LAST26(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24
#define TL_SKIP1LAST27(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25
#define TL_SKIP1LAST28(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26
#define TL_SKIP1LAST29(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27
#define TL_SKIP1LAST30(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28
#define TL_SKIP1LAST31(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29

#define TL_SKIP1LAST(...) CONCAT(TL_SKIP1LAST,TL_GET_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define TL_REPEAT0(X)
#define TL_REPEAT1(X) X
#define TL_REPEAT2(X) X X
#define TL_REPEAT3(X) X X X
#define TL_REPEAT4(X) X X X X
#define TL_REPEAT5(X) X X X X X
#define TL_REPEAT6(X) X X X X X X
#define TL_REPEAT7(X) X X X X X X X
#define TL_REPEAT8(X) X X X X X X X X
#define TL_REPEAT9(X) X X X X X X X X X
#define TL_REPEAT10(X) X X X X X X X X X X
#define TL_REPEAT11(X) X X X X X X X X X X X
#define TL_REPEAT12(X) X X X X X X X X X X X X
#define TL_REPEAT13(X) X X X X X X X X X X X X X
#define TL_REPEAT14(X) X X X X X X X X X X X X X X
#define TL_REPEAT15(X) X X X X X X X X X X X X X X X
#define TL_REPEAT16(X) X X X X X X X X X X X X X X X X
#define TL_REPEAT17(X) X X X X X X X X X X X X X X X X X
#define TL_REPEAT18(X) X X X X X X X X X X X X X X X X X X
#define TL_REPEAT19(X) X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT20(X) X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT21(X) X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT22(X) X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT23(X) X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT24(X) X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT25(X) X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT26(X) X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT27(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT28(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT29(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT30(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT31(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT32(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT33(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT34(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT35(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT36(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT37(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT38(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT39(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT40(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT41(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT42(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT43(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT44(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT45(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT46(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT47(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT48(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT49(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT50(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT51(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT52(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT53(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT54(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT55(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT56(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT57(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT58(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT59(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT60(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT61(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT62(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT63(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
#define TL_REPEAT64(X) X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X

#define TL_REPEAT(N, X) TL_REPEAT##N(X)

#define TL_IREPEAT0(X)
#define TL_IREPEAT1(X) X(0)
#define TL_IREPEAT2(X) X(0) X(1)
#define TL_IREPEAT3(X) X(0) X(1) X(2)
#define TL_IREPEAT4(X) X(0) X(1) X(2) X(3)
#define TL_IREPEAT5(X) X(0) X(1) X(2) X(3) X(4)
#define TL_IREPEAT6(X) X(0) X(1) X(2) X(3) X(4) X(5)
#define TL_IREPEAT7(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6)
#define TL_IREPEAT8(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7)
#define TL_IREPEAT9(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)
#define TL_IREPEAT10(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9)
#define TL_IREPEAT11(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10)
#define TL_IREPEAT12(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11)
#define TL_IREPEAT13(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12)
#define TL_IREPEAT14(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13)
#define TL_IREPEAT15(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14)
#define TL_IREPEAT16(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15)
#define TL_IREPEAT17(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16)
#define TL_IREPEAT18(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17)
#define TL_IREPEAT19(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18)
#define TL_IREPEAT20(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19)
#define TL_IREPEAT21(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20)
#define TL_IREPEAT22(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21)
#define TL_IREPEAT23(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22)
#define TL_IREPEAT24(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23)
#define TL_IREPEAT25(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24)
#define TL_IREPEAT26(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25)
#define TL_IREPEAT27(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26)
#define TL_IREPEAT28(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27)
#define TL_IREPEAT29(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28)
#define TL_IREPEAT30(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29)
#define TL_IREPEAT31(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30)
#define TL_IREPEAT32(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31)
#define TL_IREPEAT33(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32)
#define TL_IREPEAT34(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33)
#define TL_IREPEAT35(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34)
#define TL_IREPEAT36(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35)
#define TL_IREPEAT37(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36)
#define TL_IREPEAT38(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37)
#define TL_IREPEAT39(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38)
#define TL_IREPEAT40(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39)
#define TL_IREPEAT41(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40)
#define TL_IREPEAT42(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41)
#define TL_IREPEAT43(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42)
#define TL_IREPEAT44(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43)
#define TL_IREPEAT45(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44)
#define TL_IREPEAT46(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45)
#define TL_IREPEAT47(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46)
#define TL_IREPEAT48(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47)
#define TL_IREPEAT49(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48)
#define TL_IREPEAT50(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49)
#define TL_IREPEAT51(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50)
#define TL_IREPEAT52(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51)
#define TL_IREPEAT53(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52)
#define TL_IREPEAT54(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53)
#define TL_IREPEAT55(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54)
#define TL_IREPEAT56(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55)
#define TL_IREPEAT57(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56)
#define TL_IREPEAT58(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57)
#define TL_IREPEAT59(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58)
#define TL_IREPEAT60(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59)
#define TL_IREPEAT61(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59) X(60)
#define TL_IREPEAT62(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59) X(60) X(61)
#define TL_IREPEAT63(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59) X(60) X(61) X(62)
#define TL_IREPEAT64(X) X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59) X(60) X(61) X(62) X(63)

#define TL_IREPEAT(N, X) TL_IREPEAT##N(X)
