#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <locale>
#include "stack"
#include "DataType.h"
#include "VM_instructions.h"

#define STACK_INITSIZE 100

// the stack
// because all the data in 

std::wstring output;

// 指令集规格：
// liti 读入一个整数型常量
// litd 读入一个小数型常量

struct CVirtualMachineInstruction
{
    void set(VirtualInstruction vi, int a, int b) { inst = vi; param_a = a; param_b = b; };
    VirtualInstruction inst;
    int param_a;
    int param_b;
};

CVirtualMachineInstruction *code = NULL;

// self-use

#define LOPART(_qw)    ((signed long)(_qw))
#define HIPART(_qw)    ((signed long)(((_qw) >> 32) & 0xffffffff))

class CRuntimeStack
{
public:
    CRuntimeStack();
    __int64 & operator[] (size_t);

    size_t get_top();
    void init_vars(size_t count);
    void push(__int64);
    void pop();
    void return_pop(size_t top_pos);
    void store(size_t pos);

    __int64 top();

protected:
    std::vector<__int64> m_stackData;
};


class CVirutalMachineInterpreter
{
public:
    CVirutalMachineInterpreter() {}
    void interpret(const CVirtualMachineInstruction *instruction);
protected:

    CRuntimeStack rs;
};


//////////////////////////////////////////////
//////////////////////////////////////////////

void CVirutalMachineInterpreter::interpret(const CVirtualMachineInstruction *code)
{
    size_t pc = 0;
    size_t funcBase = 0;
    CVirtualMachineInstruction instruction;
    do
    {
        instruction = code[pc];	/* 读当前指令 */
        pc++;
        switch (instruction.inst)
        {
        case VI_LIT_INTEGER:
            rs.push(instruction.param_b);
            break;

        case VI_LOAD_INTEGER:
            if (instruction.param_a == 0)
                rs.push(rs[instruction.param_b]);
            else if (instruction.param_a == 1)
                rs.push(rs[funcBase + instruction.param_b]);
            else
                exit(EXIT_FAILURE);
            break;

        case VI_LOAD_INTEGER_REF:
            if (instruction.param_a == 1)
            {
                __int64 ref_data = rs[funcBase + instruction.param_b];
                __int32 refBase = LOPART(ref_data);
                __int32 refOffset = HIPART(ref_data);
                rs.push(rs[refBase + refOffset]);
            }
            else
                exit(EXIT_FAILURE);
            break;

        case VI_MAKE_REFERENCE:
        {
            unsigned __int64 refWord = (unsigned __int64)((unsigned long)0);
            refWord |= (unsigned __int64)((unsigned long)instruction.param_b) << 32;
            rs.push(refWord);
            break;
        }

        case VI_STORE_INTEGER:
            if (instruction.param_a == 0)
                rs.store(instruction.param_b);
            else if (instruction.param_a == 1)
                rs.store(funcBase + instruction.param_b);
            else
                exit(EXIT_FAILURE);
            rs.pop();
            break;

        case VI_STORE_INTEGER_REF:
            if (instruction.param_a == 1)
            {
                __int64 ref_data = rs[funcBase + instruction.param_b];
                __int32 refBase = LOPART(ref_data);
                __int32 refOffset = HIPART(ref_data);
                rs.store(refBase + refOffset);
            }
            else
                exit(EXIT_FAILURE);
            rs.pop();
            break;

        case VI_BRANCH:
            rs.push(0);
        case VI_BRANCH_COND:
            if (!rs.top())
                pc = instruction.param_b;
            rs.pop();
            break;

        case VI_INITIALIZE_VAR:
            // int 0, count
            rs.init_vars(instruction.param_b);
            break;

        case VI_BRANCH_LINK: //
        {
            //bl 0, 地址
            // 第一个为保留字段
            size_t newBase = rs.get_top(); // the pos for param_count
            rs.push(pc); // RA
            rs.push(funcBase); // DL
            pc = instruction.param_b;
            funcBase = newBase;
            break;
        }

        case VI_OPERATION:
            switch (instruction.param_a)
            {
            case 0: // opr 0, 0 函数返回
            {
                size_t newTop = funcBase - rs[funcBase];//params
                pc = rs[funcBase + 1];//RA
                funcBase = rs[funcBase + 2];//last base
                rs.return_pop(newTop);
                break;
            }
            case 1:
                rs[rs.get_top()] = -rs.top();
                break;

            case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] + rs.top();
                rs.pop();
                break;

            case 3: /* 次栈顶项减去栈顶项 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] - rs.top();
                rs.pop();
                break;

            case 4:/* 次栈顶项乘以栈顶项 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] * rs.top();
                rs.pop();
                break;

            case 5:/* 次栈顶项除以栈顶项 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] / rs.top();
                rs.pop();
                break;

            case 6:/* 次栈顶项取模栈顶项 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] % rs.top();
                rs.pop();
                break;

            case 7:/* 次栈顶项与栈顶项是否相等 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] == rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 8:/* 次栈顶项与栈顶项是否不相等 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] != rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 9:/* 次栈顶项与栈顶项是否小于 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] < rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 10:/* 次栈顶项与栈顶项是否小于等于 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] <= rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 11:/* 次栈顶项与栈顶项是否大于 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] > rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 12:/* 次栈顶项与栈顶项是否大于等于 */
                rs[rs.get_top() - 1] = rs[rs.get_top() - 1] >= rs.top() ? (__int64)-1 : 0;
                rs.pop();
                break;

            case 13:/* 次栈顶项取模栈顶项 */
                rs[rs.get_top()] = rs.top() % 2 == 0 ? 0 : (__int64)-1;
                break;

            case 14:/* 栈顶值输出 */
                printf("%I64d\n", rs.top());
                rs.pop();
                break;

            case 15:/* 读入一个输入置于栈顶 */
            {
                __int64 val;
                printf("?");
                scanf("%I64d", &val);
                rs.push(val);
                break;
            }

            default:
                // invalid instruction
                exit(EXIT_FAILURE);
            }
            break;
        case VI_HALT:
            goto HALT_EXIT;
        }
    } while (pc != 0);
HALT_EXIT:
    return;
}

int main()
{
    CVirutalMachineInterpreter prog;
    CVirtualMachineInstruction inst[100];
    inst[0].set(VI_BRANCH, 0, 14);
    inst[1].set(VI_INITIALIZE_VAR, 0, 0);
    inst[2].set(VI_LOAD_INTEGER, 1, -2);
    inst[3].set(VI_LIT_INTEGER, 0, 0);
    inst[4].set(VI_OPERATION, 12, 0);
    inst[5].set(VI_BRANCH_COND, 0, 9);
    inst[6].set(VI_LOAD_INTEGER, 1, -2);
    inst[7].set(VI_STORE_INTEGER_REF, 1, -1);
    inst[8].set(VI_BRANCH, 0, 13);
    inst[9].set(VI_LOAD_INTEGER, 1, -2);
    inst[10].set(VI_OPERATION, 1, 0);
    inst[11].set(VI_STORE_INTEGER_REF, 1, -1);
    inst[12].set(VI_BRANCH, 0, 13);
    inst[13].set(VI_OPERATION, 0, 0);
    inst[14].set(VI_INITIALIZE_VAR, 0, 2);
    inst[15].set(VI_OPERATION, 15, 0);
    inst[16].set(VI_STORE_INTEGER, 0, 3);
    inst[17].set(VI_LOAD_INTEGER, 0, 3);
    inst[18].set(VI_MAKE_REFERENCE, 0, 4);
    inst[19].set(VI_LIT_INTEGER, 0, 2);
    inst[20].set(VI_BRANCH_LINK, 0, 1);
    inst[21].set(VI_LOAD_INTEGER, 0, 4);
    inst[22].set(VI_OPERATION, 14, 0);
    inst[23].set(VI_OPERATION, 0, 0);

    prog.interpret(inst);
    return 0;
}

CRuntimeStack::CRuntimeStack()
{
    m_stackData.clear();
    // 0 params, RA for the program, reserved.
    m_stackData.resize(3, 0);
}

__int64 & CRuntimeStack::operator[](size_t pos)
{
    return m_stackData[pos];
}

void CRuntimeStack::push(__int64 value)
{
    m_stackData.push_back(value);
}

void CRuntimeStack::store(size_t pos)
{
    m_stackData[pos] = top();
}

void CRuntimeStack::init_vars(size_t count)
{
    m_stackData.resize(m_stackData.size() + count);
}

size_t CRuntimeStack::get_top()
{
    return m_stackData.size() - 1;
}

__int64 CRuntimeStack::top()
{
    return m_stackData[m_stackData.size() - 1];
}

void CRuntimeStack::pop()
{
    m_stackData.pop_back();
}

void CRuntimeStack::return_pop(size_t top_pos)
{
    m_stackData.resize(top_pos);
}
