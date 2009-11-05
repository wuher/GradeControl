/*
 *
 */



#include "gradesliderhandle.h"
#include "gradeslider.h"
#include <QApplication>
#include <QDebug>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>



/**
 * ctor
 *
 * todo: check the +1
 */
GradeSliderHandle::GradeSliderHandle(QGraphicsItem* parent, int lineLength,
                                     int id, int value, int lower, int upper) :
   QGraphicsItem(parent), id(id), value_(value), lineChanged_(true),
   lineLength(lineLength), valueLength(upper - lower + 1),
   slideLength(lineLength + GradeSliderHandle::HANDLE_WIDTH), leftHandle(0),
   rightHandle(0)
{
   this->setAcceptHoverEvents(true);

   // create value label
   QGraphicsSimpleTextItem* text =
      new QGraphicsSimpleTextItem(QString::number(this->value_), this);
   QFont font = text->font();
   font.setPixelSize(10);
   text->setFont(font);
   text->setPos(7 - text->boundingRect().width() / 2,  -15);

   // create id label
   QGraphicsSimpleTextItem* staticLabel =
      new QGraphicsSimpleTextItem(QString::number(id), this);
   staticLabel->setFont(font);
   staticLabel->setPos(7 - staticLabel->boundingRect().width() / 2,  15);

   Q_ASSERT(this->id == id);
   Q_ASSERT(this->value_ == value);
   Q_ASSERT(this->lineChanged_);
}


/**
 * empty dtor
 */
GradeSliderHandle::~GradeSliderHandle()
{
}


/**
 * paint the handle
 */
void GradeSliderHandle::paint(QPainter* painter,
                              const QStyleOptionGraphicsItem* /*option*/,
                              QWidget* /*widget*/)
{
#ifndef QT_NO_NODEBUG
   int tmpval = this->value();
#endif

   painter->setRenderHint(QPainter::Antialiasing);

   if (this->lineChanged_)
   {
      this->setPos(qreal(this->valueToPos(this->value_)), HANDLE_HEIGHT / -2.0);
      this->lineChanged_ = false;
   }

   // draw handle
   QRadialGradient grad(QPoint(HANDLE_WIDTH / 2, HANDLE_HEIGHT / 2), 10);
   grad.setColorAt(0, Qt::blue);
   grad.setColorAt(1, Qt::white);
   QBrush brush(grad);
   painter->setBrush(brush);
   painter->setPen(Qt::NoPen);
   painter->drawEllipse(0, 0, HANDLE_WIDTH, HANDLE_HEIGHT);

   // update value label
   Q_ASSERT(this->childItems().size() == 2);
   QGraphicsSimpleTextItem* text = dynamic_cast<QGraphicsSimpleTextItem*>(
      this->childItems().first());
   Q_ASSERT(text);
   text->setText(QString::number(this->value()));

   Q_ASSERT(tmpval == this->value());
   Q_ASSERT(!this->lineChanged_);
}


/**
 * return handle's bounds
 */
QRectF GradeSliderHandle::boundingRect() const
{
   return QRectF(0, 0, HANDLE_WIDTH, HANDLE_HEIGHT);
}


/**
 * todo: this is probably useless..
 */
bool GradeSliderHandle::acceptHoverEvents() const
{
   return true;
}


/**
 * use open hand cursor while hovering
 */
void GradeSliderHandle::hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/)
{
//    setCursor(Qt::OpenHandCursor);
}


/**
 * put arrow cursor back after hover
 */
void GradeSliderHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/)
{
   setCursor(Qt::ArrowCursor);
}


/**
 * use closed hand cursor while dragging
 */
void GradeSliderHandle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
   if (event->button() != Qt::LeftButton) {
      event->ignore();
      return;
   }

   this->dragStartPos = event->pos();

//    setCursor(Qt::ClosedHandCursor);
}



/**
 * implement dragging
 */
void GradeSliderHandle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
   Q_ASSERT(this->scene()->views().at(0));

   qreal newx = this->mapToParent(event->pos()).x() - this->dragStartPos.x();
   this->nudge(newx);

   // todo: this should probably be moved into updaveValue() or somewhere..
   emit valueChanged();

   QDrag* drag = new QDrag(event->widget());
   drag->exec();
//    setCursor(Qt::ClosedHandCursor);
}



/**
 * try to move the handle
 */
int GradeSliderHandle::nudge(qreal newx)
{
   QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(
      this->parentItem());
   Q_ASSERT(line);

   qreal oldx = this->pos().x() * 1.0;
   qreal minx = -1.0 * HANDLE_WIDTH / 2.0;
   qreal maxx = this->lineLength - (HANDLE_WIDTH / 2.0);

   // don't go out of bounds
   if (newx < minx) newx = minx;
   else if (newx > maxx) newx = maxx;

   if (this->leftHandle && (oldx > newx) &&
       newx < this->leftHandle->pos().x() + HANDLE_WIDTH)
   {
      // collision with the left handle
      Q_ASSERT(this->leftHandle != this);
      newx = this->leftHandle->nudge(newx - HANDLE_WIDTH) + HANDLE_WIDTH;
   }
   else if (this->rightHandle && (oldx < newx) &&
            (newx + HANDLE_WIDTH) > this->rightHandle->pos().x())
   {
      // collision with the right handle
      Q_ASSERT(this->rightHandle != this);
      newx = this->rightHandle->nudge(newx + HANDLE_WIDTH) - HANDLE_WIDTH;
   }

   // set position
   QPointF newpoint = QPointF(newx, -(this->boundingRect().height() / 2.0));
   this->value_ = this->posToValue(newpoint.x());
   this->setPos(newpoint);

   return newx;
}


/**
 * handle's value
 */
int GradeSliderHandle::value() const
{
   return this->value_;
}



/**
 * set line's length
 */
void GradeSliderHandle::setLineLength(qreal length)
{
#ifndef QT_NO_NODEBUG
   int dirty = this->lineChanged_;
   int oldlen = this->lineLength;
#endif

   if (this->lineLength != length)
   {
      this->lineLength = length;
      this->slideLength = length + GradeSliderHandle::HANDLE_WIDTH;
      this->lineChanged_ = true;
   }

   Q_ASSERT(this->lineLength == length);
   Q_ASSERT(dirty ||
            (length == oldlen && !this->lineChanged_) ||
            (length != oldlen && this->lineChanged_));
}



/**
 * mouse released after drag
 */
void GradeSliderHandle::mouseReleaseEvent(
   QGraphicsSceneMouseEvent* /*event*/)
{
//    setCursor(Qt::OpenHandCursor);
}



/**
 * set the handle on the left
 */
void GradeSliderHandle::setLeftNeighbour(GradeSliderHandle* left)
{
   Q_ASSERT(this->leftHandle == 0);

   this->leftHandle = left;

   Q_ASSERT(this->leftHandle == left);
}



/**
 * set the handle on the right
 */
void GradeSliderHandle::setRightNeighbour(GradeSliderHandle* right)
{
   Q_ASSERT(this->rightHandle == 0);

   this->rightHandle = right;

   Q_ASSERT(this->rightHandle == right);
}



/**
 * converts value to line's x
 */
qreal GradeSliderHandle::valueToPos(int value) const
{
   qreal offset = GradeSliderHandle::HANDLE_WIDTH / 2.0;
   qreal pixelstep = this->slideLength / qreal(this->valueLength);

   qreal ret = qreal(pixelstep * value) + offset;

   return ret;
}



/**
 * converts line's x coordinate into value
 */
int GradeSliderHandle::posToValue(qreal x) const
{
   qreal offset = GradeSliderHandle::HANDLE_WIDTH / 2.0;
   qreal pixelstep = this->lineLength / qreal(this->valueLength);
   qreal newx = x + offset;

   return newx / pixelstep;
}
