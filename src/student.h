#ifndef STUDENT_H
#define STUDENT_H

#include <QPainter>
#include <QWidget>

class Student : public QWidget
{
     Q_OBJECT

public:

    Student(unsigned int studentNumber, unsigned int examPoints);
    ~Student();
    unsigned int getPoints( ) const;

    bool operator <( const Student& st );

private:

    unsigned int number_;
    unsigned int points_;

};

#endif // STUDENT_H
