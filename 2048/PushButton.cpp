#include "PushButton.h"
#include <QApplication>
#include <QMouseEvent>

PushButton::PushButton(QWidget* parent) :
   m_parent(parent) {
}

void PushButton::mousePressEvent(QMouseEvent* evt) {
   m_click = true;
   QPushButton::mousePressEvent(evt);
   QApplication::sendEvent(m_parent, evt);
}

void PushButton::mouseMoveEvent(QMouseEvent* evt) {
   m_click = false;
   QApplication::sendEvent(m_parent, evt);
}

void PushButton::mouseReleaseEvent(QMouseEvent* evt) {

   if (m_click) {
      QPushButton::mouseReleaseEvent(evt);
      m_click = false;
   }
   
   QApplication::sendEvent(m_parent, evt);
}