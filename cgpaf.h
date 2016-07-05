/********************************************************
*OLUWATIMILEHIN AKOGUN                                  *
*Title: CGPA Calculator with Multidata linked lisk      *
*version: 1.0                                           *
*Github: functionOfFunction                             *
*Email: akoguntimi@gmail.com                            *
*License: Open source                                   *
*********************************************************/


#ifndef _CGPAF_H
#define _CGPAF_H

#include <stdlib.h>

struct courses_list {
  //data
  char* course_code;
  int unit;
  int pos;
  //link
  struct courses_list* next;
};

typedef struct courses_list courses_list;


struct course
{
  //data
  courses_list* address;
  double student_score;
  //link
  struct course* next;
};

typedef struct course course;


struct students
{
  //data
  char* name;
  course* course;
  //link
  struct students* another;
};

typedef struct students students;


int
create_student(students** head, char* name);


courses_list*
get_course_address(courses_list* all, int course_index);

/*
args: course** head, courses_list* address , double score
returns: 1 on new course record or replacement, 0 on malloc error
*/
int
course_record(course** head, courses_list* address , double score);

int
print_count_all_courses(courses_list* all);

int
add_new_course(courses_list** all, char* course_code, int unit);

/*
args:  students* head(address of head student linked list), int start_index_from(numbering index)
returns: last valid index or starting index if empty
*/
int
print_all_students(students* head, int start_index_from);


students*
get_student(int id, students* head);

int
print_student_courses(course* courses);

course*
get_student_course_by_id(int id, course* courses);

int
write_courses(FILE* fp, courses_list* all);

int
grade_score(double score);

void
write_student_record_csv(FILE* fp, course* courses, int number_of_courses);

#endif
