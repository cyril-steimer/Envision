/***********************************************************************************************************************
 * VIntegerLiteral.h
 *
 *  Created on: Feb 11, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#ifndef VINTEGERLITERAL_H_
#define VINTEGERLITERAL_H_

#include "../oovisualization_api.h"

#include "VisualizationBase/headers/items/ModelItem.h"
#include "VisualizationBase/headers/items/TextStyle.h"
#include "VisualizationBase/headers/Styles.h"

#include "OOModel/headers/expressions/IntegerLiteral.h"

namespace OOVisualization {

class OOVISUALIZATION_API VIntegerLiteral : public Visualization::ModelItem
{
	ITEM_COMMON_CUSTOM_STYLENAME(VIntegerLiteral, Visualization::ModelItem, Visualization::TextStyle)

	public:
		VIntegerLiteral(Item* parent, OOModel::IntegerLiteral *literal, const Visualization::TextStyle *style = Visualization::Styles::item<VIntegerLiteral>("default"));
		virtual ~VIntegerLiteral();

		virtual bool focusChild(FocusTarget location);

	protected:
		virtual void determineChildren();
		virtual void updateGeometry(int availableWidth, int availableHeight);

	private:
		Visualization::ModelItem* vis_;
};

}

#endif /* VINTEGERLITERAL_H_ */
