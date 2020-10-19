#include "Mesh.h"
using namespace Primative;
const Vertex Mesh::operator[](int index) const
{
    index = (index < 0) ? static_cast<int>(verts.size()) + index : index;
    return verts.at(index);
}
