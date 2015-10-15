/***************************************************************************
 *   Copyright (C) 2008 by lovewilliam   *
 *   lovewilliam@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <ntqlayout.h>

#include <tdelocale.h>
#include <tdeglobal.h>
#include <tdeparts/genericfactory.h>

#include <kdebug.h>

#include "mlimecfg.h"

static const char description[] =
    I18N_NOOP("This module help you to manage your input methods");

static const char version[] = "1.0.0";

typedef KGenericFactory<mlimecfg, TQWidget> mlimecfgFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_mlimecfg, mlimecfgFactory("mlimecfg"))

mlimecfg::mlimecfg(TQWidget *parent, const char *name, const TQStringList&)
    : TDECModule(parent, name), myAboutData(0)
{

    TDEAboutData *about =
    new TDEAboutData(I18N_NOOP("mlimecfg"), I18N_NOOP("Magic Linux IME Module"),
                  version, description, TDEAboutData::License_GPL,
                  I18N_NOOP("(c) 2005-2008 lovewilliam"));

	about->addAuthor("KanKer",0,"kanker@163.com");
	about->addAuthor("lovewilliam", 0, "lovewilliam@gmail.com");

	setAboutData( about );

    // place widgets here
	load();
};


mlimecfg::~mlimecfg()
{
}


void mlimecfg::load()
{
	micl = new mlimecfg_cl(this,i18n("Input Method"));

	TQGridLayout *grid = new TQGridLayout( this, 1, 1 );
	grid->addWidget( micl,   0, 0 );

	micl->show();
	micl->adjustSize();

}


void mlimecfg::defaults()
{
    // insert your default settings code here...
    emit changed(true);
}


void mlimecfg::save()
{
    // insert your saving code here...
	micl->buttonOk_clicked();
	emit changed(false);
}


int mlimecfg::buttons()
{
	return TDECModule::Default|TDECModule::Apply|TDECModule::Help;

}


void mlimecfg::configChanged()
{
    // insert your saving code here...
    emit changed(true);
}


TQString mlimecfg::quickHelp() const
{
    return description;
}


#include "mlimecfg.moc"
