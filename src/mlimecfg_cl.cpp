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
#include "mlimecfg_cl.h"

#include <ntqdir.h>
#include <stdlib.h>
#include <ntqfile.h>
#include <ntqtextstream.h>
#include <ntqfileinfo.h>
#include <ntqmessagebox.h>
#include <ntqfiledialog.h>

#include <ntqpushbutton.h>
#include <ntqlistbox.h>
#include <ntqlabel.h>
#include <ntqtabwidget.h>
#include <ntqlineedit.h>
#include <ntqlistbox.h>
#include <ntqtextedit.h>

#include <tdelocale.h>

#define IMEDIR "/etc/ime"

TQString def_ime = "";

int imecount,listitem;//imecount用于记录系统中原有的输入法个数，listitem记录当前添加后输入法的个数。
bool imefilestatus=false;//记录输入法配置文件的更新状态。


mlimecfg_cl::mlimecfg_cl(TQWidget *parent, const char *name)
    :mlimecfg_base(parent, name)
{

	buttonOk->hide();
	buttonCancel->hide();

    connect( listBox, SIGNAL( clicked(TQListBoxItem*) ), this, SLOT( listBox_clicked(TQListBoxItem*) ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOk_clicked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( buttonCancel_clicked() ) );
    connect( savebt, SIGNAL( clicked() ), this, SLOT( savebt_clicked() ) );
    connect( createbt, SIGNAL( clicked() ), this, SLOT( createbt_clicked() ) );
    connect( opencmd, SIGNAL( clicked() ), this, SLOT( opencmd_clicked() ) );
    connect( delbn, SIGNAL( clicked() ), this, SLOT( delbn_clicked() ) );
    connect( addbn, SIGNAL( clicked() ), this, SLOT( addbn_clicked() ) );
    connect( clearbn, SIGNAL( clicked() ), this, SLOT( clearbn_clicked() ) );
    connect( defaultbn, SIGNAL( clicked() ), this, SLOT( defaultbn_clicked() ) );


    //this->setIcon(Icon("input-keyboard"));
    TQDir ime(IMEDIR);
        //检查用户权限，若非 root 则禁用添加删除输入法及保存启动脚本的按钮。
    if (checkroot()){
    if (!ime.exists()){
 //如果目录不存在则创建它。
    TQString tmpcmd;
    tmpcmd="mkdir -p " + TQString(IMEDIR);
    system(tmpcmd); 
     }
}    else{
    addbn->setEnabled(false);
    delbn->setEnabled(false);
    savebt->setEnabled(false);
    
}
    ime.setFilter(TQDir::Files);
    imecount=ime.count();
    listitem=imecount;
    for (int  i=0;i<imecount;i++ ) {
         listBox->insertItem(ime[i]);
    }
    
//    textlabel initialization
    this->setCaption(i18n("Magiconf::IME Configure"));
    textLabel3->setText(i18n("<font color=\"#aa0000\"><b>IME installed in  your system:</b></font>"));
     tabWidget->setTabLabel(Widget2,i18n("IME Configure Table"));
     tabWidget->setTabLabel(Widget3,i18n("xinput Editor"));
    textLabel2->setText(i18n("<font color=\"#ff0000\"><b>Warning:</b>If you want to edit it yourself,\nplease keep it correct!</font>"));
    createbt->setText(i18n("Rebuild"));
    savebt->setText(i18n("Save"));
    infolb->setText(i18n("<font color=\"#ff0000\"><b>Tip:</b></font>Only root can do something for system setting.\nOther users can only change default ime."));
    buttonOk->setText(i18n("OK"));
    buttonCancel->setText(i18n("Cancel"));
    textLabel1->setText(i18n("IME name:"));
    textLabel1_2->setText(i18n("XMODIFIERS parameter(<font color=\"#ff0000\">@im=</font>):"));
    textLabel1_3->setText(i18n("Command:"));
    textLabel1_4->setText(i18n("IME parameter(if have):"));
    textLabel1_5->setText(i18n("GTK_IM_MODULE(optional):"));
    textLabel1_6->setText(i18n("QT_IM_MODULE(optional):"));
    defaultbn->setText(i18n("Set as default"));
    clearbn->setText(i18n("Clean"));
    addbn->setText(i18n("Add"));
    delbn->setText(i18n("Remove"));

    this->adjustSize();
/*    this->setMaximumSize(TQSize(this->width(),this->height()));
    this->setMinimumSize(TQSize(this->width(),this->height()));*/

    //显示默认输入法
    showdefaultime();
    readxinput();
    auto_choose();
}

void mlimecfg_cl::addbn_clicked()
{
    saveime();
    if (imefilestatus)
    clearle();
    imefilestatus=false;
}

void mlimecfg_cl::clearle()
{
    //添加新的输入法后，清除已输入的内容。
    namele->clear();
    ximle->clear();
    programle->clear();
    argle->clear();
    gtkle->clear();
    qtle->clear();
}

void mlimecfg_cl::saveime()
{
    //检查输入的有效性。
    if (ximle->text().length()==0){
 TQMessageBox::warning( 0, i18n("Error"), i18n("no XMODIFIERS") );
 ximle->setFocus();
    }
    else if(programle->text().length()==0){
 TQMessageBox::warning( 0,i18n( "Error"), i18n("no Command") );
  programle->setFocus();
    }
    else {
 TQString imename=programle->text();
            int nameplace=imename.findRev('/');
            imename=imename.right(imename.length()-nameplace-1);
 //设置配置文件的全路径。
 TQString filename=TQString(IMEDIR)+ "/" + imename;
  //检查是否重复，如果重复则修改已存在的。
            if ( !TQFile::exists( filename ) ){
                    listBox->insertItem(imename);
                    //listBox->setCurrentItem(listitem);
                    listitem=listBox->count();  
         imefilestatus=true;
                 }
 //写入相应的配置文件。
            TQFile f(filename);
     if(!f.open(IO_WriteOnly)) {
  TQMessageBox::warning( 0,i18n( "Error!"), i18n("Eorror while writing config file,please check")+ TQString(IMEDIR) +i18n("\nIf you have permission to access the file!") );
     listBox->removeItem(listitem);
                listitem=listBox->count();  
     }
     else {
            TQTextStream t(&f);
            t<<"NAME=" + namele->text()<<endl;
            t<<"XIM=" + ximle->text()<<endl;
            t<<"PROGRAM=" + programle->text()<<endl;
            t<<"ARG=" + argle->text()<<endl;
            t<<"IMMGTK=" + gtkle->text()<<endl;
            t<<"IMMQT=" + qtle->text()<<endl;
            f.close();
 }
   }
}


void mlimecfg_cl::delbn_clicked()
{
    if (listBox->currentText().length() >0){
 TQString tmpcmd="rm -rf " + TQString(IMEDIR) + "/" +listBox->currentText();
            system(tmpcmd);
//            namele->setText(tmpcmd);//临时设置，测试命令执行情况
    }
    listBox->removeItem(listBox->currentItem());
}


void mlimecfg_cl::opencmd_clicked()
{
    //设置一个打开文件框来选择文件，可以减少命令的错误机率。
  TQString s = TQFileDialog::getOpenFileName("/usr/bin");
  if (!s.isEmpty())
      programle->setText(s);
}



void mlimecfg_cl::listBox_clicked( TQListBoxItem * )
{
   //点击列表框中的项目则读取相应的配置文件。
    TQFile r(TQString(IMEDIR) + "/" + listBox->currentText());
 if(r.open(IO_ReadOnly)) {
  TQString line;
  while(!r.atEnd()) {
   r.readLine(line, 200);
   if(line.left(5)=="NAME=") {
    namele->setText(TQString::fromLocal8Bit(line.mid(5).simplifyWhiteSpace()));
   } else if(line.left(4)=="XIM=") {
              ximle->setText(TQString::fromLocal8Bit(line.mid(4).simplifyWhiteSpace()));
   } else if(line.left(8)=="PROGRAM=") {
              programle->setText(TQString::fromLocal8Bit(line.mid(8).simplifyWhiteSpace()));
   } else if(line.left(4)=="ARG=") { 
       argle->setText(TQString::fromLocal8Bit(line.mid(4).simplifyWhiteSpace()));   
   }  else if(line.left(7)=="IMMGTK=") { 
       gtkle->setText(TQString::fromLocal8Bit(line.mid(7).simplifyWhiteSpace()));   
   }  else if(line.left(6)=="IMMQT=") { 
       qtle->setText(TQString::fromLocal8Bit(line.mid(6).simplifyWhiteSpace()));   
   } 
  }
  r.close();
            }
            else  {
   TQMessageBox::warning( 0,i18n( "Error!"), i18n("Error while reading config file") + TQString(IMEDIR) + "/" + listBox->currentText() +i18n(" Failed!") ); 
 }
}


void mlimecfg_cl::clearbn_clicked()
{
clearle();
}


void mlimecfg_cl::defaultbn_clicked()
{
    //设置默认输入法
    TQString homePath = TQDir::homeDirPath();
    TQString tmpcmd="echo "+ listBox->currentText() + ">"+homePath+"/.ime";
    system( tmpcmd) ;
    showdefaultime();
}


void mlimecfg_cl::readxinput()
{
    //读入输入法启动脚本。
 TQFile r("/etc/X11/xinit/xinitrc.d/xinput.sh");
 if(r.open(IO_ReadOnly)) {
  TQString line;
  int i=0;
  while(!r.atEnd()) {
   r.readLine(line, 1024);
   if (i==0)
       xinputte->setText(TQString::fromLocal8Bit(line));
   else
       xinputte->setText(xinputte->text() + TQString::fromLocal8Bit(line));
   i++;
}
  r.close();
}
 else
      TQMessageBox::warning( 0,i18n("Error"), i18n("Error while reading /etc/X11/xinit/xinitrc.d/xinput.sh. Failed!") ); 
}


void mlimecfg_cl::savebt_clicked()
{
//保存修改过的输入法启动脚本
    TQFile w("/etc/X11/xinit/xinitrc.d/xinput.sh");
     if(!w.open(IO_WriteOnly)) {
  TQMessageBox::warning( 0,i18n( "Error!"), i18n("Error while writing config file /etc/X11/xinit/xinitrc.d/xinput.sh.\nPleae check if you have permission to access the file!") );
     }
     else {
            TQTextStream t(&w);
            t<<xinputte->text()<<endl;
            w.close();
 TQString tmpcmd="chmod +x /etc/X11/xinit/xinitrc.d/xinput.sh";
 system(tmpcmd);
      }
}


void mlimecfg_cl::createbt_clicked()
{
    TQString XIM_IME,ARG_IME,imeselect,imename,imefind,IMMGTK,IMMQT,extra_ime;
    
    TQString xinput1="#!/bin/sh\n#\n# XIM( X Input Method ) script for Magic Linux\n# by KanKer <kanker@163.com>\n#\n#This software may be freely redistributed under \n#the terms of the GNU public license.\n#\n#Check $XIM and set a default $XIM value.\n#\n\nif [ -f /etc/sysconfig/i18n ]; then\n       . /etc/sysconfig/i18n\n       export LANG\nfi\n\nif [ -f ~/.ime ]; then\n   tmpime=$(<~/.ime)\n   if which $tmpime >/dev/null 2>&1 ; then\n       XIM=$tmpime\n       tmpime=true\n   fi\nfi\n\nif [ -z \"$XIM\" ]; then\n";
TQString xinput2="   else\n         XIM=\"none\"\n   fi\n\nfi\n\n#\n# set a proper XIM program with respect to $XIM\n#\ncase $XIM in\n";
TQString xinput3="none)\n         echo \"Can't find available x input method!\" \n         XIM_PROGRAM=true ;;\nesac\n\nif [ ! -f ~/.ime ] || [ $tmpime != true ] ; then\n    [ $XIM_PROGRAM = true ] || echo $XIM_PROGRAM >~/.ime\n    unset tmpime\nfi\n[ -n \"$XIM\" ] && export XMODIFIERS=\"@im=$XIM\"\n\n# execute XIM_PROGRAM\n\nwhich $XIM_PROGRAM > /dev/null 2>&1 && $XIM_PROGRAM $XIM_ARGS &\n";
if (listitem>0){
    for (int i=0;i<listitem;i++){
 imename=listBox->text(i);
 //查看输入法是否有效，并在无默认输入法的情况下选择一个。
        if (i==0)
 imefind="   if which " + imename + " >/dev/null 2>&1 ; then\n         XIM=\"" + imename + "\"\n";
        else
 imefind=imefind + "   elif which " + imename + " >/dev/null 2>&1 ; then\n         XIM=\"" + imename + "\"\n";
 
//此处读取各输入法配置文件的内容
 TQFile r(TQString(IMEDIR) + "/" + imename);
 if(r.open(IO_ReadOnly)) {
  TQString line;
  while(!r.atEnd()) {
   r.readLine(line, 200);
  if(line.left(4)=="XIM=") {
      XIM_IME=TQString::fromLocal8Bit(line.mid(4).simplifyWhiteSpace());
   } else if(line.left(4)=="ARG=") { 
       ARG_IME=TQString::fromLocal8Bit(line.mid(4).simplifyWhiteSpace());
   } else if(line.left(7)=="IMMGTK=") { 
       IMMGTK=TQString::fromLocal8Bit(line.mid(7).simplifyWhiteSpace());
   } else if(line.left(6)=="IMMQT=") { 
       IMMQT=TQString::fromLocal8Bit(line.mid(6).simplifyWhiteSpace());
   } 
  }
  r.close();
}
 else{
     TQMessageBox::warning( 0,i18n( "Error!"), i18n("I can't read file ") + TQString(IMEDIR) + "/" + imename ) ;
 }
  //结束
  
 //输入法选择语句，首先检查GTK_IM_MODULE和QT_IM_MODULE是否设置。
  if (IMMGTK.length()>0)
     extra_ime="        export GTK_IM_MODULE=" + IMMGTK + "\n";
 if (IMMQT.length()>0)
     extra_ime=extra_ime + "        export QT_IM_MODULE=" + IMMQT + "\n";
 if (extra_ime.length()>0)
    imeselect=imeselect+ imename + ")\n        XIM=" + XIM_IME + "\n        XIM_PROGRAM=" + imename + "\n        XIM_ARGS=\"" + ARG_IME + "\"\n" + extra_ime + ";;\n";
 else
    imeselect=imeselect+ imename + ")\n        XIM=" + XIM_IME + "\n        XIM_PROGRAM=" + imename + "\n        XIM_ARGS=\"" + ARG_IME + "\"\n;;\n";
}
xinputte->setText(xinput1 + imefind + xinput2 + imeselect + xinput3);
}
else
     TQMessageBox::warning( 0,i18n( "Error!"), i18n(("I can't find any ime in your system!") ));
}

bool mlimecfg_cl::checkroot()
{
if ( getuid() ) {
             return false;
 }
else
    return true;
}


void mlimecfg_cl::buttonOk_clicked()
{
if (checkroot())
savebt_clicked();
//exit(0);
    emit cancel_clked();
}

void mlimecfg_cl::buttonCancel_clicked()
{
    emit cancel_clked();
}

void mlimecfg_cl::showdefaultime()
{
//检测是否已设置默认输入法并显示出来
    TQString homePath = TQDir::homeDirPath();
    TQFile r(homePath  + "/.ime");
    if (r.exists())
	 if(r.open(IO_ReadOnly)) {
	      TQString line;
	      r.readLine(line, 200);
	      r.close();
	      def_ime = line.simplifyWhiteSpace();
	      defaultlb->setText(i18n("Default IME of current user : <b>") +
				 def_ime + 
				 "</b>");
	  }
    else
	defaultlb->setText(i18n("Default IME of current user : <b>Nothing</b>"));
   
}

void mlimecfg_cl::auto_choose()
{
    //void TQListBox::setCurrentItem ( TQListBoxItem * i ) [virtual] 
    TQListBoxItem *l_b_i = listBox->findItem(def_ime,0);
    if(l_b_i!=NULL){
	listBox->setCurrentItem(l_b_i);
	listBox_clicked(l_b_i);
    }
}
#include "mlimecfg_cl.moc"
