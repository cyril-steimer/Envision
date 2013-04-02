/***********************************************************************************************************************
**
** Copyright (c) 2011, 2013 ETH Zurich
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

#pragma once

#include "List.h"

#if defined(ModelBase_TypedListDefinition)
	#define TYPEDLIST_EXPORT Q_DECL_EXPORT
#else
	#define TYPEDLIST_EXPORT
#endif

namespace Model {

/* *********************************************************************************************************************
 * TypedList declaration
 **********************************************************************************************************************/
template <class T>
class TYPEDLIST_EXPORT TypedList: public List
{
	NODE_DECLARE_STANDARD_METHODS(TypedList)

	public:
		typedef std::function<T* ()> CreateDefaultElement;

		T* first();
		T* last();
		T* at(int i);

		virtual bool replaceChild(Node* child, Node* replacement);

		virtual Node* createDefaultElement() override;
		static void setDefaultElementCreationFunction(CreateDefaultElement function);

	private:
		static CreateDefaultElement& creationFunction();
};

/* *********************************************************************************************************************
 * TypedList implementation
 **********************************************************************************************************************/
#if defined(ModelBase_TypedListDefinition)

template<class T> typename TypedList<T>::CreateDefaultElement& TypedList<T>::creationFunction()
{
	static CreateDefaultElement function;
	return function;
}

template<class T> void TypedList<T>::setDefaultElementCreationFunction(CreateDefaultElement function)
{
		creationFunction() = function;
}

template<class T>
TypedList<T>::TypedList(::Model::Node* parent) :
	List(parent)
{
}

template<class T>
TypedList<T>::TypedList(::Model::Node *parent, ::Model::PersistentStore &store, bool partialLoadHint) :
	List(parent, store, partialLoadHint)
{
}

template<class T>
::Model::InitializationRegistry& TypedList<T>::initializationRegistry()
{
	return T::initializationRegistry();
}

// This must be set to the result of Node::registerNodeType
// This variable uses a clever trick to register an initialization function that will be called during the
// plug-in's initialization routine
template<class T> int TypedList<T>::typeId_ = (initializationRegistry().add(TypedList<T>::init) , -1);

template<class T>
const QString& TypedList<T>::typeName() const
{
	return typeNameStatic();
}

template<class T>
int TypedList<T>::typeId() const
{
	return typeId_;
}

template<class T>
QList<int> TypedList<T>::hierarchyTypeIds() const
{
	auto l = List::hierarchyTypeIds();
	l.prepend(typeIdStatic());
	return l;
}

template<class T>
int TypedList<T>::typeIdStatic()
{
	return typeId_;
}

template<class T>
const QString& TypedList<T>::typeNameStatic()
{
	static QString typeName_(QString("TypedListOf") + T::typeNameStatic());
	return typeName_;
}

template<class T>
void TypedList<T>::registerNodeType()
{
	typeId_ = Node::registerNodeType(typeNameStatic(), ::Model::createNewNode<TypedList<T> >,
			::Model::createNodeFromPersistence<TypedList<T> >);
}

template<class T>
void TypedList<T>::init()
{
	registerNodeType();
}

template<class T> T* TypedList<T>::first()
{
	return List::first<T>();
}
template<class T> T* TypedList<T>::last()
{
	return List::last<T>();
}
template<class T> T* TypedList<T>::at(int i)
{
	return List::at<T>(i);
}

template<class T> bool TypedList<T>::replaceChild(Node* child, Node* replacement)
{
	if (!dynamic_cast<T*>(replacement)) return false;
	else return List::replaceChild(child, replacement);
}

template<class T> Node* TypedList<T>::createDefaultElement()
{
	if (creationFunction()) return creationFunction()();
	else return nullptr;
}

#endif
}
