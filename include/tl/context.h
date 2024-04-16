#pragma once
#include "hash_map.h"

#ifndef TL_ENUMERATE_CONTEXT_MEMBERS
namespace tl {
struct Allocator;
struct TemporaryAllocator;
struct Logger;
}
/*
#define x(Type, name)
*/
#define TL_ENUMERATE_CONTEXT_MEMBERS \
    x(Allocator, allocator) \
    x(TemporaryAllocator, temporary_allocator) \
    x(Logger, logger) \

#endif

int wmain(int argc, wchar_t **argv);

namespace tl {

struct Context {
private:

    struct Shared {
        ContiguousHashMap<u32, Context *> thread_contexts;
        ContiguousHashMap<Span<char>, umm> member_offsets;
        umm size;
        umm alignment;
    };

    Shared *shared;


    Context() = default;

    friend struct FullContext;
    friend int ::wmain(int argc, wchar_t **argv);

    static Context *create();

    void registrate();

public:
    Context(Context const &that) = delete;
    Context(Context &&that) = delete;
    Context &operator=(Context const &that) = delete;
    Context &operator=(Context &&that) = delete;
    
    static thread_local Context *current;

    static Context *create(Context *that) {
        auto result = (Context *)tl_allocate(that->shared->size, that->shared->alignment);
        memcpy(result, that, that->shared->size);
        return result;
    }
    static void free(Context *context) {
        tl_free(context);
    }
    void registrate_dll();

    forceinline void *get_member(Span<char> name) {
        auto found = shared->member_offsets.find(name);
        assert(found, "Context does not contain member \"{}\"", name);
        return (u8 *)this + found->value;
    }

    #define x(Type, name) Type &name() { return *(Type *)get_member(#name##s); }
    TL_ENUMERATE_CONTEXT_MEMBERS
    #undef x
};
}

#ifdef TL_IMPL
#include "thread.h"
namespace tl {

#define NEW(T) &construct(*(T *)tl_allocate(sizeof(T), alignof(T)))

void Context::registrate() {
    auto thread_id = get_current_thread_id();
    assert(!shared->thread_contexts.find(thread_id), "Cannot create a second context for the same thread");
    shared->thread_contexts.get_or_insert(thread_id) = this;
    current = this;
}

void Context::registrate_dll() {
    current = this;
}

struct FullContext : Context {
    #define x(Type, name) Type name;
    TL_ENUMERATE_CONTEXT_MEMBERS
    #undef x
};

Context *Context::create() {
    auto result = NEW(Context);

    result->shared = NEW(Context::Shared);
    result->shared->size = sizeof(FullContext);
    result->shared->alignment = alignof(FullContext);
    result->registrate();

    #define x(Type, name) result->shared->member_offsets.get_or_insert(#name##s) = offsetof(FullContext, name);
    TL_ENUMERATE_CONTEXT_MEMBERS;
    #undef x

    return result;
}
}
#endif
