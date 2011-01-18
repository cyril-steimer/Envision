/***********************************************************************************************************************
 * CommandExecutionEngine.cpp
 *
 *  Created on: Jan 17, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "commands/Command.h"
#include "commands/CommandExecutionEngine.h"
#include "commands/CommandResult.h"
#include "GenericHandler.h"

#include "interactionbase.h"

#include "VisualizationBase/headers/items/Item.h"
#include "VisualizationBase/headers/items/SceneHandlerItem.h"
#include "VisualizationBase/headers/Scene.h"

namespace Interaction {

const char* QUOTE_SYMBOLS = "\"'`";
const char* ESCAPE_SYMBOLS = "\\";

CommandExecutionEngine::CommandExecutionEngine() :
	lastCommandResult(NULL)
{
}

CommandExecutionEngine::~CommandExecutionEngine()
{
	deleteLastCommandResult();
}

void CommandExecutionEngine::deleteLastCommandResult()
{
	SAFE_DELETE(lastCommandResult);
}

CommandExecutionEngine* CommandExecutionEngine::instance()
{
	static CommandExecutionEngine engine;
	return &engine;
}

void CommandExecutionEngine::execute(Visualization::Item *originator, const QString& command)
{
	deleteLastCommandResult();

	QString trimmed = command.trimmed();

	if ( !doQuotesMatch(command, QUOTE_SYMBOLS, ESCAPE_SYMBOLS) )
	{
		lastCommandResult = new CommandResult(new CommandError("A quoted string expands past the end of the command."));
		return;
	}

	// First extract navigation information if any
	QString simplified = trimmed.simplified(); // Note that this will destroy any spacing within quotations.
	QString navigation;
	if ( simplified.startsWith("~ ") || simplified.startsWith(".. ") || simplified.startsWith(". ") || simplified.startsWith("../")
			|| simplified.startsWith("./") || simplified.startsWith("/") )
	{
		navigation = trimmed.left(simplified.indexOf(' '));
		trimmed = trimmed.mid(navigation.size()).trimmed();
	}

	// This is the node where we begin trying to process the command
	Visualization::Item* source = originator;
	// Alter the source node according to the requested navigation location.
	if (!navigation.isEmpty()) source = navigate(originator, navigation);

	// This is the node (source or one of its ancestors) where we manage to process the command.
	Visualization::Item* target = source;

	QStringList tokens = tokenize(trimmed, QUOTE_SYMBOLS, ESCAPE_SYMBOLS);

	bool processed = false;

	while(target != NULL && !processed)
	{
		GenericHandler* handler = dynamic_cast<GenericHandler*> (target->handler());

		if (handler)
		{
			for(int i = 0; i< handler->commands().size(); ++i)
			{
				if ( handler->commands().at(i)->canInterpret(source, target, tokens) )
				{
					lastCommandResult = handler->commands().at(i)->execute(source, target, tokens);

					if (lastCommandResult->code() != CommandResult::CanNotInterpret)
					{
						processed = true;
						break;
					}
					else deleteLastCommandResult();
				}
			}
		}

		if (!processed) target = static_cast<Visualization::Item*> (target->parentItem());
	}

	// If no item can process this command dispatch it to the SceneItem
	if (!processed)
	{
		GenericHandler* handler = dynamic_cast<GenericHandler*> (source->scene()->sceneHandlerItem()->handler());

		if ( handler )
		{
			for (int i = 0; i < handler->commands().size(); ++i)
			{
				if ( handler->commands().at(i)->canInterpret(source, NULL, tokens) )
				{
					lastCommandResult = handler->commands().at(i)->execute(source, NULL, tokens);

					if ( lastCommandResult->code() != CommandResult::CanNotInterpret )
					{
						processed = true;
						break;
					}
					else deleteLastCommandResult();
				}
			}
		}
	}

	// If the command is still not processed this is an error
	if (!processed)
	{
		lastCommandResult = new CommandResult(new CommandError("Unknown command."));
		InteractionBase::log()->add(Logger::Log::LOGWARNING, "Unknown command: " + command);
	}
}

Visualization::Item* CommandExecutionEngine::navigate(Visualization::Item *originator, const QString&)
{
	//TODO figure out what navigation we want and implement it.
	return originator;
}

QStringList CommandExecutionEngine::tokenize(const QString& string, const QString& quoteSymbols, const QString& escapeSymbols)
{
	QStringList result;
	QString str;

	QChar quote;

	for (int i = 0; i < string.size(); ++i)
	{
		if ( quote.isNull() )
		{
			if ( quoteSymbols.contains(string[i]) )
			{
				if ( !str.isNull() ) result.append(tokenizeNonQuoted(str.simplified()));
				quote = string[i];
				str = quote;
			}
			else str.append(string[i]);
		}
		else
		{
			if ( string[i] == quote && !isEscaped(string, i, escapeSymbols) )
			{
				result.append(str + quote);
				quote = QChar();
				str = QString();
			}
		}
	}

	if ( !str.isNull() ) result.append(str.simplified());
	return result;
}

QStringList CommandExecutionEngine::tokenizeNonQuoted(const QString& string)
{
	//TODO The concept of tokens here is unclear. Define this better.
	QStringList result;
	QString str;

	QChar last;
	bool fractionInitiated = false;

	for (int i = 0; i < string.size(); ++i)
	{
		if ( string[i] == ' ' )
		{
			if ( !str.isNull() ) result.append(str);
			str = QString();
			fractionInitiated = false;
		}
		else if ( string[i].isDigit() && fractionInitiated && last == '.' )
		{
			str = result.last() + '.' + string[i];
			result.removeLast();
			fractionInitiated = false;
		}
		else if ( string[i].isDigit() && str.isNull() )
		{
			fractionInitiated = true;
			str += string[i];
		}
		else if ( string[i] == '.' && fractionInitiated )
		{
			result.append(str);
			str = ".";
		}
		else if ( string[i].isDigit() && fractionInitiated )
		{
			str += string[i];
		}
		else if ( (string[i].isLetter() || string[i].isDigit() || string[i] == '_') != (last.isLetter() || last.isDigit() || last == '_') )
		{
			if ( !str.isNull() ) result.append(str);
			str = string[i];
			fractionInitiated = false;
		}
		else
		{
			str += string[i];
		}

		last = string[i];
	}

	if ( !str.isNull() ) result.append(str);
	return result;
}

bool CommandExecutionEngine::doQuotesMatch(const QString& string, const QString& quoteSymbols, const QString& escapeSymbols)
{
	QChar quote;

	for (int i = 0; i < string.size(); ++i)
	{
		if ( quote.isNull() )
		{
			if ( quoteSymbols.contains(string[i]) ) quote = string[i];
		}
		else
		{
			if ( string[i] == quote && !isEscaped(string, i, escapeSymbols) ) quote = QChar();
		}
	}

	return quote.isNull();
}

bool CommandExecutionEngine::isEscaped(const QString& string, int indexToCheck, const QString& escapeSymbols)
{
	int num = 0;
	int index = indexToCheck - 1;
	while ( index >= 0 && escapeSymbols.contains(string.at(index)) )
	{
		--index;
		++num;
	}

	return num % 2 == 1;
}

}