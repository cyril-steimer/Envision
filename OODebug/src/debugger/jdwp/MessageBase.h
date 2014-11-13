/***********************************************************************************************************************
**
** Copyright (c) 2011, 2014 ETH Zurich
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

#include "../../oodebug_api.h"

namespace OODebug {

/**
 * The base class for the Message class, it defines the stream reading and writing operators.
 * This class should only be used by the class Message. Clients should implement the Message class.
 *
 * We use separate MessageBase and Message mainly due to a compiling problem: Message includes MessageField
 * and MessageField includes MessageBase, so if MessageBase and Message would be the same we would have a cyclic
 * dependency.
 */
class OODEBUG_API MessageBase
{
	public:
		/**
		 * The direction of a MessageField.
		 *
		 * In means the MessageField will be set when using the >> operator.
		 * Out mean the MessageField will be used when using the << operator.
		 * We allow to use both simultaniously (In | Out).
		 */
		enum Directions {In = 0x1, Out = 0x2};
		Q_DECLARE_FLAGS(Direction, Directions)

		using ReadOperator = std::function<void (QDataStream&)>;
		void addReadOperator(ReadOperator reader);

		using WriteOperator = std::function<void (QDataStream&)>;
		void addWriteOperator(WriteOperator writer);

		friend QDataStream& operator>>(QDataStream& stream, MessageBase& message);
		friend QDataStream& operator<<(QDataStream& stream, MessageBase& message);

	private:
		QList<ReadOperator> readers_;
		QList<WriteOperator> writers_;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MessageBase::Direction)

inline void MessageBase::addReadOperator(MessageBase::ReadOperator reader) { readers_.append(reader); }
inline void MessageBase::addWriteOperator(MessageBase::WriteOperator writer) { writers_.append(writer); }

} /* namespace OODebug */
