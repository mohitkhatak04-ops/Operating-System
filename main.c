typedef unsigned short CHAR16;
typedef unsigned long long UINTN;
typedef UINTN EFI_STATUS;
typedef void *EFI_HANDLE;

typedef struct _SIMPLE_TEXT_OUTPUT SIMPLE_TEXT_OUTPUT;

struct _SIMPLE_TEXT_OUTPUT {
    void *Reset;
    EFI_STATUS (*OutputString)(SIMPLE_TEXT_OUTPUT *self, CHAR16 *string);
};

typedef struct {
    unsigned char  Signature[8];    // 8 bytes
    unsigned int   Revision;        // 4 bytes
    unsigned int   HeaderSize;      // 4 bytes
    unsigned int   CRC32;           // 4 bytes
    unsigned int   Reserved;        // 4 bytes
    // total header = 24 bytes
    void          *FirmwareVendor;  // 8 bytes
    unsigned int   FirmwareRevision;// 4 bytes
    unsigned int   _pad;            // 4 bytes
    void          *ConsoleInHandle; // 8 bytes
    void          *ConIn;           // 8 bytes
    void          *ConsoleOutHandle;// 8 bytes
    SIMPLE_TEXT_OUTPUT *ConOut;     // 8 bytes
} EFI_SYSTEM_TABLE;

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *st) {
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"Jai Hind!\r\n");
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"Jai Bharat!\r\n");
    while(1);
    return 0;
}
