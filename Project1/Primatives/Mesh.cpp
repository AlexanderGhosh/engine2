#include "Mesh.h"
#include "Vertex.h"
const Primative::Vertex Primative::Mesh::operator[](int index) const
{
    index = (index < 0) ? static_cast<int>(verts.size()) + index : index;
    return verts.at(index);
}

Primative::Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned> indi, std::string name) : verts(verts), indices(indi), name(name)
{

}
