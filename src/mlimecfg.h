#ifndef _MLIMECFG_H_
#define _MLIMECFG_H_

#include <KCModule>
#include <KAboutData>

#include "mlimecfg_cl.h"

class MLImeCfg : public KCModule
{
    Q_OBJECT

    public:
        MLImeCfg( QWidget *parent, const QVariantList &args );
        ~MLImeCfg();

        void load();
        void save();
        void defaults();

    public slots:
        void configChanged();

    private:
        MLImeCfg_CL* m_micl;
};

#endif // _MLIMECFG_H_
