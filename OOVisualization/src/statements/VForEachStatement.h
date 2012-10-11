/***********************************************************************************************************************
**
** Copyright (c) 2011, ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
**    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**      disclaimer.
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
***********************************************************************************************************************/

/***********************************************************************************************************************
 * VForEachStatement.h
 *
 *  Created on: Feb 16, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#ifndef VFOREACHSTATEMENT_H_
#define VFOREACHSTATEMENT_H_

#include "../oovisualization_api.h"
#include "VForEachStatementStyle.h"

#include "OOModel/src/statements/ForEachStatement.h"

#include "VisualizationBase/src/items/ItemWithNode.h"
#include "VisualizationBase/src/items/LayoutProvider.h"

namespace Visualization {
	class VText;
	class SequentialLayout;
	class PanelBorderLayout;
}

namespace OOVisualization {

class VStatementItemList;

class OOVISUALIZATION_API VForEachStatement
	: public Visualization::ItemWithNode< Visualization::LayoutProvider<Visualization::PanelBorderLayout>,
	  OOModel::ForEachStatement>
{
	ITEM_COMMON(VForEachStatement)

	public:
		VForEachStatement(Item* parent, NodeType* node, const StyleType* style = itemStyles().get());
		virtual ~VForEachStatement();

		Visualization::SequentialLayout* header() const;

		Visualization::VText* varName() const;
		Visualization::Item* collection() const;
		Visualization::Item* varType() const;
		VStatementItemList* body() const;
		Visualization::Item* icon() const;

	protected:
		void determineChildren();

	private:
		typedef Visualization::ItemWithNode< Visualization::LayoutProvider<Visualization::PanelBorderLayout>,
				  OOModel::ForEachStatement> BaseItemType;

		Visualization::SequentialLayout* header_;

		Visualization::SequentialLayout* varContainer_;
		Visualization::SequentialLayout* collectionBackground_;

		Visualization::VText* varName_;
		Visualization::Item* collection_;
		Visualization::Item* varType_;
		VStatementItemList* body_;
};

	inline Visualization::SequentialLayout* VForEachStatement::header() const { return header_; }

inline Visualization::VText* VForEachStatement::varName() const { return varName_; }
inline Visualization::Item* VForEachStatement::collection() const { return collection_; }
inline Visualization::Item* VForEachStatement::varType() const { return varType_; }
inline VStatementItemList* VForEachStatement::body() const { return body_; }

inline Visualization::Item* VForEachStatement::icon() const {return header_->at<Visualization::Item>(0);}

}

#endif /* VFOREACHSTATEMENT_H_ */