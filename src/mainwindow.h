#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui>
#include <QDebug>
#include <QString>
#include <QShortcut>
#include "studentlist.h"


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
   MainWindow(QWidget *parent = 0 );
   ~MainWindow();

   void readFile( const QString& filename );

protected:

   /// app becomes visible
   virtual void showEvent(QShowEvent* /*event*/);


private slots:
   void loadFile();
   void about();

private:
   Ui::MainWindow *ui;
   StudentList studentList_;
   bool fileread_;
};

#endif // MAINWINDOW_H
