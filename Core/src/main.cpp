/***********************************************************************************************************************
 * main.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/
#include "EnvisionWindow.h"
#include "PluginManager.h"
#include "EnvisionException.h"
#include "DefaultEnvisionManager.h"
#include "TestRunner.h"

#include <QtGui/QApplication>
#include <QtCore/QTextStream>

using namespace Envision;

/**
 * This is the main executed when Envision is started. It shows the main window, loads all plug-ins and starts the event
 * loop.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EnvisionWindow w;
    w.show();

    TestRunner testr;

    int retCode = 0;
    try
    {
   	 PluginManager pm;
   	 DefaultEnvisionManager manager;

   	 // Give the Envision manager all the information it needs to operate properly.
   	 manager.setPluginManager(&pm);
   	 manager.setMainWindow(&w);

   	 pm.loadAllPlugins(manager);

   	 testr.enqueueSelfTests(pm);

   	 QTextStream out(stdout);
   	 out << "------------------------------" << endl;
   	 out << "All plug-ins loaded." << endl;
   	 out << "------------------------------" << endl;

   	 retCode = a.exec();
    }
    catch (EnvisionException &e)
    {
   	 e.printError();
   	 retCode = 1;
    }

    return retCode;
}
