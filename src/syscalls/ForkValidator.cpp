#include "ForkValidator.h"

ForkValidator::ForkValidator(uint32_t childPID, bool isChild)
	: _childPID(childPID)
	, _isChild(isChild)
{}

uint32_t ForkValidator::syscallNumber() const {
	return 2;
}

uint32_t ForkValidator::handleSyscall(uc_engine* uc) const {
	return _isChild ? 0 : _childPID;
}
