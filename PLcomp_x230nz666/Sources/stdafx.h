// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
//#include <tchar.h>

// TODO: 在此处引用程序需要的其他头文件

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <cmath>

#include "Exception.h"

#define PropertyHelper(PropertyName, FieldName, Type) void Set##PropertyName(Type value) { (FieldName) = value; } \
Type & _##PropertyName() { return (FieldName); } \
const Type Get##PropertyName() const { return (FieldName); }

#define ReadonlyPropertyHelper(PropertyName, FieldName, Type) Type Get##PropertyName() { return (FieldName); }