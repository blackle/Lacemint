#pragma once

#include "SyscallValidator.h"

class ForkValidator : public SyscallValidator {
public:
	ForkValidator(uint32_t childPID, bool isChild);

	virtual uint32_t syscallNumber() const override;
	virtual uint32_t handleSyscall(uc_engine* uc) const override;

private:
	const uint32_t _childPID;
	const bool _isChild;
};
