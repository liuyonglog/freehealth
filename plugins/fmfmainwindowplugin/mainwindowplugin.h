/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2016 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *  Main developer: Eric MAEKER, <eric.maeker@gmail.com>                   *
 *  Contributors:                                                          *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef MAINWIN_PLUGIN_H
#define MAINWIN_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include <QtCore/QObject>

/**
 * \file mainwindowplugin.h
 * \author Eric MAEKER <eric.maeker@gmail.com>
 * \version 0.8.0
 * \date 25 July 2012
*/

namespace MainWin {
class MainWindow;

namespace Internal {
class MainWindowPreferencesPage;
class VirtualPatientBasePage;

class MainWinPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.freehealth.ehr.MainWindowPlugin" FILE "MainWindow.json")

public:
    MainWinPlugin();
    ~MainWinPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    MainWindow *m_MainWindow;
    Internal::MainWindowPreferencesPage *prefPage;
    Internal::VirtualPatientBasePage *virtualBasePage;
};

} // End Internal
}  // End MainWin

#endif  // End MAINWIN_PLUGIN_H
