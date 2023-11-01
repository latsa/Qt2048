#include "DirSelection.h"
#include <QLabel>
#include <QBoxLayout>
#include <QDialogButtonBox>

CDirSelectionDlg::CDirSelectionDlg(const QString& programsPath, const QString& installationPath, QWidget *parent) :
   QDialog(parent), m_programsPath(programsPath), m_installationPath(installationPath) {
   setMinimumSize(200, 300);
   resize(400, 430);

   m_model = new QFileSystemModel(this);
   QString targetPath = m_programsPath + m_installationPath;
   auto rootIdx = m_model->setRootPath(m_programsPath);

   m_treeView = new QTreeView(this);
   m_treeView->setModel(m_model);
   m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
   m_treeView->setHeaderHidden(true);
   m_treeView->setSortingEnabled(true);
   m_treeView->sortByColumn(0, Qt::AscendingOrder);

   for(int i = 1; i < m_model->columnCount(); i ++)    // don't show Size, Type, etc.
      m_treeView->setColumnHidden(i, true);

   m_treeView->scrollTo(rootIdx);
   m_treeView->selectionModel()->setCurrentIndex(rootIdx, QItemSelectionModel::Current | QItemSelectionModel::Select);
   connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &CDirSelectionDlg::onCurrentChanged);

   auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
   connect(buttonBox, &QDialogButtonBox::accepted, this, &CDirSelectionDlg::accept);
   connect(buttonBox, &QDialogButtonBox::rejected, this, &CDirSelectionDlg::reject);
   m_OKbutton = buttonBox->button(QDialogButtonBox::Ok);

   auto label = new QLabel(tr("Folder:"));
   m_folderName = new QLineEdit(this);
   m_folderName->setReadOnly(true);

   m_folderName->setText(targetPath);

   auto pathLayout = new QHBoxLayout();
   pathLayout->addWidget(label);
   pathLayout->addSpacing(10);
   pathLayout->addWidget(m_folderName);

   auto mainLayout = new QVBoxLayout();
   mainLayout->addWidget(m_treeView);
   mainLayout->addSpacing(10);
   mainLayout->addLayout(pathLayout);
   mainLayout->addSpacing(10);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);
}

void CDirSelectionDlg::onCurrentChanged() {
   auto fileInfo = m_model->fileInfo(m_treeView->selectionModel()->currentIndex());

   m_programsPath = fileInfo.absoluteFilePath();
   QString targetPath = m_programsPath + m_installationPath;

   m_folderName->setText(targetPath);
   m_OKbutton->setEnabled(fileInfo.isDir());
   m_OKbutton->setDefault(fileInfo.isDir());
}

QString CDirSelectionDlg::dirPath() const {
   //return m_model->fileInfo(m_treeView->selectionModel()->currentIndex()).absoluteFilePath();
   return m_folderName->text();
}

QDir CDirSelectionDlg::dir() const {
    return QDir(dirPath());
}


