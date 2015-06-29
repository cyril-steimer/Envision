/***********************************************************************************************************************
 **
 ** Copyright (c) 2015 ETH Zurich
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
#include "SelectionAtCursorItem.h"

#include "VisualizationBase/src/declarative/DeclarativeItemDef.h"
#include "VisualizationBase/src/declarative/DynamicGridFormElement.h"
#include "VisualizationBase/src/declarative/GridLayouter.h"
#include "VisualizationBase/src/items/Text.h"
#include "VisualizationBase/src/cursor/TextCursor.h"
#include "QGraphicsColorizeEffect"

namespace Interaction {

ITEM_COMMON_DEFINITIONS(SelectionAtCursorItem, "item")

SelectionAtCursorItem* SelectionAtCursorItem::instance{};

void SelectionAtCursorItem::show(QVector<Model::Node*> selectableNodes, const OnSelectAction onSelect, Item *target,
								 bool hasTextField, const OnTextAction onText)
{
	if (instance)
		instance->scene()->removeTopLevelItem(instance);
	instance = new SelectionAtCursorItem(target, selectableNodes, onSelect, hasTextField, onText);
}

void SelectionAtCursorItem::hide()
{
	if (instance)
		instance->scene()->removeTopLevelItem(instance);
	instance = nullptr;
}

SelectionAtCursorItem::SelectionAtCursorItem(Visualization::Item* target, QVector<Model::Node*> selectableNodes,
		const OnSelectAction onSelect, bool hasTextField, const OnTextAction onText, StyleType* style)
	: Super(nullptr, style), target_(target), onSelect_(onSelect), onText_(onText), hasTextField_(hasTextField)
{
	currentNodes_ = arrange(selectableNodes);
	target->scene()->addTopLevelItem(this);
	mousePosition_ = target->scene()->lastMouseHoverPosition();
	selectedEffect_ = new QGraphicsColorizeEffect();
	if (hasTextField)
	{
		textField_ = new Visualization::Text(this, &style->nameField(), "Enter name here");
		textField_->setEditable(true);
		selectItem(textField_);
	}
}

void SelectionAtCursorItem::initializeForms()
{
	auto nodeGrid = (new Visualization::DynamicGridFormElement())
			->setSpacing(5, 5)
			->setMajorAxis(Visualization::GridLayouter::ColumnMajor)
			->setNodesGetter([](Visualization::Item* v)
				{ auto self = static_cast<I*>(v);
				  return self->currentNodes(); });
	//First form: only the nodes
	addForm(nodeGrid);
	//Second form: the nodes and a text field
	addForm((new Visualization::GridLayoutFormElement())
			->setSpacing(5, 5)->setMargins(10)
			->put(0, 0, item<Visualization::Text>(&I::textField_,
									[](I* v) -> const Visualization::TextStyle* { return &v->style()->nameField(); }))
									//[](I* v) { return v->style()->nameField(); }))
			->put(0, 1, nodeGrid));
}

int SelectionAtCursorItem::determineForm()
{
	return hasTextField() ? 1 : 0;
}

void SelectionAtCursorItem::selectItem(Visualization::Item *item)
{
	if (focusedItem() && focusedItem() != textField())
	{
		focusedItem()->setGraphicsEffect(nullptr);
		selectedEffect_ = new QGraphicsColorizeEffect();
	}
	if (item && item != textField())
		item->setGraphicsEffect(selectedEffect_);
	if (item == textField())
	{
		item->moveCursor();
		item->correspondingSceneCursor<Visualization::TextCursor>()->selectAll();
	}
	else moveCursor(Visualization::Item::MoveOnPosition, QPoint(0, 0));
	focusedItem_ = item;
}

void SelectionAtCursorItem::executeFocused()
{
	if (hasTextField() && focusedItem() == textField() && onText_)
		onText_(textField()->text(), target_);
	else if (focusedItem() && onSelect_)
		onSelect_(focusedItem()->node(), target_);
}

QPoint SelectionAtCursorItem::indexOf(Model::Node *node) const
{
	for (int col = 0; col < currentNodes_.size(); col++)
		for (int row = 0; row < currentNodes_[col].size(); row++)
			if (currentNodes_[col][row] == node)
				return QPoint(col, row);
	return QPoint(-1, -1);
}

QPoint SelectionAtCursorItem::correctCoordinates(QPoint point) const
{
	QPoint result = point;
	if (point.x() < 0)
		result.setX(0);
	if (point.x() >= currentNodes_.size())
		result.setX(currentNodes_.size() - 1);
	if (point.y() < 0)
		result.setY(0);
	if (point.y() >= currentNodes_[result.x()].size())
		result.setY(currentNodes_[result.x()].size() - 1);
	return result;
}

void SelectionAtCursorItem::updateGeometry(int availableWidth, int availableHeight)
{
	Super::updateGeometry(availableWidth, availableHeight);
	setPos(mousePosition_.x() - widthInScene() / 2, mousePosition_.y() - heightInScene() / 2);
	if (!focusedItem())
		selectItem(findVisualizationOf(currentNodes()[0][0]));
}

QVector<QVector<Model::Node*>> SelectionAtCursorItem::arrange(QVector<Model::Node *> nodes)
{
	QVector<QVector<Model::Node*>> result;
	result.resize(std::min(3, nodes.size()));
	for (int index = 0; index < nodes.size(); index++)
	{
		int column = index % 3;
		result[column].append(nodes[index]);
	}
	return result;
}

}
