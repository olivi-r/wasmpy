#include "helpers.hpp"

bytes concat(bytes v0, std::vector<bytes> vn)
{
    for (size_t i = 0; i < vn.size(); i++)
    {
        v0.insert(v0.end(), vn.at(i).begin(), vn.at(i).end());
    }

    return v0;
}
