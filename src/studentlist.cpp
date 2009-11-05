#include <QtAlgorithms>
#include <QMapIterator>
#include <QListIterator>
#include <QDebug>
#include "student.h"
#include "gradeslider.h"
#include "studentlist.h"

bool lessThan(const Student* s1, const Student* s2)
{
   return (s1->getPoints() < s2->getPoints());
}

StudentList::StudentList(int grades_amount):students_(), grades_()
{
   for(int i = 0; i <= grades_amount; ++i)
   {
      students_.insert(i, QList<Student* >());
   }

   for(int i = 0; i < grades_amount; ++i)
   {
      grades_.push_back(0);
   }
}

StudentList::~StudentList()
{
   deleteStudents();
}

void StudentList::add(unsigned int studentNumber, unsigned int examPoints)
{
   Student* student = new Student(studentNumber, examPoints);
   unsigned grade = getGrade(examPoints);
   students_[ grade ].append(student);
}

void StudentList::setLimits(QList<int> limits)
{
   grades_ = limits;
}

// Change grade limit
void StudentList::changeLimit(unsigned int newGradeLimit) {

   unsigned int grade = getGrade(newGradeLimit);

   while(!students_[ grade ].empty() &&
         students_[ grade ].first()->getPoints() < newGradeLimit)
   {
      students_[ grade-1 ].push_back(students_[ grade ].first());
      students_[ grade ].pop_front();
   }
   if(grade != 5) {
      while(!students_[ grade ].empty() &&
            students_[ grade ].last()->getPoints() >= newGradeLimit)
      {
         students_[ grade+1 ].push_front(students_[ grade ].last());
         students_[ grade ].pop_back();
      }
   }
   if(grade == 1)
   {
      while(!students_[ grade-1 ].empty() &&
            students_[ grade-1 ].last()->getPoints() >= newGradeLimit)
      {
         students_[ grade ].push_front(students_[ grade-1 ].last());
         students_[ grade-1 ].pop_back();
      }
   }

}

void StudentList::sort()
{

   QMap<unsigned int, QList<Student*> >::iterator i;
   for(i = students_.begin(); i != students_.end(); ++i)
   {
      qSort(i.value().begin(), i.value().end(), lessThan);

   }
}

void StudentList::clearData()
{
   deleteStudents();
   QMap<unsigned int, QList<Student*> >::iterator i;
   for(i = students_.begin(); i != students_.end(); ++i)
   {
      i.value().clear();
   }
}

QList<unsigned int> StudentList::getStudentAmounts() const
{
   QList< unsigned int > amounts;

   QMapIterator<unsigned int, QList<Student*> > i(students_);

   while (i.hasNext())
   {
      i.next();
      amounts.push_back(i.value().size());
   }

   return amounts;
}

void StudentList::limitsChanged(GradeSlider* slider)
{
   grades_ = slider->getValues(); //new values from slider

   // update students to correct grades
   QListIterator<int> i(grades_);
   while (i.hasNext())
   {
      changeLimit(i.next());
   }

   emit canDraw(this); // send signal
}



// Private functions
int StudentList::getGrade(unsigned int examPoints)
{
   // loops through grades_ array to find which grade student gets
   // with examPoints points
   int i = -1;
   while (++i < grades_.size())
   {
      if (int(examPoints) < grades_.at(i))
      {
         break;
      }
   }

   Q_ASSERT(i >= 0 && i <= grades_.size());

   return i;
}

void StudentList::deleteStudents()
{
   QMapIterator< unsigned int, QList< Student* > > i(students_);

   while (i.hasNext())
   {
      i.next();
      QList<Student*>::const_iterator j = i.value().begin();
      while (j != i.value().end())
      {
         delete *j;
         ++j;
      }
   }
}

