/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2014 ETH Zurich
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

#include "GenericNode.h"
#include "GenericPersistentUnit.h"
#include "Parser.h"

namespace FilePersistence {

static const int MAX_DOUBLE_PRECISION = 15;

GenericNode::GenericNode(){}

inline bool GenericNode::sameTree(const GenericNode* other)
{
	return persistentUnit_->tree() == other->persistentUnit_->tree();
}

const QString& GenericNode::valueAsString() const
{
	ensureDataRead();
	Q_ASSERT(valueType_ == STRING_VALUE);
	return value_;
}

long GenericNode::valueAsLong() const
{
	ensureDataRead();
	Q_ASSERT(valueType_ == INT_VALUE);
	bool ok = true;

	int res = value_.toInt(&ok);
	if ( !ok ) throw FilePersistenceException("Could not read integer value " + value_);

	return res;
}

double GenericNode::valueAsDouble() const
{
	ensureDataRead();
	Q_ASSERT(valueType_ == DOUBLE_VALUE);
	bool ok = true;

	double res = value_.toDouble(&ok);
	if ( !ok ) throw FilePersistenceException("Could read real value " + value_);

	return res;
}

GenericNode* GenericNode::child(const QString& name)
{
	ensureDataRead();
	for (auto c : children_)
		if (c->name() == name) return c;

	return nullptr;
}

void GenericNode::setValue(const QString& value)
{
	Q_ASSERT(children_.isEmpty());
	Q_ASSERT(valueType_ == NO_VALUE);

	valueType_ = STRING_VALUE;
	value_ = value;
}

void GenericNode::setValue(long value)
{
	Q_ASSERT(children_.isEmpty());
	Q_ASSERT(valueType_ == NO_VALUE);

	valueType_ = INT_VALUE;
	value_ = QString::number(value);
}

void GenericNode::setValue(double value)
{
	Q_ASSERT(children_.isEmpty());
	Q_ASSERT(valueType_ == NO_VALUE);

	valueType_ = DOUBLE_VALUE;
	value_ = QString::number(value, 'f', MAX_DOUBLE_PRECISION);
}

void GenericNode::setValue(ValueType type, const QString& value)
{
	Q_ASSERT(children_.isEmpty());
	Q_ASSERT(valueType_ == NO_VALUE);
	Q_ASSERT(type != NO_VALUE);

	valueType_ = type;
	value_ = value;
}


void GenericNode::setParent(GenericNode* parent)
{
	if (parent) Q_ASSERT(sameTree(parent));
	parent_ = parent;
}

GenericNode* GenericNode::addChild(GenericNode* child)
{
	Q_ASSERT(child);
	Q_ASSERT(sameTree(child));
	Q_ASSERT(value_.isEmpty());
	Q_ASSERT(valueType_ == NO_VALUE);

	children_.append(child);
	return child;
}

GenericNode* GenericNode::find(Model::NodeIdType id)
{
	ensureDataRead();
	if (id_ == id) return this;
	for (auto c : children_)
		if (auto found = c->find(id)) return found;

	return nullptr;
}

void GenericNode::reset(GenericPersistentUnit* persistentUnit)
{
	Q_ASSERT(persistentUnit);
	persistentUnit_ = persistentUnit;

	name_.clear();
	type_.clear();
	value_.clear();
	valueType_ = NO_VALUE;
	id_ = {};
	parentId_ = {};
	children_.clear();
	parent_ = nullptr;
	dataLine_ = nullptr;
	dataLineLength_ = {};
}

void GenericNode::reset(GenericPersistentUnit* persistentUnit, const char* dataLine, int dataLineLength, bool lazy)
{
	reset(persistentUnit);
	Q_ASSERT(dataLine);
	Q_ASSERT(dataLineLength > 0);

	if (lazy)
	{
		dataLine_ = dataLine;
		dataLineLength_ = dataLineLength;
	}
	else
		Parser::parseLine(const_cast<GenericNode*>(this), dataLine, dataLineLength);
}

void GenericNode::reset(GenericPersistentUnit* persistentUnit, const GenericNode* nodeToCopy)
{
	Q_ASSERT(nodeToCopy);
	reset(persistentUnit);

	if (nodeToCopy->dataLine_)
		Parser::parseLine(const_cast<GenericNode*>(this), nodeToCopy->dataLine_, nodeToCopy->dataLineLength_);
	else
	{
		name_ = nodeToCopy->name_;
		type_ = nodeToCopy->type_;
		value_ = nodeToCopy->value_;
		valueType_ = nodeToCopy->valueType_;
		id_ = nodeToCopy->id_;
		parentId_ = nodeToCopy->parentId_;
	}
}

void GenericNode::ensureDataRead() const
{
	if (dataLine_)
	{
		Parser::parseLine(const_cast<GenericNode*>(this), dataLine_, dataLineLength_);
		// Don't delete the line, we don't own it.
		const_cast<GenericNode*>(this)->dataLine_ = nullptr;
	}
}

} /* namespace FilePersistence */
