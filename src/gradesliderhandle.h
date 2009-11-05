/*
 *
 */


#ifndef GRADE_SLIDER_HANDLE_H
#define GRADE_SLIDER_HANDLE_H


#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>


class GradeSlider;


class GradeSliderHandle :
   public QObject, public QGraphicsItem
{

   Q_OBJECT

#if QT_VERSION >= 0x040600
   Q_INTERFACES(QGraphicsItem);
#endif


public:

   const static qreal HANDLE_WIDTH = 15.0;
   const static qreal HANDLE_HEIGHT = 15.0;


   /// ctor
   GradeSliderHandle(QGraphicsItem* parent, int lineLength,
                     int id, int value, int lower, int upper);


   /// dtor
   virtual ~GradeSliderHandle();


   /// paint me
   void paint(QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget);


   /// our bounds
   QRectF boundingRect() const;


   /// set neighbouring handles
   void setLeftNeighbour(GradeSliderHandle*);
   void setRightNeighbour(GradeSliderHandle*);

   /// get handle's value
   int value() const;


   /// store new line length
   void setLineLength(qreal length);


signals:

   /// some (at least one) values have changed
   void valueChanged();

protected:

   virtual bool acceptHoverEvents() const;
   virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
   virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
   virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

private:

   // our id (this is just for showing the tooltip)
   int id;

   // our value
   int value_;

   // has our value changed? (i.e. do we need to recalculate our position)
   bool lineChanged_;

   /// this where the hand grabbed the handle
   QPointF dragStartPos;

   /// the slider line's length
   int lineLength;

   /// get line's length in values (i.e. how many possible values
   /// there are)
   int valueLength;

   /// this is usually bigger than lineLength as we want to allow the
   /// handle to go little further than where the line ends
   int slideLength;

   /// handle on the left (null if this is the leftmost)
   GradeSliderHandle* leftHandle;

   /// handle on the right (null if this is the rightmost)
   GradeSliderHandle* rightHandle;

   /// prevent copying
   GradeSliderHandle(const GradeSliderHandle&);

   /// prevent assignment
   GradeSliderHandle& operator=(const GradeSliderHandle&);

   /// update our value
   void updateValueAndPos(const QPointF& point);

   /// try to move this handle to the given x coordinate
   int nudge(qreal newx);

   /// convert line's x to a grade
   int posToValue(qreal x) const;

   /// convert grade to position on the line
   qreal valueToPos(int value) const;

};


#endif
