#pragma once
#include <QPushButton>

class PushButton : 
   public QPushButton {
   Q_OBJECT

public:
      PushButton(QWidget* parent);

protected:
   void mousePressEvent(QMouseEvent* evt) override;
   void mouseReleaseEvent(QMouseEvent* evt) override;
   void mouseMoveEvent(QMouseEvent* evt) override;

private:
   QWidget* m_parent = Q_NULLPTR;
   bool m_click = false;
};