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
#ifndef CONSTANTTRANSLATIONS_USER_H
#define CONSTANTTRANSLATIONS_USER_H

#include <QCoreApplication>

namespace Trans {
namespace Constants {

// Users related message
const char* const WELCOME_USER                      = QT_TRANSLATE_NOOP("tkConstants", "Welcome to FreeMedForms, you are now connected.");
const char* const CONNECTED_AS_1                    = QT_TRANSLATE_NOOP("tkConstants", "Connected as\n  %1");
const char* const NO_USER_CONNECTED                 = QT_TRANSLATE_NOOP("tkConstants", "No user connected.");
const char* const USER                              = QT_TRANSLATE_NOOP("tkConstants", "User");
const char* const USERS                             = QT_TRANSLATE_NOOP("tkConstants", "Users");
const char* const USER_DATAS                        = QT_TRANSLATE_NOOP("tkConstants", "User datas");
const char* const RIGHTS                            = QT_TRANSLATE_NOOP("tkConstants", "Rights");
const char* const USER_RIGHTS                       = QT_TRANSLATE_NOOP("tkConstants", "User rights");
const char* const RIGHTS_MEDICAL                    = QT_TRANSLATE_NOOP("tkConstants", "Medical rights");
const char* const RIGHTS_ADMINISTRATIVE             = QT_TRANSLATE_NOOP("tkConstants", "Administrative rights");
const char* const RIGHTS_USERMANAGER                = QT_TRANSLATE_NOOP("tkConstants", "User manager rights");
const char* const RIGHTS_DRUGS                      = QT_TRANSLATE_NOOP("tkConstants", "Drug rights");

const char* const TELEPHONE                         = QT_TRANSLATE_NOOP("tkConstants", "Telephone");
const char* const FAX                               = QT_TRANSLATE_NOOP("tkConstants", "Fax");
const char* const MAIL                              = QT_TRANSLATE_NOOP("tkConstants", "E-Mail");

const char* const SAVE_AND_RESTORE                  = QT_TRANSLATE_NOOP("tkConstants", "Save and restore");


} // end Constants
}  // end Trans

#endif // CONSTANTTRANSLATIONS_USER_H
