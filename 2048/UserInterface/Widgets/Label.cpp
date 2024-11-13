#include "Label.h"
#include <QApplication>
#include <QString>
#include <QUrl>
#include <QMouseEvent>
#include <QDesktopServices>

Label::Label(QWidget* parent) :
   m_parent(parent) {
}

void Label::mousePressEvent(QMouseEvent* evt) {
   QApplication::sendEvent(m_parent, evt);
}

void Label::mouseMoveEvent(QMouseEvent* evt) {
   QApplication::sendEvent(m_parent, evt);
}

void Label::mouseReleaseEvent(QMouseEvent* evt) {
   QApplication::sendEvent(m_parent, evt);
}
