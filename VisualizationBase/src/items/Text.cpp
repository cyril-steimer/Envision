/***********************************************************************************************************************
 * Text.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "items/Text.h"

#include "Styles.h"

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>

namespace Visualization {

Text::Text(Item* parent, const QString& text_) :
	Item(parent, Styles::item<Text>("default")), text(text_)
{
}

Text::Text(Item* parent, const TextStyle *style_, const QString& text_) :
	Item(parent, style_), text(text_)
{
}

void Text::determineChildren()
{
}

void Text::updateState()
{
	QFontMetrics qfm(style()->font());

	bounding_rect = qfm.boundingRect(text);
	xOffset = - bounding_rect.left();
	yOffset = - bounding_rect.top();
	bounding_rect.moveTopLeft(QPointF(0,0));
	size.setWidth(bounding_rect.width());
	size.setHeight(bounding_rect.height());
}

void Text::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(style()->pen());
	painter->setFont(style()->font());
	painter->drawText(xOffset, yOffset, text);
}

}
