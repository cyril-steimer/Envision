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
#include "HModule.h"

#include "commands/CCreateModule.h"
#include "commands/CCreateClass.h"
#include "commands/CCreateMethod.h"
#include "commands/CCreateField.h"

#include "OOVisualization/src/declarations/VModule.h"
#include "OOModel/src/declarations/Module.h"

#include "FilePersistence/src/SystemClipboard.h"

namespace OOInteraction {

HModule::HModule()
{
	addCommand(new CCreateModule());
	addCommand(new CCreateClass());
	addCommand(new CCreateMethod());
	addCommand(new CCreateField());
}

HModule* HModule::instance()
{
	static HModule h;
	return &h;
}

void HModule::keyPressEvent(Visualization::Item *target, QKeyEvent *event)
{
	if (event->matches(QKeySequence::Paste))
	{
		FilePersistence::SystemClipboard clipboard;
		if (clipboard.numNodes() == 1 && clipboard.currentNodeType() == OOModel::Module::typeNameStatic())
		{
			if (target->hasNode() && target->node()->typeName() == clipboard.currentNodeType())
			{
				auto module = static_cast<OOVisualization::VModule*>(target);
				module->node()->beginModification("paste a module");
				auto newModule = new OOModel::Module();
				module->node()->modules()->append(newModule);
				newModule->load(clipboard);
				module->node()->endModification();
				module->setUpdateNeeded(Visualization::Item::StandardUpdate);
			}
			else GenericHandler::keyPressEvent(target, event);
		}
		else GenericHandler::keyPressEvent(target, event);
	}
	else if (event->modifiers() == Qt::NoModifier && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
	{
		showCommandPrompt(target);
	}
	else GenericHandler::keyPressEvent(target, event);
}

} /* namespace OOInteraction */
