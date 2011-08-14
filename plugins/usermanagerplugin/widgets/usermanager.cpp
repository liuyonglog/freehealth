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
 *   Main Developper : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
/**
  \class UserPlugin::UserManager
  \brief This is the Users' Manager of FreeMedForms.

  You can use it as main app or secondary mainWindow.\n
  You only need to instanciate the UserModel and define a current user before using this mainwindow.
  \ingroup usertoolkit widget_usertoolkit
  \ingroup usermanager
*/

#include "usermanager.h"
#include "usermanager_p.h"

#include <utils/log.h>
#include <utils/widgets/qbuttonlineedit.h>

#include <coreplugin/icore.h>
#include <coreplugin/itheme.h>
#include <coreplugin/ipatient.h>

#include <coreplugin/contextmanager/icontext.h>
#include <coreplugin/contextmanager/contextmanager.h>
#include <coreplugin/uniqueidmanager.h>
#include <coreplugin/constants_icons.h>
#include <coreplugin/constants_menus.h>

#include <usermanagerplugin/userdata.h>
#include <usermanagerplugin/widgets/userviewer.h>
#include <usermanagerplugin/constants.h>
#include <usermanagerplugin/usermodel.h>
#include <usermanagerplugin/widgets/userwizard.h>

#include <QModelIndex>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QToolButton>
#include <QToolBar>
#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "ui_usermanagerwidget.h"

using namespace UserPlugin;
using namespace Internal;
using namespace Trans::ConstantTranslations;

static inline Core::ContextManager *contextManager() {return  Core::ICore::instance()->contextManager();}
static inline Core::ITheme *theme() {return Core::ICore::instance()->theme();}
static inline Core::IPatient *patient() {return Core::ICore::instance()->patient();}
static inline Core::IUser *user() {return Core::ICore::instance()->user();}

namespace UserPlugin {
namespace Internal {

class UserManagerContext : public Core::IContext
{
public:
    UserManagerContext(QWidget *parent) : Core::IContext(parent), wgt(parent)
    {
        setObjectName("UserManagerContext");
        ctx << Core::ICore::instance()->uniqueIDManager()->uniqueIdentifier(Core::Constants::C_USERMANAGER);
    }
    ~UserManagerContext() {}

    QList<int> context() const {return ctx;}
    QWidget *widget() {return wgt;}

private:
    QWidget *wgt;
    QList<int> ctx;
};

}  // End  Internal
}  // End UserPlugin


/**
  \brief Main user interface for User Manager.
  User Model must have been instanciated BEFORE this interface, and a current user must have been setted.\n
  You must instanciate this class as a pointer in order to avoid errors at deletion.
  \sa UserModel, UserModel::hasCurrentUser()
  \todo Search user by city, search by name & firstname,
*/
UserManager::UserManager(QWidget * parent) :
        QMainWindow(parent)
{
    Q_ASSERT_X(UserModel::instance()->hasCurrentUser(), "UserManager", "NO CURRENT USER");
    if (!UserModel::instance()->hasCurrentUser())
        return;
    setAttribute(Qt::WA_DeleteOnClose);
    m_Widget = new UserManagerWidget(this);
    setCentralWidget(m_Widget);
    setUnifiedTitleAndToolBarOnMac(true);
}

bool UserManager::initialize()
{
    m_Widget->initialize();
    return true;
}

/** \brief Close the usermanager. Check if modifications have to be saved and ask user. */
void UserManager::closeEvent(QCloseEvent *event)
{
    if (m_Widget->canCloseParent()) {
        event->accept();
    } else {
        event->ignore();
    }
}

/** \brief Destructor */
UserManager::~UserManager()
{
    if (Utils::isDebugCompilation())
        qWarning() << "~UserManager";
//    if (m_Widget) {
//        delete m_Widget;
//        m_Widget = 0;
//    }
}



/**
  \brief Main user interface for User Manager.
  User Model must have been instanciated BEFORE this interface, and a current user must have been setted.\n
  You must instanciate this class as a pointer in order to avoid errors at deletion.
  \sa UserModel, UserModel::hasCurrentUser()
  \todo Search user by city, search by name & firstname,
*/
UserManagerDialog::UserManagerDialog(QWidget * parent) :
        QDialog(parent)
{
    Q_ASSERT_X(UserModel::instance()->hasCurrentUser(), "UserManagerDialog", "NO CURRENT USER");
    if (!UserModel::instance()->hasCurrentUser())
        return;
//    setAttribute(Qt::WA_DeleteOnClose);
    QGridLayout *lay = new QGridLayout(this);
    setLayout(lay);
    m_Widget = new UserManagerWidget(this);
    lay->addWidget(m_Widget, 0, 0);
}

bool UserManagerDialog::initialize()
{
    m_Widget->initialize();
    setWindowTitle(tkTr(Trans::Constants::USERMANAGER_TEXT));
    setWindowIcon(theme()->icon(Core::Constants::ICONUSERMANAGER));
    return true;
}

/** \brief Close the usermanager. Check if modifications have to be saved and ask user. */
void UserManagerDialog::done(int r)
{
    if (m_Widget->canCloseParent()) {
        QDialog::done(r);
    }
}

/** \brief Destructor */
UserManagerDialog::~UserManagerDialog()
{
    if (Utils::isDebugCompilation())
        qWarning() << "~UserManagerDialog";
//    if (m_Widget) {
//        delete m_Widget;
//        m_Widget = 0;
//    }
}


/**
  \brief UserManager Main Ui interface.
  \internal
  \ingroup widget_usertoolkit usertoolkit usermanager
*/
UserManagerWidget::UserManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManagerWidget),
    m_ToolBar(0),
    m_SearchToolBut(0),
    aCreateUser(0),
    aModifyUser(0),
    aSave(0),
    aRevert(0),
    aDeleteUser(0),
    aQuit(0),
    aToggleSearchView(0),
    searchByNameAct(0),
    searchByFirstnameAct(0),
    searchByNameAndFirstnameAct(0),
    searchByCityAct(0),
    m_Context(0)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 0 << 1);
    m_SearchBy = Core::IUser::Name;
    aCreateUser = new QAction(this);
    aCreateUser->setObjectName(QString::fromUtf8("aCreateUser"));
    aModifyUser = new QAction(this);
    aModifyUser->setObjectName(QString::fromUtf8("aModifyUser"));
    aSave = new QAction(this);
    aSave->setObjectName(QString::fromUtf8("aSave"));
    aRevert = new QAction(this);
    aRevert->setObjectName(QString::fromUtf8("aRevert"));
    aDeleteUser = new QAction(this);
    aDeleteUser->setObjectName(QString::fromUtf8("aDeleteUser"));
    aQuit = new QAction(this);
    aQuit->setObjectName(QString::fromUtf8("aQuit"));
    aToggleSearchView = new QAction(this);
    aToggleSearchView->setObjectName(QString::fromUtf8("aToggleSearchView"));

    // prepare Search Line Edit
    searchByNameAct = new QAction(m_SearchToolBut);
    searchByFirstnameAct = new QAction(m_SearchToolBut);
    searchByNameAndFirstnameAct = new QAction(m_SearchToolBut);
    searchByCityAct = new QAction(m_SearchToolBut);
    m_SearchToolBut = new QToolButton(ui->searchLineEdit);
    ui->searchLineEdit->setLeftButton(m_SearchToolBut);
    m_SearchToolBut->addAction(searchByNameAct);
    m_SearchToolBut->addAction(searchByFirstnameAct);
    //    m_SearchToolBut->addAction(searchByNameAndFirstnameAct);
    //    m_SearchToolBut->addAction(searchByCityAct);
    m_SearchToolBut->setPopupMode(QToolButton::InstantPopup);

    // manage theme / icons
    Core::ITheme *th = theme();
    aSave->setIcon(th->icon(Core::Constants::ICONSAVE, Core::ITheme::MediumIcon));
    aCreateUser->setIcon(th->icon(Core::Constants::ICONNEWUSER, Core::ITheme::MediumIcon));
    aModifyUser->setIcon(th->icon(Core::Constants::ICONEDITUSER, Core::ITheme::MediumIcon));
    aRevert->setIcon(th->icon(Core::Constants::ICONCLEARUSER, Core::ITheme::MediumIcon));
    aDeleteUser->setIcon(th->icon(Core::Constants::ICONDELETEUSER, Core::ITheme::MediumIcon));
    aQuit->setIcon(th->icon(Core::Constants::ICONEXIT, Core::ITheme::MediumIcon));
    aToggleSearchView->setIcon(th->icon(Core::Constants::ICONSEARCHUSER, Core::ITheme::MediumIcon));

    m_SearchToolBut->setIcon(theme()->icon(Core::Constants::ICONSEARCH));
    searchByNameAct->setIcon(th->icon(Core::Constants::ICONSEARCH));
    searchByFirstnameAct->setIcon(th->icon(Core::Constants::ICONSEARCH));
    searchByNameAndFirstnameAct->setIcon(th->icon(Core::Constants::ICONSEARCH));
    searchByCityAct->setIcon(th->icon(Core::Constants::ICONSEARCH));

    connect(user(), SIGNAL(userChanged()), this, SLOT(onCurrentUserChanged()));
}

bool UserManagerWidget::initialize()
{
//    m_Context = new UserManagerContext(this);
//    contextManager()->addContextObject(m_Context);
    m_ToolBar = new QToolBar(this);
    m_ToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_ToolBar->addAction(aToggleSearchView);
    m_ToolBar->addAction(aCreateUser);
    m_ToolBar->addAction(aModifyUser);
    m_ToolBar->addAction(aSave);
    m_ToolBar->addAction(aDeleteUser);
    m_ToolBar->addAction(aRevert);
//    m_ToolBar->addAction(aQuit);
    m_ToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->toolbarLayout->addWidget(m_ToolBar);

    UserModel *model = UserModel::instance();
    ui->userTableView->setModel(model);
    for(int i=0; i < model->columnCount(); ++i) {
        ui->userTableView->hideColumn(i);
    }
    ui->userTableView->showColumn(Core::IUser::Name);
    ui->userTableView->showColumn(Core::IUser::SecondName);
    ui->userTableView->showColumn(Core::IUser::Firstname);
    ui->userTableView->resizeColumnsToContents();
    ui->userTableView->horizontalHeader()->setStretchLastSection(true);
    ui->userTableView->horizontalHeader()->hide();
    ui->userTableView->verticalHeader()->hide();
    ui->userTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->userTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    retranslate();

    selectUserTableView(UserModel::instance()->currentUserIndex().row());
    analyseCurrentUserRights();

    aSave->setShortcut(QKeySequence::Save);
    aCreateUser->setShortcut(QKeySequence::New);

    // connect slots
    connect(aSave, SIGNAL(triggered()), this, SLOT(onSaveRequested()));
    connect(aCreateUser, SIGNAL(triggered()), this, SLOT(onCreateUserRequested()));
    connect(aRevert, SIGNAL(triggered()), this, SLOT(onClearModificationRequested()));
    connect(aDeleteUser,  SIGNAL(triggered()), this, SLOT(onDeleteUserRequested()));
    connect(aQuit,  SIGNAL(triggered()), this, SIGNAL(closeRequested()));
    connect(aToggleSearchView, SIGNAL(triggered()), this, SLOT(toggleSearchView()));
    connect(ui->userTableView, SIGNAL(activated(const QModelIndex &)),
             this, SLOT(onUserActivated(const QModelIndex &)));
    // connections for search line edit
    connect(ui->searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchRequested()));
    connect(m_SearchToolBut, SIGNAL(triggered(QAction*)), this, SLOT(onSearchToolButtonTriggered(QAction*)));


    connect(UserModel::instance(), SIGNAL(memoryUsageChanged()), this, SLOT(updateStatusBar()));

//    if (Utils::isDebugCompilation())
//        connect(userTableView,SIGNAL(activated(QModelIndex)), this, SLOT(showUserDebugDialog(QModelIndex)));

    // TODO active userTableView on currentUser

    updateStatusBar();
    return true;
}

UserManagerWidget::~UserManagerWidget()
{
    delete ui;
}

bool UserManagerWidget::canCloseParent()
{
    if (UserModel::instance()->hasUserToSave()) {
        int ret = Utils::withButtonsMessageBox(tr("You've modified the users' list."), tr("Do you want to save your changes ?"), "",
                                         QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                         QMessageBox::Save, windowTitle());
        if (ret == QMessageBox::Discard)
            return true;
        else if (ret == QMessageBox::Cancel)
            return false;
        else if (UserModel::instance()->submitAll()) {
            QMessageBox::information(this, windowTitle(), tr("Changes have been correctly saved."));
            return true;
        }
        else {
            QMessageBox::information(this, windowTitle(), tr("Changes can not be correctly saved."));
            return false;
        }
    }
    return true;
}

void UserManagerWidget::analyseCurrentUserRights()
{
    // retreive user manager rights from model
    UserModel *m = UserModel::instance();
    Core::IUser::UserRights r (m->currentUserData(Core::IUser::ManagerRights).toInt());
    // translate to bools
    m_CanModify = (r & Core::IUser::WriteAll);
    m_CanCreate = (r & Core::IUser::Create);
    m_CanViewAllUsers = (r & Core::IUser::ReadAll);
    m_CanDelete = (r & Core::IUser::Delete);

    // manage ui <-> rights
    aCreateUser->setEnabled(m_CanCreate);
    aModifyUser->setEnabled(m_CanModify);
    aDeleteUser->setEnabled(m_CanDelete);

    // manage specific creation widgets
    ui->userTableView->setVisible(m_CanViewAllUsers);
    ui->searchLineEdit->setVisible(m_CanViewAllUsers);
}

/** \brief upadet status bar for current user, and refresh memory usage group */
void UserManagerWidget::updateStatusBar()
{
//    UserModel *m = UserModel::instance();
//    ui->memoryUsageLabel->setText(tr("Database: %1,\nMemory: %2")
//                                     .arg(m->rowCount())
//                                     .arg(m->numberOfUsersInMemory()));
//    if (! m_PermanentWidget) {
//        m_PermanentWidget = new QWidget(m_Parent);
//        QHBoxLayout * l = new QHBoxLayout(m_PermanentWidget);
//        l->setMargin(0);
//        if (!m_PermanentUserName)
//            m_PermanentUserName = new QLabel(m_PermanentWidget);
//        l->addWidget(m_PermanentUserName);
//    }
//    m_PermanentUserName->setText(m->index(m->currentUserIndex().row(), Core::IUser::Name).data().toString());
//    m_Parent->statusBar()->addPermanentWidget(m_PermanentWidget);
}

/** \brief Change the search method for the users's model */
void UserManagerWidget::onSearchToolButtonTriggered(QAction *act)
{
    if (act == searchByNameAct)
        m_SearchBy= Core::IUser::Name;
    else if (act == searchByFirstnameAct)
        m_SearchBy= Core::IUser::Firstname;
    else if (act == searchByNameAndFirstnameAct)
        m_SearchBy= -1;
    else if (act == searchByCityAct)
        m_SearchBy= Core::IUser::City;
}

void UserManagerWidget::onCurrentUserChanged()
{
    selectUserTableView(UserModel::instance()->currentUserIndex().row());
    analyseCurrentUserRights();
    ui->userViewer->changeUserTo(UserModel::instance()->currentUserIndex().row());
}

/**
  \brief Update the users' model filter
  \todo Manage error when user select an action in the toolbutton
  \todo where can only be calculated by model
 */
void UserManagerWidget::onSearchRequested()
{
    QHash<int, QString> where;
    where.insert(m_SearchBy, QString("LIKE '%1%'").arg(ui->searchLineEdit->searchText()));
    UserModel::instance()->setFilter(where);
}

/** \brief Create a new user using UserPlugin::UserWizard. */
void UserManagerWidget::onCreateUserRequested()
{
    int createdRow = ui->userTableView->model()->rowCount();
    if (!ui->userTableView->model()->insertRows(createdRow, 1)) {
        LOG_ERROR("Cannot create new user : can not add row to model");
        return;
    }
    QModelIndex index = ui->userTableView->model()->index(createdRow, USER_NAME);
//    userTableView->model()->setData(index, tr("New User"));
    UserWizard wiz(this);
    /** \todo code here */
//    wiz.setModelRow(createdRow);
    int r = wiz.exec();
    if (r == QDialog::Rejected) {
        if (!ui->userTableView->model()->removeRows(createdRow, 1)) {
            LOG_ERROR("Cannot delete new user : can not delete row to model");
            return;
        } else {
//            m_Parent->statusBar()->showMessage(tr("No user created"), 2000);
        }
    } else {
        ui->userTableView->selectRow(createdRow);
        onUserActivated(index);
//        m_Parent->statusBar()->showMessage(tr("User created"), 2000);
    }
//    qApp->setActiveWindow(m_Parent);
//    m_Parent->activateWindow();
}

void UserManagerWidget::onClearModificationRequested()
{
//    if (UserModel::instance()->revertAll())
//        m_Parent->statusBar()->showMessage(tr("Modifications cleared"), 2000);
//    else
//        m_Parent->statusBar()->showMessage(tr("Can not clear modifications"), 2000);
}

void UserManagerWidget::onSaveRequested()
{
    if ((!m_CanModify) || (!m_CanCreate))
        return;
    // redefine focus
//    m_Parent->statusBar()->setFocus();
    // save changes to database
    if (UserModel::instance()->submitAll()) {
//        m_Parent->statusBar()->showMessage(tr("User saved"), 2000);
    }
}

void UserManagerWidget::onDeleteUserRequested()
{
    if (!ui->userTableView->selectionModel()->hasSelection())
        return;

    // User can not delete himself
    if (ui->userTableView->currentIndex().row() == UserModel::instance()->currentUserIndex().row())
        return;

    if (UserModel::instance()->removeRow(ui->userTableView->currentIndex().row())) {
        LOG(tr("User deleted"));
    } else {
        LOG(tr("User can not be deleted"));
    }
    selectUserTableView(UserModel::instance()->currentUserIndex().row());
}

void UserManagerWidget::toggleSearchView()
{
    QList<int> s = ui->splitter->sizes();
    if (s.count()>=2) {
        if (s.at(0)==0)
            ui->splitter->setSizes(QList<int>() << 1 << 3);
        else
            ui->splitter->setSizes(QList<int>() << 0 << 1);
    }
}

void UserManagerWidget::onUserActivated(const QModelIndex &index)
{
    ui->userViewer->changeUserTo(index.row());
}

void UserManagerWidget::selectUserTableView(int row)
{
//    ui->userTableView->selectRow(row);
    ui->userTableView->setCurrentIndex(ui->userTableView->model()->index(row, 0));
    ui->userViewer->changeUserTo(row);
}

/** \brief Assume retranslation of UI. */
void UserManagerWidget::changeEvent(QEvent *e)
{
    if ((e->type() == QEvent::LanguageChange)) {
        if (ui)
            ui->retranslateUi(this);
//        m_Parent->setWindowTitle(tr("User Manager") + " - " + qApp->applicationName());
    }
    retranslate();
}

/** Retranslate the UI */
void UserManagerWidget::retranslate()
{
    if (!searchByNameAct)
        return;
    searchByNameAct->setText(tr("Search user by name"));
    searchByFirstnameAct->setText(tr("Search user by firstname"));
    searchByNameAndFirstnameAct->setText(tr("Search user by name and firstname"));
    searchByCityAct->setText(tr("Search user by city"));

    searchByNameAct->setToolTip(searchByNameAct->text());
    searchByFirstnameAct->setToolTip(searchByFirstnameAct->text());
    searchByNameAndFirstnameAct->setToolTip(searchByNameAndFirstnameAct->text());
    searchByCityAct->setToolTip(searchByCityAct->text());
    m_SearchToolBut->setToolTip(m_SearchToolBut->text());

    aCreateUser->setText(QCoreApplication::translate(Constants::TR_CONTEXT_USERS, Constants::CREATE_USER));
    aCreateUser->setToolTip(aCreateUser->text());
    aModifyUser->setText(QCoreApplication::translate(Constants::TR_CONTEXT_USERS, Constants::MODIFY_USER));
    aModifyUser->setToolTip(aModifyUser->text());
    aSave->setText(QCoreApplication::translate(Constants::TR_CONTEXT_USERS, Constants::SAVE_USER));
    aSave->setToolTip(aSave->text());
    aRevert->setText(tr("Clear modifications"));
    aRevert->setToolTip(aRevert->text());
    aDeleteUser->setText(QCoreApplication::translate(Constants::TR_CONTEXT_USERS, Constants::DELETE_USER));
    aDeleteUser->setToolTip(aDeleteUser->text());
    aQuit->setText(tr("Quit User Manager"));
    aQuit->setToolTip(aQuit->text());
    aToggleSearchView->setText(tr("Toggle search"));
    aToggleSearchView->setToolTip(aToggleSearchView->text());

//    m_Parent->setWindowTitle(tr("User Manager") + " - " + qApp->applicationName());
//    updateStatusBar();
}

/** For debugging purpose */
void UserManagerWidget::showUserDebugDialog(const QModelIndex &id)
{
    QStringList list;
    list << UserModel::instance()->index(id.row(), Core::IUser::WarnText).data(Qt::DisplayRole).toStringList();
    Utils::quickDebugDialog(list);
}

bool UserManagerWidget::event(QEvent *event)
{
    if (patient()) {
        switch (event->type()) {
        case QEvent::Show:
            patient()->hidePatientBar();
            break;
        case QEvent::Hide:
            patient()->showPatientBar();
            break;
        default:
            break;
        }
    }
    return QWidget::event(event);
}
