#pragma once
#include "context_fwd.h"
#include "hash_map.h"

namespace tl {

struct MainContext;

struct Context {
    struct Shared {
        template <class K, class V>
        using HashMap = ContiguousHashMap<K, V, DefaultHashTraits<K>, DefaultAllocator>;
        template <class T>
        using List = List<T, DefaultAllocator>;

        struct Member {
            umm offset;
            void (*copy_construct)(void *dst, void *src);
        };

        MainContext *main_context;
        HashMap<u32, Context *> thread_contexts;
        HashMap<Span<char>, Member> members;
        List<void (*)()> initters;
        umm size;
        umm alignment;
    };

    Shared *shared;


protected:
    Context() = default;


public:
    Context(Context const &that) = delete;
    Context(Context &&that) = delete;
    Context &operator=(Context const &that) = delete;
    Context &operator=(Context &&that) = delete;
    
    static thread_local Context *current;

    static MainContext *create();
    static Context *create(Context *that) {
        auto result = (Context *)tl_allocate(that->shared->size, that->shared->alignment);
        memcpy(result, that, that->shared->size);
        return result;
    }
    static void free(Context *context) {
        tl_free(context);
    }
    void registrate();
    void registrate_dll();

    forceinline void *get_member(Span<char> name) {
        auto found = shared->members.find(name);
        assert(found, "Context does not contain member \"{}\"", name);
        if (this != (Context *)shared->main_context) {
            assert(found->value.offset < shared->size, "Member \"{}\" is only present in the main context", name);
        }
        return (u8 *)this + found->value.offset;
    }

    #define x(Type, name) Type &name() { return *(Type *)get_member(#name##s); }
    TL_ENUMERATE_CONTEXT_MEMBERS
    #undef x
};
struct MainContext : Context {
    #define x(Type, name) Type &name() { return *(Type *)get_member(#name##s); }
    TL_ENUMERATE_MAIN_CONTEXT_MEMBERS
    #undef x
};

inline Context *clone_context() {
    return Context::create(Context::current);
}

inline void registrate(Context *context) {
    return context->registrate();
}

inline void registrate_dll(Context *context) {
    return context->registrate_dll();
}
inline MainContext *main_context() { 
    if (!Context::current || !Context::current->shared->main_context)
        return Context::create();
    return Context::current->shared->main_context; 
}

}


#define x(Type, name) inline Type &current_##name() { return tl::Context::current->name(); }
TL_ENUMERATE_CONTEXT_MEMBERS
#undef x
#define x(Type, name) inline Type &global_##name() { return tl::main_context()->name(); }
TL_ENUMERATE_CONTEXT_MEMBERS
TL_ENUMERATE_MAIN_CONTEXT_MEMBERS
#undef x
