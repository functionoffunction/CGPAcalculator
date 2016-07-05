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
#include <unistd.h>

#include "cgpaf.h"

int main(void)
{
  students* head = NULL;
  courses_list* all = NULL;

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
        printf("Enter Student name: ");
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
              courses_list* course_address = get_course_address(all, course_index);
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
        printf("Id- Name\n");
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
          students* student = get_student(id - start_from, head);

          if (student == NULL){
            printf("Allocation Error Occured");
            break;
          }

          int edit_student_op = 0;
          do {

            printf("Number - Option\n");
            printf("---------------\n");
            printf("1      - Edit Name\n");
            printf("2      - Edit/show Record\n");
            printf("0      - Done\n");
            printf("Enter your Option: ");
            scanf("%d", &edit_student_op);

            switch (edit_student_op) {
              case 1 :
                printf("Enter A New Name Number: ");
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
                 course* selected_course = get_student_course_by_id(id, student->course);
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

        doc_name = strcat(doc_name,".csv");

        //check if file exist
        while ( access(doc_name, F_OK ) != -1 ) {
          int key;
          printf("File already exist, 0 enter to over Write, any other key to rename");
          scanf("%d", &key);
          if (key == 0) break;

          // renaming
          printf("Enter anther name: ");
          scanf("%s", doc_name);
          doc_name = strcat(doc_name,".csv");
        }


        FILE *fp;
        fp = fopen(doc_name, "w+");

        // prints heading
        fputs("s/n,Name,", fp);
        int number_of_courses = write_courses(fp, all);
        fputs("CGPA\n", fp);

        // prints body
        students* temp = head;
        int serial_number = 0;
        while (temp != NULL) {
          fprintf(fp, "%d,", ++serial_number);
          fprintf(fp, "%s,", temp->name);
          write_student_record_csv(fp, temp->course, number_of_courses);
          temp = temp->another;
        }
        fclose(fp);
        printf("File exported as %s\n", doc_name);

      }
    }


  } while(operation != 0);

  return 0;
}
