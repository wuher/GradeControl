/*
 * todo: clear() function for clearing the whole list
 */

#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include <QVector>
#include <QMap>
#include <QList>


#include "student.h"
#include "gradeslider.h"



class StudentList : public QObject
{
   Q_OBJECT;

public:

   StudentList(int grades_amount);

   ~StudentList();

   void add(unsigned int studentNumber, unsigned int examPoints);

   void setLimits(QList< int > limits);

   void changeLimit(unsigned int newGradeLimit);

   void sort();

   void clearData();

   QList<unsigned int> getStudentAmounts() const;

signals:

   void canDraw(StudentList*);

public slots:

   void limitsChanged(GradeSlider* slider);

private:

   int getGrade(unsigned int examPoints);
   void deleteStudents();

   // Arvosana avaimena
   QMap<unsigned int, QList<Student*> > students_;

   // Arvosanarajat on tuolla
   QList<int> grades_;
};

#endif // STUDENTLIST_H
