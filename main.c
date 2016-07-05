/********************************************************
*OLUWATIMILEHIN AKOGUN                                  *
*Title: CGPA Calculator with Multidata linked lisk      *
*version: 1.0                                           *
*Github: functionOfFunction                             *
*Email: akoguntimi@gmail.com                            *
*License: Open source                                   *
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct courses_list {
  //data
  char* course_code;
  int unit;
  int pos;
  //link
  struct courses_list* next;
};

struct course
{
  //data
  struct courses_list* address;
  double student_score;
  //link
  struct course* next;
}course;

struct students
{
  //data
  char* name;
  struct course* course;
  //link
  struct students* another;
}students;

int create_student(struct students** head, char* name)
{
  struct students* temp1 = (struct students*) malloc(sizeof(struct students));
  if (temp1 == NULL) return 0; //can not dereference NULL

  struct students* temp2 = *head;
  *head = temp1;

    (*head)->name = name;
    (*head)->course = NULL;
    (*head)->another = temp2;

  return 1; //success
}

struct courses_list* get_course_address(struct courses_list* all, int course_index)
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
@params struct course** head, struct courses_list* address , double score
returns 1 on new course record or replacement, 0 on malloc error
*/
int course_record(struct course** head, struct courses_list* address , double score){
  struct course* new = (struct course*) malloc(sizeof(struct course*));
  if (new == NULL) return 0;

  new->address  = address;
  new->student_score = score;
  new->next = NULL;

  if (*head == NULL ) { // fresh record
    *head = new;
    return 1;
  }

  struct course* temp = *head;
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

int print_count_all_courses(struct courses_list* all){
  int counter = 0;
  while (all != NULL) {
    printf("%d - %s\n", ++counter, all->course_code );
    all = all->next;
  }
  return counter;
}

int add_new_course(struct courses_list** all, char* course_code, int unit){

  struct courses_list* temp = *all;
  *all = malloc(sizeof(struct courses_list));
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
@params:  struct students* head(address of head student linked list), int start_index_from(numbering index)
returns: last valid index or starting index if empty
*/
int print_all_students(struct students* head, int start_index_from) {
  if (head == NULL)return start_index_from-1; // returns previous, which is not null or starting index
  printf("%d - %s\n", start_index_from, head->name);
  return print_all_students(head->another, ++start_index_from);
}


struct students* get_student(int id, struct students* head){
  int counter = 0;
  while (head != NULL) {
    if (counter == id) return head;
    head = head->another;
    counter ++;
  }
  return NULL;
}

int print_student_courses(struct course* courses){

  int counter = 1;
  while(courses != NULL){
    printf("%d - %d - %s score: %lf\n", counter, courses->address->pos, courses->address->course_code, courses->student_score);
    courses = courses->next;
    counter++;
  }
  return counter;
}

struct course* get_student_course_by_id(int id, struct course* courses){
  int counter = 1;
  while (courses != NULL) {
    if (counter == id)break;
    courses = courses->next;
    counter++;
  }
  return courses;
}

int write_courses(FILE* fp, struct courses_list* all){
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

void write_student_record_csv(FILE* fp, struct course* courses, int number_of_courses){ //selection sort
  double cum_score = 0;
  double total_units = 0;
  double score;
  double unit;


  if (courses == NULL) return;
  struct course* temp = courses;

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

int main(void)
{
  struct students* head = NULL;
  struct courses_list* all = NULL;

  int operation = 0;

  do {
    printf("\n");
    printf("Options         - Number:\n");
    printf("-------------------------\n");
    printf("Create student  - 1\n");
    printf("Add New Course  - 2\n");
    printf("Add edit student- 3\n");
    printf("Export as .csv  - 4\n");
    printf("Done            - 0\n");
    printf("select an Options: ");
    scanf("%d", &operation);
    printf("\n");

    switch (operation) {

      case 1:
        printf("Enter Student Matric Number: ");
        char* name = malloc(sizeof(char)*256);
        scanf("%s", name);


        if (!create_student(&head, name)) return 1; //was not created

        int course_operation = 0;
        do {
          printf("\n");
          printf("Student option  - Number:\n");
          printf("-------------------------\n");
          printf("Record Course   - 1\n");
          printf("Add New Course  - 2\n");
          printf("Done            - 0\n");
          printf("select a operations: ");
          scanf("%d", &course_operation);


          switch (course_operation) {
            case 1 :

              printf("Course by Index\n");
              printf("---------------\n");
              int course_num = print_count_all_courses(all);
              int course_index;

              printf("Enter the course by index: ");
              scanf("%d", &course_index);
              printf("\n");
              if (course_index > course_num || course_index <= 0){
                printf("Error! invalid index\n");
                break;
              }
              struct courses_list* course_address = get_course_address(all, course_index);
              printf("Enter score: ");
              double score;
              scanf("%lf", &score);
              printf("\n");

              if (!course_record(&(head->course), course_address , score)) printf("New Score Allocation error");
              break;

            case 2 :
              printf("Course code: ");
              char* course_code = (char*) malloc(sizeof(char)*256);
              scanf("%s", course_code);

              printf("Enter the Number of unit: ");
              int unit;
              scanf("%d", &unit);
              add_new_course(&all, course_code,  unit);

              break;
          }

        } while(course_operation != 0);

        break;
      case 2:
      {
        int add_course = 1;
        do {
          printf("Course code: ");
          char* course_code = (char*) malloc(sizeof(char)*256);
          scanf("%s", course_code);

          printf("Enter the Number of unit: ");
          int unit;
          scanf("%d", &unit);
          add_new_course(&all, course_code,  unit);

          printf("Enter 0 if done, any other number to add another course ");
          scanf("%d", &add_course);
        } while(add_course != 0);

        break;
      }
      case 3:
        printf("Id- Matriculation Number\n");
        printf("------------------------\n");
        int start_from = 1;
        int all_students = print_all_students(head, start_from);
        printf("Enter Id of student you want to edit: ");
        int id;
        scanf("%d", &id);
        if (id > all_students || id < start_from){
          printf("Invalid Id selected\n");
        }else{
          printf("Edit student Options\n");
          printf("%d\n", id - start_from );
          struct students* student = get_student(id - start_from, head);

          if (student == NULL){
            printf("Allocation Error Occured");
            break;
          }

          int edit_student_op = 0;
          do {

            printf("Number - Option\n");
            printf("---------------\n");
            printf("1      - Edit Matriculation Number\n");
            printf("2      - Edit/show Record\n");
            printf("0      - Done\n");
            printf("Enter your Option: ");
            scanf("%d", &edit_student_op);

            switch (edit_student_op) {
              case 1 :
                printf("Enter A New Matric Number: ");
                char* new_mat_no = malloc(sizeof(char)*256);
                scanf("%s", new_mat_no);
                if (new_mat_no == NULL){
                  printf("Allocation Error Occured");
                  break;
                }
                student->name = new_mat_no;
                break;
              case 2 :
              {
                 int record_count = print_student_courses(student->course);
                 int edit_course_op;
                 printf("To edit score, enter the index of course or 0 to go to menu\n");
                 scanf("%d",&edit_course_op);
                 if (edit_course_op  == 0) break;
                 if ((edit_course_op < 0) || (edit_course_op > record_count)) {
                   printf("Invalid Option\n");
                   break;
                 }
                 struct course* selected_course = get_student_course_by_id(id, student->course);
                 double new_score;
                 printf("Enter a new Score for %s : ", selected_course->address->course_code );
                 scanf("%lf",&new_score);
                 selected_course->student_score = new_score;
                 printf("Score Successfully changed");
                 break;
              }
            }

          } while(edit_student_op != 0);

        }
        break;
        case 4:
      {
        char* doc_name = (char*) malloc(sizeof(char)*256);
        printf("Enter document name(*.csv) for export: ");
        scanf("%s", doc_name);

        // add check if file exist, prompt for replacement or rename


        FILE *fp;
        fp = fopen(doc_name, "w+");

        // prints heading
        fputs("s/n,Matric Number,", fp);
        int number_of_courses = write_courses(fp, all);
        fputs("CGPA\n", fp);

        // prints body
        struct students* temp = head;
        int serial_number = 0;
        while (temp != NULL) {
          fprintf(fp, "%d,", ++serial_number);
          fprintf(fp, "%s,", temp->name);
          write_student_record_csv(fp, temp->course, number_of_courses);
          temp = temp->another;
        }
        fclose(fp);
      }
    }


  } while(operation != 0);

  return 0;
}
