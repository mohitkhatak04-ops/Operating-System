EFIINC = /usr/include/efi
LIB = /usr/lib/x86_64-linux-gnu
CFLAGS = -I$(EFIINC) -I$(EFIINC)/x86_64 -I$(EFIINC)/protocol \
         -fno-stack-protector -fpic -fshort-wchar -mno-red-zone \
         -Wall -DEFI_FUNCTION_WRAPPER

all: main.efi

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

main.so: main.o
	ld -nostdlib -znocombreloc -T $(LIB)/elf_x86_64_efi.lds -shared \
	-Bsymbolic -L$(LIB) $(LIB)/crt0-efi-x86_64.o main.o -o main.so \
	-lefi -lgnuefi

main.efi: main.so
	objcopy -j .text -j .data -j .dynamic -j .dynsym \
	-j .rel -j .rela -j .reloc \
	--output-target=pei-x86-64 \
	--subsystem=10 \
	main.so main.efi

clean:
	rm -f main.o main.so main.efi