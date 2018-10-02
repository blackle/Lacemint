#include <unicorn/unicorn.h>
#include <string>
#include <vector>

class SyscallValidator {
public:
	virtual uint32_t syscallNumber() const = 0;
	virtual uint32_t handleSyscall(uc_engine* uc) const = 0;
};

class ForkValidator : public SyscallValidator {
public:
	ForkValidator(uint32_t childPID, bool isChild);

	virtual uint32_t syscallNumber() const override;
	virtual uint32_t handleSyscall(uc_engine* uc) const override;

private:
	const uint32_t _childPID;
	const bool _isChild;
};

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

typedef std::vector<SyscallValidator*> syscallValidatorList;

class SyscallValidationManager {
public:
	SyscallValidationManager(const std::vector<SyscallValidator*>& validators);

	void handleSyscall(uc_engine* uc);

private:
	const syscallValidatorList _validators;
	syscallValidatorList::const_iterator _currValidator;
};

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

int main(int argc, char** argv) {
	SyscallValidationManager manager( syscallValidatorList {
		new ForkValidator(6969, 0)
	});
}