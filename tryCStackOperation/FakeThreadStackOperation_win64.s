
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

global FakeThread_GetRSP


global FakeThread_StartWithRsp
global FakeThread_YieldWithMainRsp
global FakeThread_ResumeWithThreadRsp
global FakeThread_GetItemPtr

FakeThread_GetRSP:
    mov rax, rsp
    ret

; FakeThreadItem * FakeThread_GetItemPtr(long long llMainRsp);

FakeThread_GetItemPtr:
    push rdx
    mov rdx, rsp
    mov rsp, rcx ;
    sub rsp, 8
    pop rax
    mov rsp, rdx
    pop rdx
    ret

; FakeThreadItem * FakeThread_StartWithRsp(long long llNewRsp, void (*pfunc) (), FakeThreadItem *pItem, long long *pSaveMainRsp )

FakeThread_StartWithRsp:
    mov [r9], rsp ; 保存main rsp
    push r8       ; 保存pItem
    push rbp

    mov rax, rsp
    mov rsp, rcx ; 第一个参数llNewRsp
    push rax
    push rbp

    call rdx ; 第二个参数

    pop rbp
    pop rax
    mov rsp, rax

    pop rbp
    pop rax
    ret

; FakeThreadResumeParam * FakeThread_YieldWithMainRsp(long long llMainRsp, long long *pThreadRsp);
FakeThread_YieldWithMainRsp:
    mov [rdx], rsp
    push rbp

    mov rsp, rcx
    sub rsp, 16
    pop rbp
    pop rax
    ret

; resume 函数
; void FakeThread_ResumeWithThreadRsp(long long llThreadRsp, long long *pSaveMainRsp, FakeThreadItem *pItem, FakeThreadResumeParam *pResumeParam)
FakeThread_ResumeWithThreadRsp:
    mov [rdx], rsp
    push r8
    push rbp

    mov rsp, rcx
    sub rsp, 8
    pop rbp
    mov rax, r9 ; 保存返回值
    ret
