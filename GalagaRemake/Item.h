#pragma once
#include "GameObject.h"


class Item :
    public GameObject
{
public:
    ItemType getItemType() const;
    void setItemType(ItemType type);

private:
    ItemType m_itemType = ItemType::Repair_Kit;

};
