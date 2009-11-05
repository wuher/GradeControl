/*
 * xx.10.2009
 *
 * author: ?
 */



#include "loaddata.h"
#include "studentlist.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>



/**
 * todo: this implementation should be more robust
 */
bool loadData(QString fileName, StudentList* studentList)
{
   QFile text(fileName);

   if (text.open(QFile::ReadOnly | QIODevice::Text))
   {
      QTextStream data(&text);

      QString line = data.readLine().trimmed();

      while (!line.isEmpty())
      {
         if (line.at(0) == '#')
         {
            line = data.readLine();
         }
         else
         {
            QString studentNumber = line.section(':', 0, 0);
            QString points = line.section('(', 2, 2);
            points.remove(points.size()-1, 1);
            studentList->add(studentNumber.toUInt(), points.toUInt());
         }
         line = data.readLine();
      }
      text.close();

      return true;
   }
   else
   {
      return false;
   }
}


