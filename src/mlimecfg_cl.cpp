#include "mlimecfg_cl.h"

#include <stdlib.h>
#include <unistd.h>

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <KLocale>

#define IMEDIR "/etc/ime"

#define DEFAULT_IME "fcitx"

MLImeCfg_CL::MLImeCfg_CL( QWidget* parent )
    : QWidget( parent ), imefilestatus( false )
{
    setupUI();

    buttonOk->hide();
    buttonCancel->hide();

    connect( listBox, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( listBox_clicked( QListWidgetItem* ) ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOk_clicked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( buttonCancel_clicked() ) );
    connect( savebt, SIGNAL( clicked() ), this, SLOT( savebt_clicked() ) );
    connect( createbt, SIGNAL( clicked() ), this, SLOT( createbt_clicked() ) );
    connect( opencmd, SIGNAL( clicked() ), this, SLOT( opencmd_clicked() ) );
    connect( delbn, SIGNAL( clicked() ), this, SLOT( delbn_clicked() ) );
    connect( addbn, SIGNAL( clicked() ), this, SLOT( addbn_clicked() ) );
    connect( clearbn, SIGNAL( clicked() ), this, SLOT( clearbn_clicked() ) );
    connect( defaultbn, SIGNAL( clicked() ), this, SLOT( defaultbn_clicked() ) );

    QDir ime( IMEDIR );
    //检查用户权限，若非 root 则禁用添加删除输入法及保存启动脚本的按钮
    if ( checkroot() )
    {
        if ( !ime.exists() )
        {
            //如果目录不存在则创建它。
            QString tmpcmd;
            tmpcmd = "mkdir -p " + QString( IMEDIR );
            system( qPrintable( tmpcmd ) );
        }
    }
    else
    {
        addbn->setEnabled( false );
        delbn->setEnabled( false );
        savebt->setEnabled( false );
    }

    ime.setFilter( QDir::Files );
    imecount = ime.count();
    listitem = imecount;
    for ( int i=0; i<imecount; ++i )
         listBox->addItem( ime[i] );

    //显示默认输入法
    showdefaultime();
    readxinput();
    auto_choose();
}

void MLImeCfg_CL::addbn_clicked()
{
    saveime();
    if ( imefilestatus )
        clearle();

    imefilestatus = false;
}

void MLImeCfg_CL::clearle()
{
    //添加新的输入法后，清除已输入的内容
    namele->clear();
    ximle->clear();
    programle->clear();
    argle->clear();
    gtkle->clear();
    qtle->clear();
}

void MLImeCfg_CL::saveime()
{
    //检查输入的有效性
    if ( ximle->text().length()==0 )
    {
        QMessageBox::warning( 0, i18n( "Error" ), i18n( "no XMODIFIERS" ) );
        ximle->setFocus();
    }
    else if ( programle->text().length()==0 )
    {
        QMessageBox::warning( 0, i18n( "Error" ), i18n( "no Command" ) );
        programle->setFocus();
    }
    else
    {
        QString imename = programle->text();
        int nameplace = imename.lastIndexOf( '/' );
        imename = imename.right( imename.length() - nameplace - 1 );
        //设置配置文件的全路径。
        QString filename = QString( IMEDIR )+ "/" + imename;
        //检查是否重复，如果重复则修改已存在的。
        if ( !QFile::exists( filename ) )
        {
            listBox->addItem(imename);
            //listBox->setCurrentItem(listitem);
            listitem = listBox->count();
            imefilestatus = true;
        }
        //写入相应的配置文件
        QFile f( filename );
        if( !f.open( QIODevice::WriteOnly ) )
        {
            QMessageBox::warning( 0, i18n( "Error!" ), i18n( "Error while writing config file, please check" )+ QString( IMEDIR ) + i18n( "\nIf you have permission to access the file!" ) );
            listBox->removeItemWidget( listBox->item( listitem ) );
            listitem = listBox->count();
        }
        else
        {
            QTextStream t( &f );
            t << "NAME=" + namele->text() << endl;
            t << "XIM=" + ximle->text() << endl;
            t << "PROGRAM=" + programle->text() << endl;
            t << "ARG=" + argle->text() << endl;
            t << "IMMGTK=" + gtkle->text() << endl;
            t << "IMMQT=" + qtle->text() << endl;
            f.close();
        }
    }
}

void MLImeCfg_CL::delbn_clicked()
{
    if ( listBox->currentItem()->text().length() > 0 )
    {
        QString tmpcmd = "rm -rf " + QString( IMEDIR ) + "/" + listBox->currentItem()->text();
        system( qPrintable( tmpcmd ) );
//            namele->setText(tmpcmd);//临时设置，测试命令执行情况
    }
    listBox->removeItemWidget( listBox->currentItem() );
}

void MLImeCfg_CL::opencmd_clicked()
{
    //设置一个打开文件框来选择文件，可以减少命令的错误机率
    QString s = QFileDialog::getOpenFileName( this, i18n( "Browse File" ), "/usr/bin" );
    if ( !s.isEmpty() )
      programle->setText( s );
}

void MLImeCfg_CL::listBox_clicked( QListWidgetItem* )
{
    //点击列表框中的项目则读取相应的配置文件
    QFile r( QString( IMEDIR ) + "/" + listBox->currentItem()->text() );
    if ( r.open( QIODevice::ReadOnly ) )
    {
        QString line;
        while ( !r.atEnd() )
        {
            line = QString::fromLocal8Bit( r.readLine( 200 ) );
            if ( line.left(5) == "NAME=" )
                namele->setText( line.mid(5).simplified() );
            else if ( line.left(4) == "XIM=" )
                ximle->setText( line.mid(4).simplified() );
            else if ( line.left(8) == "PROGRAM=" )
                programle->setText( line.mid(8).simplified() );
            else if ( line.left(4) == "ARG=" )
                argle->setText( line.mid(4).simplified() );
            else if ( line.left(7) == "IMMGTK=" )
                gtkle->setText( line.mid(7).simplified() );
            else if ( line.left(6) == "IMMQT=" )
                qtle->setText( line.mid(6).simplified() );
        }
        r.close();
    }
    else
        QMessageBox::warning( 0, i18n( "Error!" ), i18n( "Error while reading config file" ) + QString( IMEDIR ) + "/" + listBox->currentItem()->text() + i18n( " Failed!" ) );
}

void MLImeCfg_CL::clearbn_clicked()
{
    clearle();
}

void MLImeCfg_CL::defaultbn_clicked()
{
    //设置默认输入法
    QString homePath = QDir::homePath();
    QString tmpcmd = "echo "+ listBox->currentItem()->text() + ">" + homePath + "/.ime";
    system( qPrintable( tmpcmd ) ) ;
    showdefaultime();
}

void MLImeCfg_CL::readxinput()
{
    //读入输入法启动脚本
    QFile r( "/etc/X11/xinit/xinitrc.d/xinput.sh" );
    if ( r.open( QIODevice::ReadOnly ) )
    {
        QString line;
        int i = 0;
        while ( !r.atEnd() ) {
            line = QString::fromLocal8Bit( r.readLine( 1024 ) );
            if ( i==0 )
                xinputte->setText( line );
            else
                xinputte->setText( xinputte->toPlainText() + line );
            i++;
        }
        r.close();
    }
    else
        QMessageBox::warning( 0, i18n( "Error" ), i18n( "Error while reading /etc/X11/xinit/xinitrc.d/xinput.sh. Failed!" ) );
}

void MLImeCfg_CL::savebt_clicked()
{
    //保存修改过的输入法启动脚本
    QFile w( "/etc/X11/xinit/xinitrc.d/xinput.sh" );
    if ( !w.open( QIODevice::WriteOnly ) )
        QMessageBox::warning( 0, i18n( "Error!" ), i18n( "Error while writing config file /etc/X11/xinit/xinitrc.d/xinput.sh.\nPleae check if you have permission to access the file!" ) );
    else
    {
        QTextStream t( &w );
        t << xinputte->toPlainText() << endl;
        w.close();
        QString tmpcmd = "chmod +x /etc/X11/xinit/xinitrc.d/xinput.sh";
        system( qPrintable( tmpcmd ) );
    }
}

void MLImeCfg_CL::createbt_clicked()
{
    QString XIM_IME, ARG_IME, imeselect, imename, imefind, IMMGTK, IMMQT, extra_ime;

    QString xinput1 = "#!/bin/sh\n#\n# XIM( X Input Method ) script for Magic Linux\n# by KanKer <kanker@163.com>\n#\n#This software may be freely redistributed under \n#the terms of the GNU public license.\n#\n#Check $XIM and set a default $XIM value.\n#\n\nif [ -f /etc/sysconfig/i18n ]; then\n       . /etc/sysconfig/i18n\n       export LANG\nfi\n\nif [ -f ~/.ime ]; then\n   tmpime=$(<~/.ime)\n   if which $tmpime >/dev/null 2>&1 ; then\n       XIM=$tmpime\n       tmpime=true\n   fi\nfi\n\nif [ -z \"$XIM\" ]; then\n";
    QString xinput2 = "   else\n         XIM=\"none\"\n   fi\n\nfi\n\n#\n# set a proper XIM program with respect to $XIM\n#\ncase $XIM in\n";
    QString xinput3 = "none)\n         echo \"Can't find available x input method!\" \n         XIM_PROGRAM=true ;;\nesac\n\nif [ ! -f ~/.ime ] || [ $tmpime != true ] ; then\n    [ $XIM_PROGRAM = true ] || echo $XIM_PROGRAM >~/.ime\n    unset tmpime\nfi\n[ -n \"$XIM\" ] && export XMODIFIERS=\"@im=$XIM\"\n\n# execute XIM_PROGRAM\n\nwhich $XIM_PROGRAM > /dev/null 2>&1 && $XIM_PROGRAM $XIM_ARGS &\n";
    if ( listitem > 0 )
    {
        for ( int i=0; i<listitem; ++i )
        {
            imename = listBox->item( i )->text();
            //查看输入法是否有效，并在无默认输入法的情况下选择一个
            if ( i == 0 )
                imefind = "   if which " + imename + " >/dev/null 2>&1 ; then\n         XIM=\"" + imename + "\"\n";
            else
                imefind = imefind + "   elif which " + imename + " >/dev/null 2>&1 ; then\n         XIM=\"" + imename + "\"\n";

            //此处读取各输入法配置文件的内容
            QFile r( QString( IMEDIR ) + "/" + imename );
            if ( r.open( QIODevice::ReadOnly ) )
            {
                QString line;
                while ( !r.atEnd() )
                {
                    line = QString::fromLocal8Bit( r.readLine( 200 ) );
                    if ( line.left(4) == "XIM=" )
                        XIM_IME = line.mid(4).simplified();
                    else if ( line.left(4) == "ARG=" )
                        ARG_IME = line.mid(4).simplified();
                    else if ( line.left(7) == "IMMGTK=" )
                        IMMGTK = line.mid(7).simplified();
                    else if ( line.left(6) == "IMMQT=" )
                        IMMQT = line.mid(6).simplified();
                }
                r.close();
            }
            else
                QMessageBox::warning( 0, i18n( "Error!" ), i18n( "I can't read file " ) + QString( IMEDIR ) + "/" + imename ) ;
            //结束

            //输入法选择语句，首先检查 GTK_IM_MODULE 和 QT_IM_MODULE 是否设置
            if ( IMMGTK.length() > 0 )
                extra_ime = "        export GTK_IM_MODULE=" + IMMGTK + "\n";
            if ( IMMQT.length() > 0 )
                extra_ime = extra_ime + "        export QT_IM_MODULE=" + IMMQT + "\n";
            if ( extra_ime.length() > 0 )
                imeselect = imeselect+ imename + " )\n        XIM=" + XIM_IME + "\n        XIM_PROGRAM=" + imename + "\n        XIM_ARGS=\"" + ARG_IME + "\"\n" + extra_ime + ";;\n";
            else
                imeselect = imeselect+ imename + " )\n        XIM=" + XIM_IME + "\n        XIM_PROGRAM=" + imename + "\n        XIM_ARGS=\"" + ARG_IME + "\"\n;;\n";
        }
        xinputte->setText( xinput1 + imefind + xinput2 + imeselect + xinput3 );
    }
    else
        QMessageBox::warning( 0, i18n( "Error!" ), i18n(( "I can't find any ime in your system!" ) ));
}

bool MLImeCfg_CL::checkroot()
{
    if ( getuid() )
        return false;
    return true;
}

void MLImeCfg_CL::buttonOk_clicked()
{
    if ( checkroot() )
        savebt_clicked();
    //exit(0);
    emit cancel_clked();
}

void MLImeCfg_CL::buttonCancel_clicked()
{
    emit cancel_clked();
}

void MLImeCfg_CL::showdefaultime()
{
    //检测是否已设置默认输入法并显示出来
    QString homePath = QDir::homePath();
    QFile r( homePath  + "/.ime" );
    if ( r.exists() )
    {
	 if ( r.open( QIODevice::ReadOnly ) )
         {
	      QString line;
              line = QString::fromLocal8Bit( r.readLine( 200 ) );
	      r.close();
	      defaultlb->setText( i18n( "Default IME of current user : <b>" ) + line.simplified() + "</b>" );
              return;
	  }
    }

    defaultlb->setText( i18n( "Default IME of current user : <b>Nothing</b>" ) );
}

void MLImeCfg_CL::auto_choose()
{
    QString def_ime = DEFAULT_IME;
    QList<QListWidgetItem* > items = listBox->findItems( def_ime, 0 );
    if ( items.isEmpty() )
        return;

    QListWidgetItem* l_b_i = items.first();
    if ( l_b_i != NULL )
    {
	listBox->setCurrentItem( l_b_i );
	listBox_clicked( l_b_i );
    }
}

void MLImeCfg_CL::setupUI()
{
    QGridLayout* gridLayout = new QGridLayout( this );
    gridLayout->setSpacing( 6 );
    gridLayout->setMargin( 11 );
    tabWidget = new QTabWidget( this );
    Widget2 = new QWidget();
    QGridLayout* gridLayout1 = new QGridLayout( Widget2 );
    gridLayout1->setSpacing( 6 );
    gridLayout1->setMargin( 11 );

    defaultbn = new QPushButton( i18n( "Set as default" ), Widget2 );
    gridLayout1->addWidget( defaultbn, 6, 0, 1, 1 );

    clearbn = new QPushButton( i18n( "Clean" ), Widget2 );
    gridLayout1->addWidget( clearbn, 6, 1, 1, 2 );

    addbn = new QPushButton( i18n( "Add" ), Widget2 );
    gridLayout1->addWidget( addbn, 6, 3, 1, 1 );

    delbn = new QPushButton( i18n( "Remove" ), Widget2 );
    gridLayout1->addWidget( delbn, 6, 4, 1, 2 );

    splitter1 = new QSplitter( Widget2 );
    splitter1->setOrientation( Qt::Vertical );
    namele = new QLineEdit( splitter1 );
    splitter1->addWidget( namele );
    ximle = new QLineEdit( splitter1 );
    splitter1->addWidget( ximle );
    programle = new QLineEdit( splitter1 );
    splitter1->addWidget( programle );
    argle = new QLineEdit( splitter1 );
    splitter1->addWidget( argle );
    gtkle = new QLineEdit( splitter1 );
    splitter1->addWidget( gtkle );
    qtle = new QLineEdit( splitter1 );
    splitter1->addWidget( qtle );
    gridLayout1->addWidget( splitter1, 0, 2, 6, 3 );

    opencmd = new QPushButton( i18n( "Browse..." ), Widget2 );
    gridLayout1->addWidget( opencmd, 2, 5, 1, 1 );

    textLabel1 = new QLabel( i18n( "IME name:" ), Widget2 );
    gridLayout1->addWidget( textLabel1, 0, 0, 1, 2 );

    textLabel1_2 = new QLabel( i18n( "XMODIFIERS parameter(<font color=\"#ff0000\">@im=</font>):" ), Widget2 );
    gridLayout1->addWidget( textLabel1_2, 1, 0, 1, 2 );

    textLabel1_3 = new QLabel( i18n( "Command:" ), Widget2 );
    gridLayout1->addWidget( textLabel1_3, 2, 0, 1, 2 );

    textLabel1_4 = new QLabel( i18n( "IME parameter(if have):" ), Widget2 );
    gridLayout1->addWidget( textLabel1_4, 3, 0, 1, 2 );

    textLabel1_5 = new QLabel( Widget2 );
    textLabel1_5->setText( i18n( "GTK_IM_MODULE(optional):" ) );
    gridLayout1->addWidget( textLabel1_5, 4, 0, 1, 2 );

    textLabel1_6 = new QLabel( i18n( "QT_IM_MODULE(optional):" ), Widget2 );
    gridLayout1->addWidget( textLabel1_6, 5, 0, 1, 2 );

    tabWidget->addTab( Widget2, i18n( "&IME Configure Table" ) );
    Widget3 = new QWidget();
    QGridLayout* gridLayout2 = new QGridLayout( Widget3 );
    gridLayout2->setSpacing( 6 );
    gridLayout2->setMargin( 11 );
    xinputte = new QTextEdit( Widget3 );
    gridLayout2->addWidget( xinputte, 0, 0, 1, 3 );

    createbt = new QPushButton( i18n( "Rebuild" ), Widget3 );
    gridLayout2->addWidget( createbt, 1, 1, 1, 1 );

    savebt = new QPushButton( i18n( "Save" ), Widget3 );
    gridLayout2->addWidget( savebt, 1, 2, 1, 1 );

    textLabel2 = new QLabel( i18n( "<font color=\"#ff0000\"><b>Warning:</b>If you want to edit it yourself,\nplease keep it correct!</font>" ), Widget3 );
    gridLayout2->addWidget( textLabel2, 1, 0, 1, 1 );

    tabWidget->addTab( Widget3, i18n( "&xinput Editor" ) );

    gridLayout->addWidget( tabWidget, 0, 1, 1, 1 );

    QVBoxLayout* vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing( 6 );
    textLabel3 = new QLabel( i18n( "<font color=\"#aa0000\"><b>IME installed in  your system:</b></font>" ), this );
    vboxLayout->addWidget( textLabel3 );

    listBox = new QListWidget( this );
    QSizePolicy sizePolicy( static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(7) );
    sizePolicy.setHorizontalStretch( 0 );
    sizePolicy.setVerticalStretch( 0 );
    sizePolicy.setHeightForWidth( listBox->sizePolicy().hasHeightForWidth() );
    listBox->setSizePolicy( sizePolicy );
    listBox->setFrameShape( QFrame::StyledPanel );
    listBox->setFrameShadow( QFrame::Sunken );
    vboxLayout->addWidget( listBox );

    defaultlb = new QLabel( this );
    defaultlb->setAlignment( Qt::AlignVCenter );
    defaultlb->setWordWrap( true );
    vboxLayout->addWidget( defaultlb );

    gridLayout->addLayout( vboxLayout, 0, 0, 1, 1 );

    QHBoxLayout* hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing( 6 );
    infolb = new QLabel( i18n( "<font color=\"#ff0000\"><b>Tip:</b></font>Only root can do something for system setting.\nOther users can only change default ime." ), this );
    hboxLayout->addWidget( infolb );

    buttonOk = new QPushButton( i18n( "OK" ), this );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    hboxLayout->addWidget( buttonOk );

    buttonCancel = new QPushButton( i18n( "Cancel" ), this );
    buttonCancel->setAutoDefault( true );
    hboxLayout->addWidget( buttonCancel );

    gridLayout->addLayout( hboxLayout, 1, 0, 1, 2 );
}

#include "mlimecfg_cl.moc"
