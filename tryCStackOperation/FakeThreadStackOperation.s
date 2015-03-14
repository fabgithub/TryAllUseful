
section .data
    top db 0x12,0x34,0x56,0x78,
        dq 0x2121212121212121
        dq 0x1111111111111111
        dq 0x2222222222222222
        dq 0x3333333333333333
    stack        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"

section .text

global _FakeThread_GetRSP


global _FakeThread_StartWithRsp
global _FakeThread_YieldWithMainRsp
global _FakeThread_ResumeWithThreadRsp
global _FakeThread_GetItemPtr

_FakeThread_GetRSP:
    mov rax, rsp
    ret

_FakeThread_GetItemPtr:
    push rdx
    mov rdx, rsp
    mov rsp, rdi
    sub rsp, 8
    pop rax
    mov rsp, rdx
    pop rdx
    ret

_FakeThread_StartWithRsp:
    mov [rcx], rsp ; 保存main rsp
    push rdx       ; 保存pItem
    push rbp

    mov rax, rsp
    mov rsp, rdi
    push rax
    push rbp

    call rsi

    pop rbp
    pop rax
    mov rsp, rax

    pop rbp
    pop rax
    ret

_FakeThread_YieldWithMainRsp:
    mov [rsi], rsp
    push rbp

    mov rsp, rdi
    sub rsp, 16
    pop rbp
    pop rax
    ret

; resume 函数
_FakeThread_ResumeWithThreadRsp:
    mov [rsi], rsp
    push rdx
    push rbp

    mov rsp, rdi
    sub rsp, 8
    pop rbp
    mov rax, rcx ; 保存返回值
    ret
