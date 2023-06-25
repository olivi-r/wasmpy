#include <vector>
#include <cstdint>
#include <cstddef>

typedef std::vector<uint8_t> bytes;

bytes concat(bytes v0, std::vector<bytes> vn);
