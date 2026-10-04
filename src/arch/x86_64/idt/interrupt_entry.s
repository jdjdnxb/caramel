extern x86_64_idt_interrupt_dispatch

; By the time we reached any of the stubs, the CPU has already constructed
; the architectural interrupt frame (made up out of SS, RSP, RFLAGS, CS, RIP 
; and the error code if we have any).
; If we dont receive a real error code from the CPU then the stub will
; supply it so it can match the interrupt frame.
;
; struct interrupt_frame {
;    uint64_t rax;
;    ...
;    uint64_t r15;
;
;    uint64_t vector;
;    uint64_t error_code;
;
;    uint64_t rip;
;    uint64_t cs;
;    uint64_t rflags;
;    uint64_t rsp;
;    uint64_t ss;
; };

; For ISRs with no error code
%macro ISR_STUB_NO_EC 1

global isr_stub_%1

isr_stub_%1:
    push 0          ; Dummy error code
    push %1         ; Interrupt vector

    jmp x86_64_interrupt_common

%endmacro

; For ISRs with error code
%macro ISR_STUB_EC 1

global isr_stub_%1

isr_stub_%1:
    push %1         ; We only push the interrupt vector this time

    jmp x86_64_interrupt_common
%endmacro

x86_64_interrupt_common:
    ; The stack grows downward on x86, so we have to push the registers in the opposite way that
    ; our struct is structured, RAX ending up at [rsp + 0]
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

    ; RSP is the pointer to our struct since its at the top of our pushes
    mov rdi, rsp

    call x86_64_idt_interrupt_dispatch

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    add rsp, 16      ; Clear vector and error code

    iretq

ISR_STUB_NO_EC 0
ISR_STUB_NO_EC 2
ISR_STUB_NO_EC 4
ISR_STUB_NO_EC 6
ISR_STUB_NO_EC 7
ISR_STUB_NO_EC 10

ISR_STUB_EC    8
ISR_STUB_EC    13
ISR_STUB_EC    14
ISR_STUB_EC    21
