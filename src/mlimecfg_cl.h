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
#ifndef MLIMECFG_CL_H
#define MLIMECFG_CL_H

#include "mlimecfg_base.h"

class mlimecfg_cl: public mlimecfg_base {
Q_OBJECT
public:
    mlimecfg_cl(TQWidget *parent = 0, const char *name = 0);
    virtual bool checkroot();

public slots:
    virtual void addbn_clicked();
    virtual void clearle();
    virtual void saveime();
    virtual void delbn_clicked();
    virtual void opencmd_clicked();
    virtual void listBox_clicked( TQListBoxItem * );
    virtual void clearbn_clicked();
    virtual void defaultbn_clicked();
    virtual void readxinput();
    virtual void savebt_clicked();
    virtual void createbt_clicked();
    virtual void buttonOk_clicked();
    virtual void buttonCancel_clicked();
    virtual void showdefaultime();
    virtual void auto_choose();

signals:
    void cancel_clked();


};

#endif
