#ifndef DRAWAREA_H
#define DRAWAREA_H
#include <QWidget>
#include <QtGui>
#include <QPalette>

#include <QList>
#include <QFont>
#include <QObject>
#include <QtGui/QPen>
#include "studentlist.h"


class DrawArea : public QWidget
{
   Q_OBJECT

   public:

   DrawArea(QWidget *parent = 0);
   ~DrawArea();
   void paintEvent(QPaintEvent * );



public slots:

   void drawGrades( StudentList* studentList);


private:

   /// drawing constants conf: ///////////////////////////


   /// how many vertical pixels between "students"
   static const int ROWSPACER = 2;
   /// how many horizontal pixels between columns
   static const int COLUMNSPACER = 20;
   /// number of coluns to draw
   static const int COLUMNCOUNT = GradeSlider::HANDLER_COUNT + 1;
   // margins
   static const int MARGIN_LEFT = 35;
   static const int MARGIN_RIGHT = 35;
   static const int MARGIN_TOP = 50;
   static const int MARGIN_BOTTOM = 50;
   static const int LABEL_MARGIN_BOTTOM = 10;


   ///////////////////////////////////////////////////////

   QList<unsigned int> grades_;

   unsigned int getUnitHeight() const;
   unsigned int getUnitWidth() const;
   QBrush createColumnBrush() const;

};

#endif // DRAWAREA_H
