/***********************************************************************************************************************
 * TestRunner.h
 *
 *  Created on: Nov 2, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include "PluginManager.h"
#include <QtCore/QEvent>
#include <QtCore/QObject>

namespace Envision {

class TestRunner: public QObject
{
	Q_OBJECT

	private:
		PluginManager *pm;

	protected:
		void customEvent ( QEvent * event );

	public:
		TestRunner();
		void enqueueSelfTests(PluginManager& pm);
};

}

#endif /* TESTRUNNER_H_ */
