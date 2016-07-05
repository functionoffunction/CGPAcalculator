#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "cgpaf.h"

int create_student(students** head, char* name)
{
  students* temp1 = (students*) malloc(sizeof(students));
  if (temp1 == NULL) return 0; //can not dereference NULL

  students* temp2 = *head;
  *head = temp1;

    (*head)->name = name;
    (*head)->course = NULL;
    (*head)->another = temp2;

  return 1; //success
}

courses_list* get_course_address(courses_list* all, int course_index)
{
  int counter = 1;
  while (all != NULL) {
    if (counter == course_index) return all;
    all = all->next;
    counter++;
  }
  return NULL;
}
/*
@params course** head, courses_list* address , double score
returns 1 on new course record or replacement, 0 on malloc error
*/
int course_record(course** head, courses_list* address , double score){
  course* new = (course*) malloc(sizeof(course*));
  if (new == NULL) return 0;

  new->address  = address;
  new->student_score = score;
  new->next = NULL;

  if (*head == NULL ) { // fresh record
    *head = new;
    return 1;
  }

  course* temp = *head;
  while(temp != NULL){ // check for multiple insertion
    if (temp->address == address) {
      temp->student_score = score;
      free(new);
      return 1;
    }
    if (temp->next == NULL) {
      temp->next = new;
      break;
    }
      temp = temp->next;
  }
  return 1;
}

int print_count_all_courses(courses_list* all){
  int counter = 0;
  while (all != NULL) {
    printf("%d - %s\n", ++counter, all->course_code );
    all = all->next;
  }
  return counter;
}

int add_new_course(courses_list** all, char* course_code, int unit){

  courses_list* temp = *all;
  *all = malloc(sizeof(courses_list));
  if (all == NULL) return 0;

  (*all)->course_code = course_code;
  (*all)->unit = unit;

  if (temp == NULL) {
    (*all)->pos = 1;
  }else{
    (*all)->pos = temp->pos + 1;
  }

  (*all)->next = temp;
  return 1;
}

/*
@params:  students* head(address of head student linked list), int start_index_from(numbering index)
returns: last valid index or starting index if empty
*/
int print_all_students(students* head, int start_index_from) {
  if (head == NULL)return start_index_from-1; // returns previous, which is not null or starting index
  printf("%d - %s\n", start_index_from, head->name);
  return print_all_students(head->another, ++start_index_from);
}


students* get_student(int id, students* head){
  int counter = 0;
  while (head != NULL) {
    if (counter == id) return head;
    head = head->another;
    counter ++;
  }
  return NULL;
}

int print_student_courses(course* courses){

  int counter = 1;
  while(courses != NULL){
    printf("%d - %d - %s score: %lf\n", counter, courses->address->pos, courses->address->course_code, courses->student_score);
    courses = courses->next;
    counter++;
  }
  return counter;
}

course* get_student_course_by_id(int id, course* courses){
  int counter = 1;
  while (courses != NULL) {
    if (counter == id)break;
    courses = courses->next;
    counter++;
  }
  return courses;
}

int write_courses(FILE* fp, courses_list* all){
  int course_num = 0;
  while (all != NULL) {
    //printf("%s", strcat(all->course_code,",") ); //memory usage strcat O(n+m)  for two strings
    fprintf(fp, "%s,",  all->course_code); // no concatenation O(0)
    all = all->next;
    course_num++;
  }
  return course_num;
}

int grade_score(double score){
  if (score < 40) {//F
    return 0;
  }else if(score < 45){//E
    return 1;
  }else if(score < 50){//D
    return 2;
  }else if(score < 60){//C
    return 3;
  }else if(score < 70){//B
    return 4;
  }else{//A
    return 5;
  }
}

void write_student_record_csv(FILE* fp, course* courses, int number_of_courses){ //selection sort
  double cum_score = 0;
  double total_units = 0;
  double score;
  double unit;


  if (courses == NULL) return;
  course* temp = courses;

  int track = 0;
  int highest = 0;
  int last_pos = 0;
  int previous_highest = number_of_courses;

  while (temp != NULL) {
    int course_pos = temp->address->pos;
    if(!track){ // not tracked, first
      if ( course_pos > highest) {
        highest = course_pos;
        score = temp->student_score;
        unit  = temp->address->unit;
      }
    }else{
      if (course_pos < track) {
        if ( course_pos > highest){
          highest = course_pos;
          score = temp->student_score;
          unit  = temp->address->unit;
        }

      }
    }

    temp = temp->next;
    if (temp == NULL) {// last
      temp = courses; // start allover
      if (highest == 0) break; //comes back unchanged
      track = highest;


      while (previous_highest > highest) {// to put missing commas
          fputs(",", fp);
          previous_highest--;
        }



      fprintf(fp, "%lf", score);
      // fprintf(fp, "{%d}", highest);

      printf("%d\n", highest);
      cum_score = cum_score + (grade_score(score) * unit );
      total_units = total_units + unit ;
      highest = 0; //reset
    }
  }
  int remaining_cell_count = previous_highest +  1;
  while (remaining_cell_count > 1 ) {//trailing commas
    fputs(",", fp);
    remaining_cell_count--;
  }
  //prints cgpa and the terminates line
  fprintf(fp, "%lf\n", cum_score/total_units);
}
