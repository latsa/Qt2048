#include "DlgAbout.h"
#include <QDesktopservices>
#include <QUrl>

DlgAbout::DlgAbout(QWidget *parent)
    : QDialog(parent) {
   ui.setupUi(this);
   setWindowFlags(windowFlags().setFlag(Qt::WindowContextHelpButtonHint, false));
   connect(ui.okButton,SIGNAL(clicked()), this, SLOT(OK()));
   connect(ui.pbLink,SIGNAL(clicked()), this, SLOT(link()));
}


DlgAbout::~DlgAbout() {

}


void DlgAbout::OK() {
}

void DlgAbout::link() {
   QDesktopServices::openUrl(QUrl("https://web.archive.org/web/20140328011720/https://play.google.com/store/apps/details?id=com.veewo.a1024"));
}
