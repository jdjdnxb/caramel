CC      := x86_64-elf-gcc
AS      := nasm
LD      := x86_64-elf-gcc
QEMU    := qemu-system-x86_64
XORRISO := xorriso
LIMINE  := ./limine/limine

TARGET := kernel.elf
ISO    := caramel.iso

BUILD    := build
ISO_ROOT := $(BUILD)/iso_root

C_SOURCES := $(shell find src boot -type f -name '*.c')
S_SOURCES := $(shell find src boot -type f -name '*.s')

C_OBJECTS := $(C_SOURCES:%.c=$(BUILD)/%.o)
S_OBJECTS := $(S_SOURCES:%.s=$(BUILD)/%.o)

OBJECTS := $(C_OBJECTS) $(S_OBJECTS)

CFLAGS := \
	-std=c23 \
	-ffreestanding \
	-g \
	-O0 \
	-Wall \
	-Wextra \
	-mcmodel=kernel \
	-mno-red-zone \
	-mno-sse \
	-mno-sse2 \
	-mno-mmx \
	-msoft-float \
	-fno-builtin \
	-Iinclude \
	-Ilimine

LDFLAGS := \
	-T linker.ld \
	-nostdlib \
	-g

QEMU_FLAGS := \
	-m 512M \
	-serial stdio

.PHONY: all
all: $(ISO)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf '  CC  %s\n' "$<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	@printf '  AS  %s\n' "$<"
	$(AS) -f elf64 $< -o $@

.PHONY: iso
iso: $(ISO)

$(ISO): $(TARGET) limine.conf
	@rm -rf $(ISO_ROOT)

	@mkdir -p \
		$(ISO_ROOT)/boot \
		$(ISO_ROOT)/EFI/BOOT

	@cp $(TARGET) \
		$(ISO_ROOT)/boot/kernel.elf

	@cp limine.conf \
		$(ISO_ROOT)/limine.conf

	@cp limine/limine-bios-cd.bin \
		$(ISO_ROOT)/limine-bios-cd.bin

	@cp limine/limine-uefi-cd.bin \
		$(ISO_ROOT)/limine-uefi-cd.bin

	@cp limine/limine-bios.sys \
		$(ISO_ROOT)/limine-bios.sys

	@cp limine/BOOTX64.EFI \
		$(ISO_ROOT)/EFI/BOOT/BOOTX64.EFI

	@printf '  ISO %s\n' "$(ISO)"

	$(XORRISO) -as mkisofs \
		-R -r -J \
		-b limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-hfsplus \
		-apm-block-size 2048 \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(ISO_ROOT) \
		-o $(ISO)

	$(LIMINE) bios-install $(ISO)

.PHONY: run
run: $(ISO)
	$(QEMU) \
		$(QEMU_FLAGS) \
		-cdrom $(ISO)

.PHONY: debug
debug: $(ISO)
	$(QEMU) \
		-m 512M \
		-cdrom $(ISO) \
		-serial stdio \
		-S \
		-s \
		-no-reboot

.PHONY: clean
clean:
	rm -rf $(BUILD)
	rm -f $(TARGET) $(ISO)

.PHONY: rebuild
rebuild: clean all
