#pragma once

#include <unicorn/unicorn.h>
#include <vector>

class SyscallValidator {
public:
	virtual uint32_t syscallNumber() const = 0;
	virtual uint32_t handleSyscall(uc_engine* uc) const = 0;
};

typedef std::vector<SyscallValidator*> syscallValidatorList;
