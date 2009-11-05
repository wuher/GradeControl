/*
 *
 */


#ifndef GRADESLIDER_H
#define GRADESLIDER_H

#include <QtGui/QWidget>

#include <QtGui/QGraphicsView>



class GradeSliderHandle;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsLineItem;


class GradeSlider :
   public QWidget
{
   Q_OBJECT;

public:

   /// number of handlers
   static const int HANDLER_COUNT = 5;

   /// default minimum and maximum values
   static const int DEFAULT_MIN_VALUE = 0;
   static const int DEFAULT_MAX_VALUE = 100;

   /// how much spae we leave to the left and right of the line
   static const int LINE_MARGIN_X = 20;
   static const int LINE_MARGIN_TOP = 20;

   /// ctor
   GradeSlider(QWidget *parent);

   /// dtor
   virtual ~GradeSlider();

   /// init. call this after creation
   virtual void init(int lower, int upper);

   /// resize event
   virtual void resizeEvent(QResizeEvent*);

   /// paint event
   virtual void paintEvent(QPaintEvent*);

   /// get slider values
   QList<int> getValues();


signals:

   void valuesChanged(GradeSlider*);


public slots:

   void handleMoved();


private:


   /// our view and scene
   QGraphicsView* view;
   QGraphicsScene* scene;

   /// the horizontal line
   QGraphicsLineItem* line;

   /// all handles
   QList<GradeSliderHandle*> handles;

   /// minimum and maximus values for a grade
   int minValue;
   int maxValue;

   /// prevent copying and assignemnt
   GradeSlider(const GradeSlider&);
   GradeSlider& operator=(const GradeSlider&);

   /// sets line's position and size based on our own size
   void setLinePos();

   /// initial ui construction
   void initUi();

};


#endif
