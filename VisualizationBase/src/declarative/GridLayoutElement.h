/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2013 ETH Zurich
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 ** following conditions are met:
 **
 **    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 **      following disclaimer.
 **    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 **      following disclaimer in the documentation and/or other materials provided with the distribution.
 **    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
 **      derived from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 ** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 ** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **
 **********************************************************************************************************************/

#ifndef VisualizationBase_GRIDLAYOUTELEMENT_H_
#define VisualizationBase_GRIDLAYOUTELEMENT_H_

#include "../visualizationbase_api.h"
#include "LayoutElement.h"

namespace Visualization {

class GridLayoutElement : public LayoutElement {
	public: // Methods executable on element definition
		GridLayoutElement(int numHorizontalCells, int numVerticalCells);
		virtual ~GridLayoutElement();
		void add(int cellX, int cellY, Element* element);
		void setSpacing(int spacing);
		void setSpacing(int horizontalSpacing, int verticalSpacing);
		void setHorizontalSpacing(int horizontalSpacing);
		void setVerticalSpacing(int verticalSpacing);

	public: // Methods executable when items need to be rendered
		virtual void computeSize(Item* item, int availableWidth, int availableHeight) override;
		virtual void setItemPositions(Item* item, int parentX, int parentY) override;
		virtual void synchronizeWithItem(Item* item) override;

	private:
		int numHorizontalCells_;
		int numVerticalCells_;
		int horizontalSpacing_;
		int verticalSpacing_;
		QVector<QVector<Element*>> elementGrid_{};
};

inline void GridLayoutElement::setSpacing(int spacing)
{
	horizontalSpacing_ = spacing;
	verticalSpacing_ = spacing;
}
inline void GridLayoutElement::setSpacing(int horizontalSpacing, int verticalSpacing)
{
	horizontalSpacing_ = horizontalSpacing;
	verticalSpacing_ = verticalSpacing;
}
inline void GridLayoutElement::setHorizontalSpacing(int horizontalSpacing){horizontalSpacing_ = horizontalSpacing;}
inline void GridLayoutElement::setVerticalSpacing(int verticalSpacing){verticalSpacing_ = verticalSpacing;}

} /* namespace Visualization */

#endif /* VisualizationBase_GRIDLAYOUTELEMENT_H_ */
