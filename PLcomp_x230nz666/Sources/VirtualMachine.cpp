#define _CRT_SECURE_NO_WARNINGS
#include "VirtualMachine.h"

#define LOPART(_qw)    ((signed long)(_qw))
#define HIPART(_qw)    ((signed long)(((_qw) >> 32) & 0xffffffff))

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
        case VI_NOP:
            // empty instruction, nothing needed to do.
            break;

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
            unsigned __int64 refWord = (unsigned __int64)((unsigned long)instruction.param_a);
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
                pc = (size_t)instruction.param_b;
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
            switch ((size_t)instruction.param_a)
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

            case 13: //odd function
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

            case 16: // not the top value
                rs[rs.get_top()] = rs.top() != 0 ? 0 : (__int64)-1;
                break;
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

// CRuntimeStack Members

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
