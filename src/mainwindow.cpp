/*
 *
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loaddata.h"
#include "drawarea.h"
#include "gradeslider.h"
#include "studentlist.h"
#include <QtGui/QMessageBox>


MainWindow::MainWindow(QWidget *parent )
   : QMainWindow(parent), ui(new Ui::MainWindow),
     studentList_(GradeSlider::HANDLER_COUNT), fileread_(false)
{
   ui->setupUi(this);
   ui->widget->init(0, 20);

   // todo
   ui->drawArea->setMinimumSize(80, 200);
   ui->widget->setMinimumSize(100, 70);

   this->setBaseSize(200, 500);

   studentList_.setLimits(ui->widget->getValues());

   ui->actionLoad->setShortcut(tr("Ctrl+O"));
   ui->actionQuit->setShortcut(tr("Ctrl+Q"));
   connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadFile()));
   connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
   connect(ui->widget, SIGNAL(valuesChanged( GradeSlider*)),
           &studentList_, SLOT(limitsChanged( GradeSlider*)));
   connect(&studentList_, SIGNAL(canDraw(StudentList*)),
           ui->drawArea, SLOT(drawGrades(StudentList*)));

}


MainWindow::~MainWindow()
{
   delete this->ui;
}


void MainWindow::showEvent(QShowEvent* event)
{
   QMainWindow::showEvent(event);

   // open file dialog automatically if no command line argument was given
   if (!this->fileread_) loadFile();
}


void MainWindow::readFile(const QString& filename)
{
   studentList_.clearData();
   studentList_.setLimits(ui->widget->getValues());

   if (loadData(filename, &studentList_))
   {
      studentList_.sort();
      ui->drawArea->drawGrades(&studentList_);
      this->fileread_ = true;
   }
   else
   {
      QMessageBox* box = new QMessageBox(this);
      box->setWindowTitle(tr(""));
      box->setText(tr("Unable to read the file"));
      box->show();
      delete box;
   }
}


void MainWindow::loadFile()
{
   QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"));

   readFile( fileName );
}



/**
 * show about dialog
 */
void MainWindow::about()
{
   QMessageBox msgBox;
   msgBox.setIcon(QMessageBox::Information);
   msgBox.setWindowTitle("About");

   msgBox.setText(" Great Evaluation Tool 0.0.1b \n Evaluation tool for evaluation purposes");
   msgBox.exec();
}
