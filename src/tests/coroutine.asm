bits 64

global coroutine_call
global coroutine_yield


coroutine_call:
    mov rsp, coroutine's rsp
    ret


coroutine_yield:
    mov rsp, caller's rsp
    ret
