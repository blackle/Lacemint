#include <syscalls/ForkValidator.h>
#include <SyscallValidationManager.h>
#include <string>

#define X86_SYSCALL_FORK "\xb8\x02\x00\x00\x00\xcd\x80"
#define X86_SYSCALL_OTHER "\xb8\x01\x00\x00\x00\xcd\x80"
#define ADDRESS 0x1000000

static void hook_interrupt(uc_engine *uc, uint32_t intno, SyscallValidationManager* manager)
{
	if (intno == 0x80) {
		printf("Hooked onto syscall! %p\n", manager);
		manager->handleSyscall(uc);
	}
}

int main(int argc, char** argv) {
	uc_engine *uc;
	uc_err err;
	uc_hook syscallTrace;

	err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
	if (err) {
		printf("Failed on uc_open() with error returned: %u\n", err);
		return -1;
	}

	uc_mem_map(uc, ADDRESS, 2 * 1024 * 1024, UC_PROT_ALL);

	if (uc_mem_write(uc, ADDRESS, X86_SYSCALL_FORK, sizeof(X86_SYSCALL_FORK) - 1)) {
		printf("Failed to write emulation code to memory, quit!\n");
		return -1;
	}

	auto manager = new SyscallValidationManager({
		new ForkValidator(6969, 0)
	});


	printf("Hooking syscall %p\n", manager);
	if (uc_hook_add(uc, &syscallTrace, UC_HOOK_INTR, (void*)hook_interrupt, (void*)manager, 1, 0, 0)) {
		printf("Failed to add syscall hook, quit!\n");
		return -1;
	}

	int32_t eax = 0x0;
	uc_reg_write(uc, UC_X86_REG_EAX, &eax);

	err = uc_emu_start(uc, ADDRESS, ADDRESS + sizeof(X86_SYSCALL_FORK) - 1, 0, 0);
	if (err) {
		printf("Failed on uc_emu_start() with error returned: %u\n", err);
		return -1;
	}

	uc_close(uc);

	return 0;
}