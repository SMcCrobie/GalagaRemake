#include "Item.h"


ItemType Item::getItemType() const
{
	return m_itemType;
}

void Item::setItemType(const ItemType type)
{
	m_itemType = type;
}

