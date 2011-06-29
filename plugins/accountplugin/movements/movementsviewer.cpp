/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2011 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  The FreeAccount plugins are free, open source FreeMedForms' plugins.   *
 *  (C) 2010-2011 by Pierre-Marie Desombre, MD <pm.desombre@medsyn.fr>     *
 *  and Eric Maeker, MD <eric.maeker@gmail.com>                            *
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
 *  Main Developpers : Pierre-Marie DESOMBRE <pm.desombre@medsyn.fr>,      *
 *                     Eric MAEKER, <eric.maeker@gmail.com>                *
 *  Contributors :                                                         *
 *      NAME <MAIL@ADRESS>                                                 *
 ***************************************************************************/
#include "movementsviewer.h"
#include "movementsIO.h"
#include "movementsmanager.h"
#include "../accountwidgetmanager.h"

#include <accountbaseplugin/movementmodel.h>
#include <accountbaseplugin/constants.h>

#include "ui_movementsviewer.h"

#include <QMessageBox>
#include <QDebug>
enum { WarnDebugMessage = false };
/*********************/
//todo bank system et deposit
/********************/

using namespace AccountDB;
using namespace Account;
using namespace Constants;

MovementsViewer::MovementsViewer(QWidget * parent) :
        QWidget(parent),ui(new Ui::MovementsViewerWidget)
{
    ui->setupUi(this);
    //instanciate
    m_valid = 0; //bill not received
    ui->valueDoubleSpinBox->setRange(0.00,999999999999999.00);
    ui->percentDoubleSpinBox->setRange(0.00,100.00);
    ui->percentDoubleSpinBox->setValue(100.00);
    ui->dateEdit->setDate(QDate::currentDate());
    fillMovementsComboBox();
    fillYearComboBox();
    fillBankComboBox();
    ui->valAndRecButton->setShortcut(QKeySequence::InsertParagraphSeparator);
    showMovements();
    connect(ui->quitButton,SIGNAL(pressed()),this,SLOT(close()));
    connect(ui->recordButton,SIGNAL(pressed()),this,SLOT(recordMovement()));
    connect(ui->deleteButton,SIGNAL(pressed()),this,SLOT(deleteMovement()));
    connect(ui->validButton,SIGNAL(pressed()),this,SLOT(validMovement()));
    connect(ui->valAndRecButton,SIGNAL(pressed()),this,SLOT(validAndRecord()));
    connect(ui->movementsComboBox,SIGNAL(highlighted(int)),this,
                                  SLOT(setMovementsComboBoxToolTips(int)));
    
}

MovementsViewer::~MovementsViewer()
{
}

void MovementsViewer::showMovements()
{
    MovementsIODb  mov(this) ;
    QString year = ui->yearComboBox->currentText();
    MovementModel * model = mov.getModelMovements(year);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::SelectedClicked);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection ( true );
    ui->tableView->verticalHeader()  ->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setColumnHidden(MOV_ID,true);
    ui->tableView->setColumnHidden(MOV_AV_MOVEMENT_ID,true);
    ui->tableView->setColumnHidden(MOV_USER_UID,true);
    ui->tableView->setColumnHidden(MOV_ACCOUNT_ID,true);
    ui->tableView->setColumnHidden(MOV_TYPE,true);
    ui->tableView->setColumnHidden(MOV_TRACE,true);
    ui->tableView->setColumnHidden(MOV_COMMENT,true);
}

void MovementsViewer::recordMovement()
{
    MovementsIODb  mov(this) ;
    MovementsManager manager;
    QHash<int,QVariant>  hashValues;
    QString availableMovement = ui->movementsComboBox->currentText();
    int acMovId = mov.getAvailableMovementId(availableMovement);
    QString userUid = mov.getUserUid();
    QString bank = ui->bankComboBox->currentText();
    int bankId = mov.getBankId(bank);
    int type = mov.getTypeOfMovement(availableMovement);
    QString label = availableMovement;
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString dateValue = ui->dateEdit->date().toString("yyyy-MM-dd");
    double valueCalculated = 0.00;
    valueCalculated = manager.getCalculatedValue(ui->valueDoubleSpinBox->value(),ui->percentDoubleSpinBox->value());    
    QString comment;//no comment
    int validity = 0;
    QString trace;// ??
    int isValid = m_valid;
    QString details = ui->detailsEdit->text();
    hashValues = manager.getHashOfValues(acMovId ,
                                         userUid,
                                         bankId,
                                         type,
                                         label,
                                         date,
                                         dateValue ,
                                         valueCalculated ,
                                         comment,
                                         validity ,
                                         trace ,
                                         isValid ,
                                         details);
    
    if (!mov.insertIntoMovements(hashValues)) {
        QMessageBox::warning(0,trUtf8("Error"),trUtf8("Movement not inserted."),QMessageBox::Ok);
    } else {
        QMessageBox::information(0,trUtf8("Information"),trUtf8("Movement is inserted."),QMessageBox::Ok);
    }
    showMovements();
}

void MovementsViewer::deleteMovement()
{
    QModelIndex index = ui->tableView->QAbstractItemView::currentIndex();
    if(!index.isValid()) {
        QMessageBox::warning(0,trUtf8("Error"),trUtf8("You forgot to select a line."),QMessageBox::Ok);
    }
    int row = index.row(); 
    if (WarnDebugMessage)
    	      qDebug() << __FILE__ << QString::number(__LINE__) << " row =" << QString::number(row) ;
    MovementsIODb  mov(this) ;
    if (mov.containsFixAsset(row))
    {
    	  QMessageBox::warning(0,trUtf8("Error"),trUtf8("This fixed asset cannot be deleted.\nDo it in assets."),
    	                       QMessageBox::Ok);
    	  return;
        }
    if (!mov.deleteMovement(row)) {
    	QMessageBox::warning(0,trUtf8("Error"),trUtf8("Movement is not deleted."),QMessageBox::Ok);
    }  else {
        QMessageBox::information(0,trUtf8("Information"),trUtf8("Movement is deleted."),QMessageBox::Ok);
    }
    showMovements();
}

void MovementsViewer::validMovement()
{
    QModelIndex index = ui->tableView->QAbstractItemView::currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(0,trUtf8("Error"),trUtf8("You forgot to select a line."),QMessageBox::Ok);
    }
    int row = index.row(); 
    MovementsIODb  mov(this) ;
    if (!mov.validMovement(row)) {
    	QMessageBox::warning(0,trUtf8("Error"),trUtf8("Movement is not validated."),QMessageBox::Ok);
    } else {
        QMessageBox::information(0,trUtf8("Information"),trUtf8("Movement is validated."),QMessageBox::Ok);
    }
    showMovements();
}

void MovementsViewer::validAndRecord()
{
    m_valid = 1;
    recordMovement();// 1 = invoice,bill received
    m_valid = 0;
}

void MovementsViewer::fillMovementsComboBox()
{
    MovementsIODb mov(this);
    ui->movementsComboBox->setModel(mov.getMovementsComboBoxModel(this));
}

void MovementsViewer::setMovementsComboBoxToolTips(int row){
    QHash<QString,QString> hashChildrenAndParents;
    MovementsIODb mov(this);
    hashChildrenAndParents = mov.hashChildrenAndParentsAvailableMovements();
    QString text = ui->movementsComboBox->itemText(row);
    QString parent = hashChildrenAndParents.value(text);
    QString toolTipText = QString("Parent = %1").arg(parent);
    QAbstractItemModel *m = new QStandardItemModel;
    m = ui->movementsComboBox->model();
    QStandardItemModel *model = qobject_cast<QStandardItemModel*> (m);
    QStandardItem *i = model->item(row);
    i->setToolTip(toolTipText);
}

void MovementsViewer::fillYearComboBox()
{
    MovementsIODb mov(this);
    QStringList listOfYears;
    listOfYears = mov.getYearComboBoxModel();
    if (listOfYears.isEmpty()) {
        listOfYears << QString::number(QDate::currentDate().year());
    }
    ui->yearComboBox->addItems(listOfYears);
}

void MovementsViewer::fillBankComboBox(){
    MovementsIODb mov(this);
    ui->bankComboBox->setModel(mov.getBankComboBoxModel(this));
}


