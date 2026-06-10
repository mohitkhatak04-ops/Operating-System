[org 0x7c00]

mov si, msg1
call print

mov si, msg2
call print

cli
hlt

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print
.done:
    ret

msg1 db 'Jai Hind!', 13, 10, 0
msg2 db 'Jai Bharat!', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
