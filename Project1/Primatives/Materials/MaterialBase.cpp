#include "MaterialBase.h"
#include "../../Utils/ResourceLoader.h"
#include "Forward.h"


Materials::MaterialBase::MaterialBase()
{
	ResourceLoader::addMaterial(this);
}
