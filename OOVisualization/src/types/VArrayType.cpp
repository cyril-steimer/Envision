/***********************************************************************************************************************
 * VArrayType.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "types/VArrayType.h"

#include "OOModel/headers/types/ArrayType.h"

#include "VisualizationBase/headers/layouts/SequentialLayout.h"
#include "VisualizationBase/headers/items/Symbol.h"

using namespace Visualization;
using namespace OOModel;

namespace OOVisualization {

ITEM_COMMON_DEFINITIONS(VArrayType)

VArrayType::VArrayType(Item* parent, ArrayType* node, const VArrayTypeStyle* style) :
	ModelItem(parent, node, style),
	container_( new SequentialLayout(this, &style->container()) ),
	symbol_( new Symbol(NULL, &style->symbol())),
	type_(NULL)
{
	container_->append(symbol_);
}

VArrayType::~VArrayType()
{
	SAFE_DELETE_ITEM(container_);

	// These were automatically deleted by container's destructor
	symbol_ = NULL;
	type_ = NULL;
}

void VArrayType::determineChildren()
{
	ArrayType* node = static_cast<ArrayType*> (getNode());

	if (!type_)
	{
		type_ = renderer()->render(NULL, node->type());
		container_->prepend(type_);
	}

	// TODO: find a better way and place to determine the style of children. Is doing this causing too many updates?
	// TODO: consider the performance of this. Possibly introduce a style updated boolean for all items so that they know
	//			what's the reason they are being updated.
	// The style needs to be updated every time since if our own style changes, so will that of the children.
	container_->setStyle( &style()->container());
	symbol_->setStyle( &style()->symbol());
}

void VArrayType::updateGeometry(int availableWidth, int availableHeight)
{
	Item::updateGeometry(container_, availableWidth, availableHeight);
}

bool VArrayType::focusChild(FocusTarget location)
{
	return container_->focusChild(location);
}

}