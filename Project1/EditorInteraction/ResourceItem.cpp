#include "ResourceItem.h"

EditorInteraction::ResourceItem::ResourceItem() : name(""), type(ResourceTypes::ERROR)
{
}

EditorInteraction::ResourceItem::ResourceItem(String name, const ResourceTypes& type) : ResourceItem()
{
	this->name = name;
	this->type = type;
}

void EditorInteraction::ResourceItem::cleanUp()
{
}

const EditorInteraction::ResourceTypes& EditorInteraction::ResourceItem::getType() const
{
	return type;
}

String EditorInteraction::ResourceItem::getName() const
{
	return name;
}

void EditorInteraction::ResourceItem::setType(const ResourceTypes& type)
{
	this->type = type;
}

void EditorInteraction::ResourceItem::setName(String name)
{
	this->name = name;
}
