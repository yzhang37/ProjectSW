#pragma once
#include "stdafx.h"

class CSWStructure
{
private:
    struct StructData
    {
        StructData(size_t tb, size_t cb) :tBase(tb), cBase(cb), cSize(0) {}
        size_t tBase;
        size_t cBase;
        size_t cSize;
    };
    // 1.记录当前的 level
    // 2.当前的 bl
    //

public:
    CSWStructure();
    // allocate more space.
    void AppendSpace(size_t size);
    // in the current, in the future when the program
    // want to fill back the jump position, call GetCurCodeBase function.
    // This maintain the current base position.
    size_t GetCurCodeBase();
    // get the base position in the table.
    size_t GetCurTableBase();
    // get the space
    size_t GetCurVariableSpace();

    //
    size_t GetCurLevel();

    // add a new level of the program structure
    void push(size_t table_base, size_t code_base);
    void pop();
protected:
    std::vector<StructData> m_structData;
};