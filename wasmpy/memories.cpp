#include "memories.hpp"

struct memory_t
{
    void *address;
    int32_t currentPage;
    uint32_t numPages;
    uint32_t maxPages;
    int32_t fd;
#ifdef PLATFORM_WINDOWS
    HANDLE hFile;
    HANDLE hMap;
#endif
};

uint8_t blankPage[65536] = {0};
std::vector<memory_t *> memories = {};

bool createMemory(uint32_t fd, uint32_t minPages, uint32_t maxPages)
{
    // create new memory with minPages amount of initial memory
    memory_t *mem = new memory_t();
    mem->currentPage = -1;
    mem->numPages = minPages; // initialize to minimum required
    mem->maxPages = maxPages;
    mem->fd = fd;
    if (mem->fd == -1)
        return false;

#ifdef PLATFORM_WINDOWS
    mem->hFile = (HANDLE)_get_osfhandle(fd);
    if (mem->hFile == INVALID_HANDLE_VALUE)
        return false;

    mem->hMap = NULL;
#endif
    memories.push_back(mem);
    return true;
}

int32_t growMemory(memory_t *mem, uint32_t n)
{
    // increase amount of memory pages by n
    uint32_t size = mem->numPages;
    if (size + n > mem->maxPages)
        return -1;

    lseek(mem->fd, 0, SEEK_END);
    for (uint32_t i = 0; i < n; i++)
        write(mem->fd, blankPage, 65536);

    mem->numPages = size + n;
    return size;
}

bool loadMemoryPage(memory_t *mem, uint16_t pageNum)
{
    // swap page from memory file into memory mapped region
    unmapMemory(mem);
#ifdef PLATFORM_LINUX
    mem->address = mmap(NULL, 65536, PROT_READ | PROT_WRITE, MAP_SHARED, mem->fd, 65536 * pageNum);
    if (mem->address == MAP_FAILED)
        return false;

#elif PLATFORM_WINDOWS
    mem->hMap = CreateFileMapping(mem->hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (mem->hMap == NULL)
        return false;

    mem->address = MapViewOfFile(mem->hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 65536 * pageNum, 65536);
    if (mem->address == NULL)
        return false;

#endif
    mem->currentPage = pageNum;
    return true;
}

void unmapMemory(memory_t *mem)
{
    // release mapped memory
    if (mem->currentPage != -1)
    {
#ifdef PLATFORM_LINUX
        munmap(mem->address, 65536);
#elif PLATFORM_WINDOWS
        UnmapViewOfFile(mem->address);
#endif
        mem->currentPage = -1;
    }
}

void closeMemory(memory_t *mem)
{
    // unmap memory and release file descriptor
    unmapMemory(mem);
#ifdef PLATFORM_WINDOWS
    CloseHandle(mem->hMap);
    CloseHandle(mem->hFile);
#endif
    close(mem->fd);
}

// function wrappers for native python access
PyObject *createMemory(PyObject *self, PyObject *args)
{
    uint32_t fd, minPages, maxPages;
    if (!PyArg_ParseTuple(args, "LLL", &fd, &minPages, &maxPages))
        return NULL;

    maxPages = maxPages < 65536 ? maxPages : 65536;

    if (!createMemory(fd, minPages, maxPages))
    {
        PyErr_SetString(PyExc_MemoryError, "failed to create memory");
        return NULL;
    }
    return PyLong_FromSize_t(memories.size() - 1);
}

PyObject *loadMemoryPage(PyObject *self, PyObject *args)
{
    uint32_t memoryIndex;
    uint16_t pageNum;
    if (!PyArg_ParseTuple(args, "LH", &memoryIndex, &pageNum))
        return NULL;

    if (!loadMemoryPage(memories.at(memoryIndex), pageNum))
    {
        PyErr_SetString(PyExc_MemoryError, "failed to load memory page");
        return NULL;
    }

    return PyLong_FromVoidPtr(memories.at(memoryIndex)->address);
}
