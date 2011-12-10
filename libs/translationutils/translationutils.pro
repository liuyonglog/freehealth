# toolkit library project file
TEMPLATE        = lib
TARGET          = TranslationUtils

DEFINES *= TRANSLATIONUTILS_LIBRARY

# include config file
include( ../libsworkbench.pri )

QT *= network

HEADERS += \
    translationutils_exporter.h \
    constanttranslations.h \
    multilingualclasstemplate.h \
    googletranslator.h \
    trans_account.h \
    trans_agenda.h \
    trans_countries.h \
    trans_current.h \
    trans_database.h \
    trans_datetime.h \
    trans_drugs.h \
    trans_editor.h \
    trans_filepathxml.h \
    trans_menu.h \
    trans_msgerror.h \
    trans_patient.h \
    trans_spashandupdate.h \
    trans_team.h \
    trans_titles.h \
    trans_user.h

SOURCES += constanttranslations.cpp \
    googletranslator.cpp

# translators
TRANSLATIONS += $${SOURCES_TRANSLATIONS_PATH}/translationutils_fr.ts \
                $${SOURCES_TRANSLATIONS_PATH}/translationutils_de.ts \
                $${SOURCES_TRANSLATIONS_PATH}/translationutils_es.ts
