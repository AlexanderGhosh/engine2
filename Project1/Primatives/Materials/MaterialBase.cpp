#include "MaterialBase.h"
#include "../../Utils/ResourceLoader.h"


Materials::MaterialBase::MaterialBase()
{
	ResourceLoader::addMaterial(this);
}
