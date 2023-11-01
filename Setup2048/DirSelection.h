#pragma once
#include <QtWidgets/QDialog>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QLineEdit>

class CDirSelectionDlg : 
   public QDialog {
   Q_OBJECT

public:
   CDirSelectionDlg(const QString& programsPath, const QString& installationPath, QWidget *parent = nullptr);
   QDir dir() const;
   QString dirPath() const;

private:
   void onCurrentChanged();

   QTreeView* m_treeView;
   QFileSystemModel* m_model;
   QLineEdit* m_folderName;
   QPushButton* m_OKbutton;
   QString m_programsPath;
   QString m_installationPath;
};