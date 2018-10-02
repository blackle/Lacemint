#include "SyscallValidationManager.h"

SyscallValidationManager::SyscallValidationManager(const syscallValidatorList& validators)
	: _validators(validators)
	, _currValidator(validators.begin())
{}

void SyscallValidationManager::handleSyscall(uc_engine* uc) {
	if (_currValidator == _validators.end()) {
		throw "Too many syscalls";
	}

	uint32_t eax;
	uc_reg_read(uc, UC_X86_REG_EAX, &eax);

	auto validator = *_currValidator;

	if (validator->syscallNumber() != eax) {
		throw "Unexpected syscall";
	}

	eax = validator->handleSyscall(uc);
	uc_reg_write(uc, UC_X86_REG_EAX, &eax);

	_currValidator++;
}