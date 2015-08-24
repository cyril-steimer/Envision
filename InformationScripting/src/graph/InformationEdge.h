/***********************************************************************************************************************
**
** Copyright (c) 2011, 2015 ETH Zurich
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

#include "../informationscripting_api.h"

#include "Property.h"
#include "PropertyMap.h"

namespace InformationScripting {

class InformationNode;

class InformationEdge : public PropertyMap
{
	public:
		enum class Orientation : int {Directed, Undirected};

		InformationEdge() = default;
		InformationEdge(InformationNode* from, InformationNode* to, const QString& name,
							 Orientation orientation = Orientation::Directed);
		InformationEdge(const InformationEdge& other);

		int count() const;
		void incrementCount();
		QString name() const;

		InformationNode* from() const;
		InformationNode* to() const;
		void setFrom(InformationNode* from);
		void setTo(InformationNode* to);

		bool isDirected() const;
		Orientation orientation() const;

	private:
		static const QString COUNT_PROPERTY_;
		static const QString NAME_PROPERTY_;

		InformationNode* from_{};
		InformationNode* to_{};

		Orientation orientation_{};
};

inline int InformationEdge::count() const { auto it = find(COUNT_PROPERTY_); Q_ASSERT(it != end()); return it->second; }
inline QString InformationEdge::name() const
	{ auto it = find(NAME_PROPERTY_); Q_ASSERT(it != end()); return it->second; }
inline InformationNode* InformationEdge::from() const { return from_; }
inline InformationNode* InformationEdge::to() const { return to_; }
inline bool InformationEdge::isDirected() const { return orientation_ == Orientation::Directed; }
inline InformationEdge::Orientation InformationEdge::orientation() const { return orientation_; }

} /* namespace InformationScripting */
