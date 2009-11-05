#include <QtGui/QApplication>
#include <QStringList>
#include "mainwindow.h"



int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QStringList args = a.arguments();
   MainWindow w;

//    w.setFixedSize(350, 600);
   w.setWindowTitle("Great Evaluation Tool");

   if (args.size() == 2)
   {
      w.readFile(args.at(1));
   }

   w.show();
   return a.exec();
}

