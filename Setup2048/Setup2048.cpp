#include "Setup2048.h"
#include "DirSelection.h"
//#include "CreateLink.h"
#include "ShortcutProvider.h"
#include "QuaZip/quazip.h"
#include "QuaZip/quazipfile.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QTreeView>
#include <QFile>
#include <QTemporaryFile>
#include <QSaveFile>
#include <QDir>
#include <QSettings>
#include <QDebug>

Setup2048::Setup2048(QWidget *parent)
   : QMainWindow(parent) {
   ui.setupUi(this);
   ui.leTargetDirectory->setText(m_targetLocation+m_installationPath);

   ui.labelProgress->hide();
   ui.progressBar->hide();
   connect(ui.pbCancel,SIGNAL(clicked()),this,SLOT(cancel()));
   connect(ui.pbInstall,SIGNAL(clicked()),this,SLOT(install()));
   connect(ui.pbTargetDirectory,SIGNAL(clicked()),this,SLOT(select()));
}

Setup2048::~Setup2048() {
}

void Setup2048::select() {
   // get target location from user
   CDirSelectionDlg dirSelector(m_targetLocation, m_installationPath, this);

   if (!dirSelector.exec())
      return;

   m_targetLocation = dirSelector.dirPath();
   ui.leTargetDirectory->setText(m_targetLocation);
}

void Setup2048::install() {
   ui.progressBar->setValue(10); ////

   ui.labelProgress->show();
   ui.progressBar->show();

   // Start application on exit?
   m_startApp = ui.chkStartApp->isChecked();

   // Extract .zip archive from resource
   QFile stream;
   stream.setFileName(":/Setup2048/App/App.zip");
   QByteArray zip;

   if (stream.open(QIODevice::ReadOnly)) {
      zip = stream.readAll();
      stream.close();
   } else {
      QMessageBox::critical(this, tr("2048 Setup"), stream.errorString(), QMessageBox::Ok);
      return;
   }

   ui.progressBar->setValue(20); ////

   // Write .zip archive to file
   m_targetLocation = ui.leTargetDirectory->text();
   QDir tl(m_targetLocation);
   tl.removeRecursively();
   tl.mkpath(m_targetLocation);

   QString zipArchivePath = m_targetLocation + QString("/2048setup.zip");
   QTemporaryFile file(this);
   if (file.open()) {
      QString tempFileName = file.fileName();
      file.write(zip);
      file.close();
      file.copy(zipArchivePath);
   }

   ui.progressBar->setValue(30); ////

   // Unpack zip file
   QuaZip quazip(zipArchivePath);

   if (quazip.open(QuaZip::mdUnzip)) {
      QuaZipFile quazipfile(&quazip);

      for (bool f = quazip.goToFirstFile(); f; f = quazip.goToNextFile()) {
         QuaZipFileInfo64 info;
         quazipfile.getFileInfo(&info);

         if (quazipfile.open(QIODevice::ReadOnly)) {
            QByteArray filedata = quazipfile.readAll();
            quazipfile.close();

            QString name = info.name;
            QString lastchar = name.mid(name.size() - 1);
            QString path = m_targetLocation + "/" + info.name;

            if (lastchar == "\\" || lastchar == "/") {
               QDir dir; dir.mkpath(path);
            }
            else {

               if (path.contains("platforms")) {
                  QDir dir; dir.mkpath(m_targetLocation + "/platforms");
               }

               QSaveFile file(path, this);
               QByteArray extra;
               QDateTime modificationTime = info.getExtTime(extra, 1);
               file.setFileTime(modificationTime, QFileDevice::FileTime::FileModificationTime);
               QDateTime accessTime = info.getExtTime(extra, 2);
               file.setFileTime(accessTime, QFileDevice::FileTime::FileAccessTime);
               QDateTime creationTime = info.getExtTime(extra, 4);
               file.setFileTime(creationTime, QFileDevice::FileTime::FileBirthTime);

               if (file.open(QIODevice::WriteOnly)) {
                  qint64 usize = quazipfile.usize();
                  qint64 fsize = filedata.size();
                  qint64 byteswritten = file.write(filedata, usize);

                  if (byteswritten == -1) {
                     QMessageBox::critical(this, tr("2048 Setup"), tr("A file write error occured. (4)"), QMessageBox::Ok);
                     return;
                  }
                  else if (usize != byteswritten) {
                     QMessageBox::critical(this, tr("2048 Setup"), tr("A file size check error occured. (3)"), QMessageBox::Ok);
                     return;
                  }

                  file.commit();
               }

            }


         }
         else {
            QMessageBox::critical(this, tr("2048 Setup"), tr("A file read error occured. (2)"), QMessageBox::Ok);
            return;
         }
      }
      quazip.close();

   } else {
      QMessageBox::critical(this, tr("2048 Setup"), tr("A file access error occured. (1)"), QMessageBox::Ok);
      return;
   }

   ui.progressBar->setValue(50); ////

   //Delete zip file
   QFile::remove(zipArchivePath);

   ui.progressBar->setValue(60); ////

   //Update PATH environment variable if needed
   if (ui.chkPath->isChecked()) {

      QString Path = qEnvironmentVariable("PATH");

      if (Path.size()) {
         if (!Path.contains(m_targetLocation, Qt::CaseInsensitive)) {

            // add our target location to the path
            Path = m_targetLocation + ";" + Path;

            #ifdef _WIN32
            QSettings setting("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", QSettings::NativeFormat);
            setting.setValue("Path", Path);
            SendMessageA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment");
            #endif

            qputenv("Path", Path.toStdString().data());
         }
      }

   }

   ui.progressBar->setValue(70); ////

   // program's path
   QString dst = m_targetLocation + "/2048.exe";
   std::wstring wstrDst = dst.toStdWString();

   // program's working path
   std::wstring wstrCurrDir = m_targetLocation.toStdWString();

   // %APPDATA%
   QByteArray APPDATA = qgetenv("APPDATA");

   //Update Start Menu if needed
   // 
   // The paths to the Start Menu are:
   //    %ProgramData%\Microsoft\Windows\Start Menu\Programs
   //    %AppData%\Microsoft\Windows\Start Menu\Programs

   if (ui.chkStartMenu->isChecked()) {
      QByteArray startMenuTail = QByteArray("\\Microsoft\\Windows\\Start Menu\\Programs");
    
      //QByteArray userStartMenuPath = APPDATA + startMenuTail;

      QByteArray ProgramData = qgetenv("ProgramData");
      QByteArray globalStartMenuPath = ProgramData + startMenuTail;

      // create directory
      QByteArray targetDir = globalStartMenuPath + QByteArray("\\2048");
      QDir dir(targetDir);
      if (!dir.exists())
         dir.mkpath(".");

      // add shortcut into the directory
      QString dst = m_targetLocation + "/2048.exe";
      std::wstring wstrDst = dst.toStdWString();
     
      QFileInfo f(dst);
      QString lnk = targetDir + QString("\\") + f.fileName() + QString(".lnk");
      std::wstring wstrLnk = lnk.toStdWString();
    
      

      ShortcutProvider::Create(wstrDst.data(), /* target args*/ L"", wstrLnk.data(),
         /* description*/ L"2048.exe", 1, wstrCurrDir.data(), /* icon file */L"", /*icon index */ 0);
   }

   ui.progressBar->setValue(80); ////

   //Add desktop shortcut
   if (ui.chkDesktopShortcut->isChecked()) {
      QByteArray home = qgetenv("USERPROFILE");
      QByteArray targetDir = home + QByteArray("\\Desktop");

      QFileInfo f(dst);
      QString lnk = targetDir + QString("\\") + f.fileName() + QString(".lnk");
      std::wstring wstrLnk = lnk.toStdWString();

      ShortcutProvider::Create(wstrDst.data(), /* target args*/ L"", wstrLnk.data(),
         /* description*/ L"2048.exe", 1, wstrCurrDir.data(), /* icon file */L"", /*icon index */ 0);
   }

   ui.progressBar->setValue(90); ////

   //Update QuickLaunch if needed
   if (ui.chkQuickLaunch->isChecked()) {
      QByteArray targetDir = APPDATA + QByteArray("\\Microsoft\\Internet Explorer\\Quick Launch");

      QFileInfo f(dst);
      QString lnk = targetDir + QString("\\") + f.fileName() + QString(".lnk");
      std::wstring wstrLnk = lnk.toStdWString();

      ShortcutProvider::Create(wstrDst.data(), /* target args*/ L"", wstrLnk.data(),
         /* description*/ L"2048.exe", 1, wstrCurrDir.data(), /* icon file */L"", /*icon index */ 0);
   }

   ui.progressBar->setValue(100); ////

   ui.pbCancel->setText("&Quit");
   ui.pbInstall->hide();
   ui.labelProgress->setText("Ready");

   ui.chkDesktopShortcut->setEnabled(false);
   ui.chkPath->setEnabled(false);
   ui.chkQuickLaunch->setEnabled(false);
   ui.chkStartApp->setEnabled(false);
   ui.chkStartMenu->setEnabled(false);
   ui.pbInstall->setEnabled(false);
   ui.pbTargetDirectory->setEnabled(false);
   ui.leTargetDirectory->setEnabled(false);

   m_installed = true;
}

void Setup2048::cancel() {
   qApp->quit();

   if (m_installed && m_startApp) {
      QString appPath = m_targetLocation + QString("/2048.exe");
      system(appPath.toStdString().data());
   }
   
}
