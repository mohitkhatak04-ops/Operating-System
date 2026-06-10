typedef unsigned short CHAR16;
typedef unsigned long long UINTN;
typedef UINTN EFI_STATUS;
typedef void *EFI_HANDLE;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

// ── GOP structures ──────────────────────────────

typedef struct {
    UINT32 RedMask;
    UINT32 GreenMask;
    UINT32 BlueMask;
    UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;

typedef struct {
    UINT32            Version;
    UINT32            HorizontalResolution;
    UINT32            VerticalResolution;
    UINT32            PixelFormat;
    EFI_PIXEL_BITMASK PixelInformation;
    UINT32            PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    UINT32                                MaxMode;
    UINT32                                Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN                                 SizeOfInfo;
    UINT64                                FrameBufferBase;
    UINTN                                 FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
    void                               *QueryMode;
    void                               *SetMode;
    void                               *Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

// ── GUID for GOP ────────────────────────────────

typedef struct {
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} EFI_GUID;

// ── Boot Services (just what we need) ───────────

typedef struct {
    char _pad[192];
    EFI_STATUS (*LocateProtocol)(
        EFI_GUID *protocol,
        void     *registration,
        void     **interface
    );
} EFI_BOOT_SERVICES;

// ── System Table ────────────────────────────────

typedef struct {
    char              _pad[52];
    void             *ConIn;
    void             *ConInHandle;
    void             *ConOut;
    void             *ConOutHandle;
    void             *StdErr;
    void             *StdErrHandle;
    void             *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

// ── Helper: draw one pixel ───────────────────────

void put_pixel(UINT64 fb, UINT32 x, UINT32 y,
               UINT32 width, UINT32 color) {
    UINT32 *framebuffer = (UINT32 *)fb;
    framebuffer[y * width + x] = color;
}

// ── Helper: draw a filled rectangle ─────────────

void draw_rect(UINT64 fb, UINT32 x, UINT32 y,
               UINT32 w, UINT32 h,
               UINT32 width, UINT32 color) {
    for (UINT32 row = y; row < y + h; row++)
        for (UINT32 col = x; col < x + w; col++)
            put_pixel(fb, col, row, width, color);
}

// ── Entry Point ─────────────────────────────────

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *st) {

    // GOP GUID - unique ID so UEFI knows which protocol we want
    EFI_GUID gop_guid = {
        0x9042a9de, 0x23dc, 0x4a38,
        {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
    };

    // get GOP protocol
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    st->BootServices->LocateProtocol(&gop_guid, 0, (void **)&gop);

    // screen info
    UINT32 width  = gop->Mode->Info->HorizontalResolution;
    UINT32 height = gop->Mode->Info->VerticalResolution;
    UINT64 fb     = gop->Mode->FrameBufferBase;

    // fill screen black
    draw_rect(fb, 0, 0, width, height, width, 0x00000000);

    // draw a white box in the middle (login box)
    draw_rect(fb, 300, 200, 600, 400, width, 0x00FFFFFF);

    // draw a blue title bar
    draw_rect(fb, 300, 200, 600, 60, width, 0x000055FF);

    // draw red and green dots (just to show colors work)
    draw_rect(fb, 340, 320, 200, 40, width, 0x00FF4444); // red box
    draw_rect(fb, 340, 380, 200, 40, width, 0x0044FF44); // green box

    while(1);
    return 0;
}