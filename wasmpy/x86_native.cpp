#include <Windows.h>
#include "x86/common.h"
#include "x86/numericInst.h"
#include <iostream>

bytes concat(bytes v0, std::vector<bytes> vn)
{
    for (int i = 0; i < vn.size(); i++)
    {
        v0.insert(v0.end(), vn.at(i).begin(), vn.at(i).end());
    }

    return v0;
}

auto buildFunction(bytes code)
{
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    LPVOID buf = VirtualAlloc(nullptr, sysinf.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
    std::memcpy(buf, code.data(), code.size());
    DWORD dummy;
    VirtualProtect(buf, code.size(), PAGE_EXECUTE_READ, &dummy);
    return reinterpret_cast<std::int32_t (*)()>(buf);
}

int main()
{
    bytes code = concat(i32_const(74), {i32_const(-1), i32_add(), {POP_V32A, 0xC3}});
    // bytes code = {PUSH(-1), PUSH(0), PUSH(2), POP_V32A, 0xC3};
    auto func = buildFunction(code);
    std::cout << func() << std::endl;
    VirtualFree((LPVOID)func, 0, MEM_RELEASE);
}
