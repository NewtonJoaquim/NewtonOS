global keyboard_handler_stub
extern keyboard_handler

keyboard_handler_stub:
    pusha
    call keyboard_handler
    popa
    iretd

global timer_handler_stub
extern timer_handler

timer_handler_stub:
    pusha
    call timer_handler
    popa
    iretd
