#include "Cube.h"

std::vector<XMFLOAT3> SVMath::Cube::GetAllCubePoints() const
{
    std::vector<XMFLOAT3> result;

    result.push_back(lbb);
    result.push_back(XMFLOAT3(rtf.x, lbb.y, lbb.z));
    result.push_back(XMFLOAT3(lbb.x, lbb.y, rtf.z));
    result.push_back(XMFLOAT3(rtf.x, lbb.y, rtf.z));
    result.push_back(XMFLOAT3(lbb.x, rtf.y, lbb.z));
    result.push_back(XMFLOAT3(rtf.x, rtf.y, lbb.z));
    result.push_back(XMFLOAT3(lbb.x, rtf.y, rtf.z));
    result.push_back(rtf);

    return result;
}
