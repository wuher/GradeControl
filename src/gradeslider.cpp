/*
 * todo: probably leaks memory somewhere...all addItems and setParents
 * should be checked from documentation
 */


#include "gradeslider.h"
#include "gradesliderhandle.h"
#include <QDebug>
#include <QApplication>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>


/**
 * this is just for showing scene coordinates in the tooltip while debugging
 */
class GradeView : public QGraphicsView
{
public:
   GradeView(QGraphicsScene* scene) : QGraphicsView(scene) {}
   virtual bool event(QEvent* event)
   {
      if (event->type() == QEvent::ToolTip)
      {
         QHelpEvent* ev = dynamic_cast<QHelpEvent*>(event);
         Q_ASSERT(ev);
         this->setToolTip("pos: " + QString::number(ev->pos().x()) +
                          ", " + QString::number(ev->pos().y()));
      }

      return QGraphicsView::event(event);
   }
};



/**
 *
 */
GradeSlider::GradeSlider(QWidget* parent)
   : QWidget(parent), view(0), scene(0), line(0),
     minValue(DEFAULT_MIN_VALUE), maxValue(DEFAULT_MAX_VALUE)

{
   // create view and scene (i.e. the canvas)
   scene = new QGraphicsScene();
   // view = new GradeView(scene);
   view = new QGraphicsView(scene);
   view->setParent(this);
   view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setBackgroundRole(parent->backgroundRole());
   view->setForegroundRole(parent->foregroundRole());
   view->setFrameShape(QFrame::NoFrame);

   // todo: calculate these properly
   this->setMinimumHeight(70);
   this->setMinimumWidth(200);

   Q_ASSERT(this->view);
   Q_ASSERT(this->scene);
}



/**
 * dtor
 */
GradeSlider::~GradeSlider()
{
   delete view;
   delete scene;
   view = 0;
   scene = 0;
}



/**
 * init. sets min and max values.
 */
void GradeSlider::init(int lower, int upper)
{
   Q_ASSERT(this->minValue == DEFAULT_MIN_VALUE);
   Q_ASSERT(this->maxValue == DEFAULT_MAX_VALUE);

   this->minValue = lower;
   this->maxValue = upper;

   Q_ASSERT(this->minValue == lower);
   Q_ASSERT(this->maxValue == upper);
}



/**
 * implements resizing
 */
void GradeSlider::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);

   // todo: i'm pretty sure i shouldn't be doing this but for some
   // reason update() and repaint() didn't call paintEvent() always
   this->paintEvent(0);
}



/**
 *
 */
void GradeSlider::paintEvent(QPaintEvent* /*event*/)
{
   Q_ASSERT(this->scene);

   // draw ui components if they haven't been drawn yet
   if (this->scene->items().size() == 0)
   {
      initUi();
   }

   this->view->setGeometry(0, 0, this->size().width(), this->size().height());
   this->view->setSceneRect(0, 0, this->size().width(), this->size().height());
   this->setLinePos();
}



/**
 * return values of all sliders (as grades)
 */
QList<int> GradeSlider::getValues()
{
   Q_ASSERT(this->scene);

   QList<int> ret;

   // if this function is called before initial paint, we generate
   // default values for handles

   if (handles.size() == 0)
   {
      qreal step = (this->maxValue-this->minValue) / (HANDLER_COUNT + 1.0);
      for (int i = 0; i < HANDLER_COUNT; i += 1)
      {
         // 0.5 is for rounding properly
         ret << (step * (i+1)) + 0.5;
      }
   }
   else
   {
      foreach (GradeSliderHandle* handle, this->handles)
      {
         ret << handle->value();
      }
   }

   Q_ASSERT(ret.size() == HANDLER_COUNT);

   return ret;
}



/**
 * propagate signal to main window
 */
void GradeSlider::handleMoved()
{
   emit valuesChanged(this);

   this->update();
}



/**
 * set line's position based on our size
 */
void GradeSlider::setLinePos()
{
   Q_ASSERT(this->line);

   static const qreal linemargin = GradeSlider::LINE_MARGIN_X;

   QSize size = this->size();
   qreal line_y = size.height() / 2.0;

   this->line->setLine(0, 0, size.width() - 2*linemargin, 0);
   this->line->setPos(linemargin, line_y);

   foreach (GradeSliderHandle* handle, this->handles)
   {
      handle->setLineLength(this->line->line().length());
   }
}



/**
 * initializes ui
 */
void GradeSlider::initUi()
{
   // draw the slider line
   QPen pen(Qt::gray);
   pen.setWidth(0);

   // add handles
   QList<int> values = this->getValues();
   this->line = scene->addLine(0, 0, 200, 0, pen);
   // this->line->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
   this->setLinePos();

   // draw handles at initial positions
   for (int i = 0; i < values.size(); i += 1)
   {
      GradeSliderHandle* handle = new GradeSliderHandle(
         this->line, this->line->line().length(), i+1, values.at(i),
         this->minValue, this->maxValue);
      // handle->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
      // handle->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

      Q_ASSERT(this->line);

      if (i > 0)
      {
         // set neighbouring handles
         Q_ASSERT(this->handles.size() > 0);
         Q_ASSERT(this->handles.last() != handle);
         handle->setLeftNeighbour(this->handles.last());
         this->handles.last()->setRightNeighbour(handle);
      }

      // store handle internally
      this->handles << handle;

      // start listening handle events
      QObject::connect(handle, SIGNAL(valueChanged()),
                       this, SLOT(handleMoved()));
   }

   Q_ASSERT(this->line);
   Q_ASSERT(this->line->childItems().size() == HANDLER_COUNT);
   Q_ASSERT(this->handles.size() == HANDLER_COUNT);
   Q_ASSERT(this->minValue < this->maxValue);
}
