#include "mlimecfg.h"

#include <QVBoxLayout>

#include <KGenericFactory>
#include <KLocale>
#include <KGlobal>
#include <KDialog>

K_PLUGIN_FACTORY( MLImeCfgFactory, registerPlugin<MLImeCfg>(); )
K_EXPORT_PLUGIN( MLImeCfgFactory( "kcmmlimecfg", "kcm_mlimecfg" ) )

MLImeCfg::MLImeCfg( QWidget *parent, const QVariantList &args )
    : KCModule( MLImeCfgFactory::componentData(), parent, args )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( KDialog::spacingHint() );

    m_micl = new MLImeCfg_CL( this );
    layout->addWidget( m_micl );
    m_micl->show();

    KAboutData *about =
            new KAboutData( "IME Configure", 0, ki18n( "Magic Linux IME Configure Module" ), "1.5.0",
                            ki18n( "This module help you to manage your input methods" ),
                            KAboutData::License_GPL,
                            ki18n( "(c) 2005-2009 Magic Group" ) );

    about->addAuthor( ki18n( "KanKer" ), ki18n( "IME configure scripts" ), "kanker@163.com" );
    about->addAuthor( ki18n( "lovewilliam" ), ki18n( "kcm module" ), "lovewilliam@gmail.com" );
    about->addAuthor( ki18n( "Ni Hui" ), ki18n( "Port to cmake, Port to Qt4/KDE4, Port to new kpluginfactory" ), "shuizhuyuanluo@126.com" );
    setAboutData( about );

    // place widgets here
    load();
};

MLImeCfg::~MLImeCfg()
{
    delete m_micl;
}

void MLImeCfg::load()
{
    // insert your loading code here...
}

void MLImeCfg::defaults()
{
    // insert your default settings code here...
    emit changed(true);
}

void MLImeCfg::save()
{
    // insert your saving code here...
    m_micl->buttonOk_clicked();
    emit changed(false);
}

void MLImeCfg::configChanged()
{
    // insert your saving code here...
    emit changed(true);
}

#include "mlimecfg.moc"
