#pragma once
#include "context.h"
#include "thread.h"
namespace tl {

thread_local Context *Context::current;

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
    #define x(Type, name) Type _##name;
    TL_ENUMERATE_CONTEXT_MEMBERS
    #undef x
};

struct FullMainContext : MainContext {
    #define x(Type, name) Type _##name;
    TL_ENUMERATE_CONTEXT_MEMBERS
    TL_ENUMERATE_MAIN_CONTEXT_MEMBERS
    #undef x
};

MainContext *Context::create() {
    auto result = NEW(FullMainContext);

    result->shared = NEW(Context::Shared);
    result->shared->size = sizeof(FullContext);
    result->shared->alignment = alignof(FullContext);
    result->registrate();
    
    #define stupid_if(condition, code) ([&](auto x) { if constexpr ((condition) && sizeof(x)) { code; } }(1))

    #define x(Type, name) \
        result->shared->members.get_or_insert(#name##s) = { \
            .offset = offsetof(FullMainContext, _##name), \
            .copy_construct = [](void *dst, void *src) { stupid_if(std::is_copy_constructible_v<Type>, new(dst) Type(*(Type *)src)); }, \
        };
    TL_ENUMERATE_CONTEXT_MEMBERS;
    TL_ENUMERATE_MAIN_CONTEXT_MEMBERS;
    #undef x

    return result;
}
}