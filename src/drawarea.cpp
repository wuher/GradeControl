/*
 *
 */


#include "drawarea.h"



/**
 * ctor
 */
DrawArea::DrawArea(QWidget *parent)
   : QWidget(parent)
{
   // conf:
//    this->setBackgroundRole(parent->backgroundRole());
//    setBackgroundRole(QPalette::Base);
//    setAutoFillBackground(true);
}



/**
 * dtor
 */
DrawArea::~DrawArea()
{
}


/**
 * returns the width of a "student" in pixels
 */
unsigned int DrawArea::getUnitWidth() const
{
   return ((this->width() - MARGIN_LEFT - MARGIN_RIGHT) / COLUMNCOUNT);
}



/**
 * returns the height of a "student" in pixels
 */
unsigned int DrawArea::getUnitHeight() const
{
   unsigned int highest = 1;

   for (int i = 0; i < grades_.size(); ++i)
   {
      if (grades_.at(i) > highest)
      {
         highest = grades_.at(i);
      }
   }

   Q_ASSERT(highest > 0);

   int ret = ((this->height() - MARGIN_TOP - MARGIN_BOTTOM) / highest);

   return (ret > 0) ? ret : 1;
}



/**
 * creates the brush for painting the coluns
 *
 * conf:
 */
QBrush DrawArea::createColumnBrush() const
{
   QLinearGradient grad(MARGIN_LEFT, 0, this->width() - MARGIN_RIGHT, 0);
   qreal gradstep = 1.0 / (COLUMNCOUNT * 2);
   for (int i = 0; i <= (COLUMNCOUNT*2); i += 1)
   {
      if (i % 2 == 0) grad.setColorAt(i * gradstep, Qt::white);
      else grad.setColorAt(i * gradstep, Qt::darkGreen);
   }

   // return QBrush(Qt::blue);
   return QBrush(grad);
}



/**
 * paints the columns
 */
void DrawArea::paintEvent(QPaintEvent* /*event*/)
{
   // initialize painter and brush (conf:)
   static QPen columnpen(Qt::NoPen);
   static QPen labelpen(Qt::black);
   static QFont font("Times", 10);

   QPainter painter(this);
   painter.setPen(columnpen);
   painter.setBrush(createColumnBrush());
   painter.setRenderHint(QPainter::Antialiasing);

   // calculate the height of a single "student" in pixels
   unsigned int unitHeight = this->getUnitHeight();
   // calculate the width of a single "student" in pixels
   unsigned int unitWidth = this->getUnitWidth();
   // rowspacer needs to be 0 if there are too many students
   int rowspacer = (unitHeight - ROWSPACER <= 0) ? 0 : ROWSPACER;

   // collect rects to be drawn in here
   QVector<QRect> rects;

   // loop all grades
   for (int i = 0; i < grades_.size(); ++i)
   {
      painter.setPen(columnpen);

      // loop all "students" within a grade
      for (unsigned int j = 0; j < grades_.at(i); ++j)
      {
         QRect rect(
            MARGIN_LEFT + (COLUMNSPACER / 2) + (i * unitWidth),
            this->height() - (j * unitHeight) - (rowspacer / 2) - MARGIN_BOTTOM,
            unitWidth - COLUMNSPACER,
            unitHeight - rowspacer);

         rects.push_back(rect);
      }

      // draw the column
      painter.drawRects(rects);

      // draw the label below
      painter.setPen(labelpen);
      painter.setFont(font);
      // todo: this 10 is an estimate of font's height
      int ypos = this->height() - LABEL_MARGIN_BOTTOM - 10;
      // todo: this 5 is an estimate of font's width
      int xpos = MARGIN_LEFT + (i * unitWidth) + (unitWidth / 2) - 5;
      painter.drawText(xpos, ypos, QString::number(grades_.at(i)));

      // clear the rects vector
      rects.clear();
   }
}



/**
 * slider has changed -> update columns
 */
void DrawArea::drawGrades(StudentList* studentList)
{
   grades_ = studentList->getStudentAmounts();
   this->update();
}
