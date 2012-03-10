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
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#include "pmhcore.h"
#include "pmhcategorymodel.h"
#include "pmhwidgetmanager.h"

#include <QApplication>

using namespace PMH;
using namespace Internal;


PmhCore *PmhCore::m_Instance = 0;

PmhCore *PmhCore::instance(QObject *parent)
{
    if (!m_Instance) {
        if (!parent)
            m_Instance = new PmhCore(qApp);
        else
            m_Instance = new PmhCore(parent);
    }
    return m_Instance;
}

namespace PMH {
namespace Internal {

class PmhCorePrivate
{
public:
    PmhCorePrivate() : m_PmhCategoryModel(0)
    {
    }

    ~PmhCorePrivate()
    {
        if (m_PmhCategoryModel)
            delete m_PmhCategoryModel;
        m_PmhCategoryModel = 0;
    }

public:
    PmhCategoryModel *m_PmhCategoryModel;
};

}
}


PmhCore::PmhCore(QObject *parent) :
    QObject(parent), d(new PmhCorePrivate)
{
    d->m_PmhCategoryModel = new PmhCategoryModel(this);
    PmhWidgetManager::instance(this);
}

PmhCore::~PmhCore()
{
    if (d)
        delete d;
    d = 0;
}

PmhCategoryModel *PmhCore::pmhCategoryModel() const
{
    return d->m_PmhCategoryModel;
}

