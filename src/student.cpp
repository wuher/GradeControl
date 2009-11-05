#include "student.h"
#ifndef STUDENT_CPP
#define STUDENT_CPP

Student::Student(unsigned int studentNumber, unsigned int examPoints)
{

    number_ = studentNumber;
    points_ = examPoints ;
}

Student::~Student() {

}

unsigned int Student::getPoints( ) const
{
    return points_;
}

bool Student::operator< ( const Student& st )
{
    if( st.points_ > points_ )
    {
        return true;
    }
    return false;
}

#endif // STUDENT_CPP
