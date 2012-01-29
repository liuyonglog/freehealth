/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2011 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *   Main Developpers :                                                    *
 *       Eric MAEKER, MD <eric.maeker@gmail.com>                           *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef DATAPACK_PACKMANAGER_H
#define DATAPACK_PACKMANAGER_H

#include <datapackutils/datapack_exporter.h>

#include <QWidget>
class QStandardItemModel;
class QModelIndex;
class QToolBar;
class QListWidgetItem;

/**
 * \file packmanager.h
 * \author Eric MAEKER <eric.maeker@gmail.com>
 * \version 0.6.2
 * \date 27 Jan 2012
*/

namespace DataPack {
class PackDescription;
class Pack;
class PackModel;

namespace Ui {
    class PackManager;
}

class DATAPACK_EXPORT PackManager : public QWidget
{
    Q_OBJECT
public:
    explicit PackManager(QWidget *parent = 0);
    ~PackManager();

public Q_SLOTS:
    bool submitChanges();

private:
    void createActions();
    void createToolbar();
//    void populateServerView(const int serverId);
    void populatePackView(const int packId);

private Q_SLOTS:
//    void onServerIndexActivated(const QModelIndex &index, const QModelIndex &previous);
    void onPackIndexActivated(const QModelIndex &index, const QModelIndex &previous);
    void serverActionTriggered(QAction *a);
    void packActionTriggered(QAction *a);
    void on_listWidgetMenu_currentRowChanged(int row);

private:
    void retranslate();
    void changeEvent(QEvent *e);

private:
    Ui::PackManager *ui;
    PackModel *m_PackModel;
    QAction *aServerRefresh, *aServerRemove, *aServerAdd, *aServerInfo;
    QAction *aInstall, *aInstallAllPack, *aRemove, *aUpdate;
    QToolBar *m_ToolBar;
    QListWidgetItem *m_datapacksItem;
    QListWidgetItem *m_serversItem;
};

}  // End namespace DataPack

#endif // DATAPACK_PACKMANAGER_H
