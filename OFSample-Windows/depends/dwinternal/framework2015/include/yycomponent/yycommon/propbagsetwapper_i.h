#pragma once

#include "dwbase/dwcomex.h"
#include "propbagwrapper_i.h"

/**
  * @brief .
  */
DWDEFINE_INTERFACE(IPropBagSetWrapper):public IUnk
{
/**
   * @brief get count.
   * @return count.
   */
	virtual UINT32 getCount() = 0;
/**
   * @brief get specific item.
   * @param idx item index.
   * @return property.
   */
	virtual IPropBagWrapperPtr getItem(UINT32 idx) = 0;

/**
   * @brief find the specific item by index.
   * @param xid index.
   * @return property.
   */
	virtual IPropBagWrapperPtr findItemById(UINT32 xid) = 0;

/**
   * @brief append item.
   * @param .
   */
	virtual void appendItem(IPropBagWrapperPtrCR spPB, UINT32 xid) = 0;

/**
   * @brief remove all items.
   */
	virtual void removeAllItems() = 0;

/**
   * @brief .
   * @return .
   */
	virtual IPropBagWrapperPtr newPropBag() = 0;
};