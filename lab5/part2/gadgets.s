.intel_syntax noprefix
.code64

.global gadget1
.global gadget2
.global gadget3
.global gadget4
.global gadget5
.global gadget6

// rax <- stack.pop()
.org 0x00
gadget1:
    pop rax
    ret

// rsi <- rsi << 1
.org 0x10
gadget2:
    shl rsi, 1
    ret

// rdi <- rdi ^ rdi
.org 0x20
gadget3:
    xor rdi, rdi
    ret

// rdx <- rax * 7
.org 0x30
gadget4:
    imul rdx, rax, 7
    ret

// rdi <- rdi + 1
.org 0x40
gadget5:
    inc rdi
    ret

// rsi <- rdi
.org 0x50
gadget6:
    mov rsi, rdi
    ret
