#pragma once

enum VirtualInstruction
{
	// liti <constant>, 0
	VI_LIT_INTEGER,

	// litd <constant>, 0
	VI_LIT_DECIMAL,

	// lodi <level>, <offset>
	VI_LOAD_INTEGER,

	// lodd <level>, <offset>
	VI_LOAD_DECIMAL,

	// lori <level = 1>, <offset>
	VI_LOAD_INTEGER_REF,

	// lord <level = 1>, <offset>
	VI_LOAD_DECIMAL_REF,

	// stoi <level>, <offset>
	VI_STORE_INTEGER,

	// stod <level>, <offset>
	VI_STORE_DECIMAL,

	// stri <level = 1>, <offset>
	VI_STORE_INTEGER_REF,

	// strd <level = 1>, <offset>
	VI_STORE_DECIMAL_REF,

	// b
	VI_BRANCH,

	// bc
	VI_BRANCH_COND,

	// bl
	VI_BRANCH_LINK,
};
