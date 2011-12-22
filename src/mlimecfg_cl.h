#ifndef MLIMECFG_CL_H
#define MLIMECFG_CL_H

#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QWidget>

class MLImeCfg_CL : public QWidget {
    Q_OBJECT

    public:
        MLImeCfg_CL( QWidget* parent = 0 );

    public slots:
        void buttonOk_clicked();

    private slots:
        void addbn_clicked();
        void clearle();
        void saveime();
        void delbn_clicked();
        void opencmd_clicked();
        void listBox_clicked( QListWidgetItem* );
        void clearbn_clicked();
        void defaultbn_clicked();
        void readxinput();
        void savebt_clicked();
        void createbt_clicked();
        void buttonCancel_clicked();
        void showdefaultime();
        void auto_choose();

    signals:
        void cancel_clked();

    private:
        bool checkroot();
        void setupUI();

        int imecount; // imecount 用于记录系统中原有的输入法个数
        int listitem; // ilistitem 记录当前添加后输入法的个数
        bool imefilestatus; // 记录输入法配置文件的更新状态

        QTabWidget *tabWidget;
        QWidget *Widget2;
        QPushButton *defaultbn;
        QPushButton *clearbn;
        QPushButton *addbn;
        QPushButton *delbn;
        QSplitter *splitter1;
        QLineEdit *namele;
        QLineEdit *ximle;
        QLineEdit *programle;
        QLineEdit *argle;
        QLineEdit *gtkle;
        QLineEdit *qtle;
        QPushButton *opencmd;
        QLabel *textLabel1;
        QLabel *textLabel1_2;
        QLabel *textLabel1_3;
        QLabel *textLabel1_4;
        QLabel *textLabel1_5;
        QLabel *textLabel1_6;
        QWidget *Widget3;
        QTextEdit *xinputte;
        QPushButton *createbt;
        QPushButton *savebt;
        QLabel *textLabel2;
        QLabel *textLabel3;
        QListWidget *listBox;
        QLabel *defaultlb;
        QLabel *infolb;
        QPushButton *buttonOk;
        QPushButton *buttonCancel;
};

#endif // MLIMECFG_CL_H
