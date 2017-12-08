#include "stdafx.h"
#include <locale>
#include "stack"
#include "DataType.h"
#include "VM_instructions.h"

#define STACK_INITSIZE 100
#define MAKEINT32(x) *((__int32 *)&(x))
#define MAKEINT64(x) *((__int64 *)&(x))
// the stack
// because all the data in 
std::vector<__int64> rs;
std::wstring output;

template <class T>
T TypeTranslate64(__int64 value)
{
	return *((T *)&value);
}

// 指令集规格：
// liti 读入一个整数型常量
// litd 读入一个小数型常量

struct CSwVirtualMachineInstruction
{
	VirtualInstruction inst;
	int param_a;
	int param_b;
};

CSwVirtualMachineInstruction *code = NULL;

// self-use

#define LOPART(_qw)    ((signed long)(_qw))
#define HIPART(_qw)    ((signed long)(((_qw) >> 32) & 0xffffffff))

void pop(std::vector<__int64> stk, size_t & op)
{
	op -= 2;
}

void push(std::vector<__int64> stk, size_t & op, SymbolDataType type, __int64 value)
{
	stk[++op] = type;
	stk[++op] = value;
}

void initialize()
{
	rs.clear();
	rs.resize(STACK_INITSIZE);
}

//////////////////////////////////////////////
//////////////////////////////////////////////

void interpret()
{
	initialize();
	output += L"Starting SW...";
	size_t pc = 0;
	size_t funcBase = 0;
	size_t top = 2;

	rs[0] = 0;
	rs[1] = 0; //RA for the program, reserved.
	CSwVirtualMachineInstruction instruction;
	do
	{
		instruction = code[pc++];
		switch (instruction.inst)
		{
		case VI_LIT_INTEGER:
			push(rs, top, SDT_INTEGER, instruction.param_a);
			break;
		case VI_LIT_DECIMAL:
			push(rs, top, SDT_DECIMAL, instruction.param_a);
			break;
		case VI_LOAD_INTEGER:
			if (instruction.param_a == 0)
				push(rs, top, SDT_INTEGER, rs[instruction.param_b]);
			else if (instruction.param_a == 1)
				push(rs, top, SDT_INTEGER, rs[funcBase + instruction.param_b]);
			else
				exit(EXIT_FAILURE);
			break;
		case VI_LOAD_DECIMAL:
			if (instruction.param_a == 0)
				push(rs, top, SDT_DECIMAL, rs[instruction.param_b]);
			else if (instruction.param_a == 1)
				push(rs, top, SDT_DECIMAL, rs[funcBase + instruction.param_b]);
			else
				exit(EXIT_FAILURE);
			break;
		case VI_LOAD_INTEGER_REF:
			if (instruction.param_a == 1)
			{
				__int64 ref_data = rs[funcBase + instruction.param_b];
				__int32 refBase = LOPART(ref_data);
				__int32 refOffset = HIPART(ref_data);
				push(rs, top, SDT_INTEGER, rs[refBase + refOffset]);
			}
			else
				exit(EXIT_FAILURE);
			break;
		case VI_LOAD_DECIMAL_REF:
			if (instruction.param_a == 1)
			{
				__int64 ref_data = rs[funcBase + instruction.param_b];
				__int32 refBase = LOPART(ref_data);
				__int32 refOffset = HIPART(ref_data);
				push(rs, top, SDT_DECIMAL, rs[refBase + refOffset]);
			}
			else
				exit(EXIT_FAILURE);
			break;
		case VI_STORE_INTEGER: //
			if (instruction.param_a == 0)
				push(rs, top, SDT_INTEGER, rs[instruction.param_b]);
			else if (instruction.param_a == 1)
				push(rs, top, SDT_INTEGER, rs[funcBase + instruction.param_b]);
			else
				exit(EXIT_FAILURE);
			pop(rs, top);
			break;
		case VI_STORE_DECIMAL: //
			if (instruction.param_a == 0)
				push(rs, top, SDT_DECIMAL, rs[instruction.param_b]);
			else if (instruction.param_a == 1)
				push(rs, top, SDT_DECIMAL, rs[funcBase + instruction.param_b]);
			else
				exit(EXIT_FAILURE);
			pop(rs, top);
			break;
		case VI_STORE_INTEGER_REF: //
			if (instruction.param_a == 1)
			{
				__int64 ref_data = rs[funcBase + instruction.param_b];
				__int32 refBase = LOPART(ref_data);
				__int32 refOffset = HIPART(ref_data);
				rs[refBase + refOffset] = rs[top];
			}
			else
				exit(EXIT_FAILURE);
			pop(rs, top);
			break;
		case VI_STORE_DECIMAL_REF: //
			if (instruction.param_a == 1)
			{
				__int64 ref_data = rs[funcBase + instruction.param_b];
				__int32 refBase = LOPART(ref_data);
				__int32 refOffset = HIPART(ref_data);
				rs[refBase + refOffset] = rs[top];
			}
			else
				exit(EXIT_FAILURE);
			pop(rs, top);
			break;
		case VI_BRANCH:
			push(rs, top, SDT_UNKNOWN, 1);
		case VI_BRANCH_COND:
			if (rs[top])
				pc = instruction.param_a;
			pop(rs, top);
			break;
		case VI_BRANCH_LINK:
			break;
		}
	} while (0);
}

int main()
{
	long long x = -8589934591;
	printf("%d %d", LOPART(x), HIPART(x));
	return 0;
}