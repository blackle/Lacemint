#include <syscalls/ForkValidator.h>
#include <SyscallValidationManager.h>

int main(int argc, char** argv) {
	SyscallValidationManager manager( syscallValidatorList {
		new ForkValidator(6969, 0)
	});
}