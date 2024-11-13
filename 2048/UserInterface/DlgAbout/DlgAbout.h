#pragma once
#include <QDialog>
#include "ui_DlgAbout.h"

class DlgAbout  : 
   public QDialog {
   Q_OBJECT

public:
    DlgAbout(QWidget *parent);
    ~DlgAbout();

public slots:
    void OK();
    void link();

private:
   Ui::DlgAboutClass ui;
};
