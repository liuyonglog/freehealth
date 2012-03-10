/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *   Main developers : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
/**
  \namespace BaseWidgets
  \brief Namespace reserved for the BaseWidgets plugin.
*/

#include "basewidgetsplugin.h"
#include "baseformwidgets.h"
#include "baseformwidgetsoptionspage.h"
#include "texteditorfactory.h"
#include "identitywidgetfactory.h"
#include "calculationwidgets.h"

#include <coreplugin/dialogs/pluginaboutpage.h>

#include <utils/log.h>

#include <QtCore/QtPlugin>
#include <QDebug>

using namespace BaseWidgets;
using namespace Internal;

BaseWidgetsPlugin::BaseWidgetsPlugin() :
        m_Factory(0),
        m_OptionsPage(0)
{
    if (Utils::Log::warnPluginsCreation())
        qWarning() << "creating BaseWidgetsPlugin";
}

BaseWidgetsPlugin::~BaseWidgetsPlugin()
{
    if (m_Factory) {
        removeObject(m_Factory);
        delete m_Factory;
        m_Factory = 0;
    }
    if (m_CalcFactory) {
        removeObject(m_CalcFactory);
        delete m_CalcFactory;
        m_CalcFactory = 0;
    }
    if (m_OptionsPage) {
//        removeObject(m_OptionsPage);
        delete m_OptionsPage;
        m_OptionsPage = 0;
    }
}

bool BaseWidgetsPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    if (Utils::Log::warnPluginsCreation())
        qWarning() << "BaseWidgetsPlugin::initialize";

    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    m_Factory = new BaseWidgetsFactory(this);
    m_Factory->initialize(arguments,errorString);
    m_CalcFactory = new CalculationWidgetsFactory(this);
    m_CalcFactory->initialize(arguments,errorString);
    return true;
}

void BaseWidgetsPlugin::extensionsInitialized()
{
    if (Utils::Log::warnPluginsCreation())
        qWarning() << "BaseWidgetsPlugin::extensionsInitialized";

    m_OptionsPage = new Internal::BaseFormWidgetsOptionsPage(this);
    m_OptionsPage->checkSettingsValidity();

    addAutoReleasedObject(new Core::PluginAboutPage(pluginSpec(), this));
    addObject(m_Factory);
    addObject(m_CalcFactory);
    addAutoReleasedObject(new TextEditorFactory(this));
    addAutoReleasedObject(new IdentityWidgetFactory(this));

//    addObject(m_OptionsPage);
}


Q_EXPORT_PLUGIN(BaseWidgetsPlugin)
