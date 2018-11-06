#include <stdio.h>
#include "dbstart.c"
#include "minunit.h"


/*
 static char* test_adding(){
 mu_assert("error, adding doesnt work", adding(3,4)==7);
 return 0;
 }
 */

void print_db(user_db* db) {
    printf("\n\n*****printing db*****\n\n");
    user_db* curr = db;
    friends* counter = db->f;
    while (curr){
        printf("%s\n", curr->name);
        if (curr->f != NULL){
            counter = curr->f;
            printf("friends:\n");
            printf("%s\n", counter->fname);
        }
        curr=curr->next;
    }
}

/*
 static char* test_parsing_db(){
 readDb(fopener("read_test.txt"));
 printf("we here");
 writeDb(fopener("write_test.txt"), readDb(fopener("read_test.txt")));
 return 0;
 }
 
 static char* all_tests() {
 mu_run_test(test_adding);
 return 0;
 }
 
 */

int main(){
    user_db* db = readDb(fopener("read_test.txt"));
    print_db(db);
    getNumUsers(db);
    printf("\n\n\n\n\n\n");
    printf("%s", getUserString(db));
    return 0;
}
/*
 
 int main(int argc, char**argv) {
 char *result = all_tests();
 if (result != 0) {
 printf("%s\n", result);
 }
 else {
 printf("ALL TESTS PASSED\n");
 }
 printf("Tests run: %d\n", tests_run);
 
 return result != 0;
 }
 */
