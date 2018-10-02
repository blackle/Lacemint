#pragma once

#include <syscalls/SyscallValidator.h>

class SyscallValidationManager {
public:
	SyscallValidationManager(const std::vector<SyscallValidator*>& validators);

	void handleSyscall(uc_engine* uc);

private:
	const syscallValidatorList _validators;
	syscallValidatorList::const_iterator _currValidator;
};
