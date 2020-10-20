#include "Mesh.h"
const Primative::Vertex Primative::Mesh::operator[](int index) const
{
    index = (index < 0) ? static_cast<int>(verts.size()) + index : index;
    return verts.at(index);
}
