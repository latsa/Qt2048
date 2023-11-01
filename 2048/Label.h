#pragma once
#include <QLabel>

class Label : 
   public QLabel {
   Q_OBJECT

public:
      Label(QWidget* parent);

protected:
   void mousePressEvent(QMouseEvent* evt) override;
   void mouseReleaseEvent(QMouseEvent* evt) override;
   void mouseMoveEvent(QMouseEvent* evt) override;

private:
   QWidget* m_parent = Q_NULLPTR;
};