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


#ifndef _MLIMECFG_H_
#define _MLIMECFG_H_

#include <tdecmodule.h>
#include <tdeaboutdata.h>
#include "mlimecfg_cl.h"

class mlimecfg: public TDECModule
{
    Q_OBJECT

public:
    mlimecfg( TQWidget *parent=0, const char *name=0, const TQStringList& = TQStringList() );
    ~mlimecfg();

    virtual void load();
    virtual void save();
    virtual void defaults();
    virtual int buttons();
    virtual TQString quickHelp() const;
    virtual const TDEAboutData *aboutData()const
    { return myAboutData; };

public slots:
    void configChanged();

private:

	mlimecfg_cl *micl;
	TDEAboutData *myAboutData;

};

#endif
