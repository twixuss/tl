#pragma once

#ifndef TL_ENUMERATE_CONTEXT_MEMBERS
#error You must define TL_ENUMERATE_CONTEXT_MEMBERS before including any tl header when using a context. See below for example.
#endif
#ifndef TL_ENUMERATE_MAIN_CONTEXT_MEMBERS
#error You must define TL_ENUMERATE_MAIN_CONTEXT_MEMBERS before including any tl header when using a context. See below for example.
#endif

// Using a context creates cyclic include dependency.
// To get around that the user must define two macros: 
//     TL_ENUMERATE_CONTEXT_MEMBERS      - defines members for all contexts
//     TL_ENUMERATE_MAIN_CONTEXT_MEMBERS - defines members for the main context only (doesn't include TL_ENUMERATE_CONTEXT_MEMBERS)
// 
// Most likely you are going to use yet undeclared types, so you need to forward declare them.
// Forward declaration is enough because only references to those types are needed.

/*
namespace tl {
struct Allocator;
struct TemporaryAllocator;
struct Logger;
struct File;
struct OsLock;
}
// #define x(Type, name)
#define TL_ENUMERATE_CONTEXT_MEMBERS \
    x(tl::Allocator, allocator) \
    x(tl::TemporaryAllocator, temporary_allocator) \
    x(tl::Logger, logger) \

#define TL_ENUMERATE_MAIN_CONTEXT_MEMBERS \
	x(tl::File, default_logger_file) \
	x(tl::OsLock, stdout_mutex) \
*/

#define x(Type, name) \
    Type &current_##name();
TL_ENUMERATE_CONTEXT_MEMBERS
#undef x

#define x(Type, name) \
    Type &global_##name();
TL_ENUMERATE_CONTEXT_MEMBERS
TL_ENUMERATE_MAIN_CONTEXT_MEMBERS
#undef x

namespace tl {
struct Context;

Context *clone_context();
void registrate(Context *context);
void registrate_dll(Context *context);

}
