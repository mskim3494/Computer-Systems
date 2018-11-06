#include "ims.h"


int adding(int a, int b){
    return a+b;
}

// check if user exists within user LL
int checkDb(char* s, user_db* penis)
{
    user_db* curr= penis;
    while(curr){
        if(!strcmp(s, penis->name)){
            return 1;
        } curr=curr->next;
    } return 0;
}

//check if friend exists within friend LL 
int checkFriends(char* s, friends* f)
{
    friends* curr=f;
    while(curr){
        if(!strcmp(s,f->fname)){
            return 1;
        } curr=curr->next;
    } return 0;
}


FILE *fopener(const char *fname) {
  FILE *ret;
  if (!strcmp("-", fname)) {
    ret = stdin;
  } else {
    ret = fopen(fname, "r+");
    if (!ret) {
      fprintf(stderr, "couldn't open %s for reading: %s\n",
          fname, strerror(errno));
    }
  }
  return ret;
}

//initialize the new user on heap
user_db* newUser(char* s)
{
    user_db *user;
    user = (user_db *)malloc(sizeof(user_db));
    if (user == NULL) {
        fprintf(stderr, "failure to malloc memory\n");
        exit(1);
    }
    user->name = strdup(s);
    user->f = NULL;
    user->next=NULL;
    return user;
}

//initialize a friend on heap
friends* newFriend(char* s, int e)
{
    friends* n = (friends *)malloc(sizeof(friends));
    n->fname=strdup(s);
    n->fstatus=e;
    n->next=NULL;
    return n;
}

void registerFriend(user_db* db, char* s)
{
    //user_db* curr = db;  -- giving off unused error
    friends* newFriendV=NULL;
    friends* temporary=NULL;
    char* tmp = NULL;
    int e = 0; // status
    
    tmp = strstr(s, "requested");
    if(tmp!=NULL){
        e = 1;
    } tmp = NULL;
    tmp = strstr(s, "toanswer");
    if(tmp!=NULL){
        e = 2;
    } 
    newFriendV = newFriend(s, e);
    temporary = db->f;
    if(db->f==NULL){
        db->f = newFriendV;
    } while(temporary){
        if(temporary->next==NULL){
            temporary->next = newFriendV;
        } temporary = temporary->next;
    } 
}

//add user to end of linked list return beginning of the LL
user_db* registerUser(user_db* db, char* s)
{
    user_db* newNode = newUser(s);
    user_db* curr = db;
    //first insertion case
    if(db==NULL){
        db = newNode;
        return db;
    }
    
    while(curr){
     //traverse the LL
        if(curr->next==NULL){
              curr->next = newNode;
             break;
         }
         curr=curr->next;
    }
    //returning the added node
    return db;
}

int getliner(char **pline, size_t *psize, FILE *file) 
{
  ssize_t red;

  //red is number of chars red, return -1 on EOF, 0 empty
  red = getline(pline, psize, file);
  if (red <= -1) {
    fprintf(stderr, "hit EOF\n");
    return 1;
  }
  if (red <= 1) {
    fprintf(stderr, "got empty line\n");
    return 1;
  }
  if ('\n' != (*pline)[red-1]) {
    fprintf(stderr, "line didn't end with '\\n'\n");
    return 1;
  }
  (*pline)[red-1] = '\0';

  return 0;
}


//Read DB File into memory return beginning of LL
user_db* readDb(FILE* f){
   char* line =NULL; 
   size_t len = 0;
   user_db* beginning = NULL;

   char* user_pos;
   char* friend_pos;
   char* end_pos;
   while (!getliner(&line, &len, f)){
        user_pos = strchr(line, ':'); //first line
        friend_pos = strchr(line, '-'); //friend lines
        end_pos = strchr(line, '.'); //end user

        //register the user
        if (user_pos == NULL && friend_pos == NULL && end_pos == NULL){
            beginning = registerUser(beginning, line);
        } 
        //EOF
   }

   user_db* counter = beginning;
    //go back to top of file
   rewind(f);
   //register the friends
   while (!getliner(&line, &len, f)){
       user_pos = strchr(line, ':'); //first line
       friend_pos = strchr(line, '-');  //friend lines
       end_pos = strchr(line, '.'); //end user

       if (user_pos == NULL && friend_pos != NULL && end_pos == NULL){
            registerFriend(counter,line);           
       }
       
       //if end of user move pointer to next node
       if (end_pos != NULL) {
            counter=counter->next;
       }
       //EOF
   }
   return beginning;
}

void freeDb(user_db* db)
{
  user_db* curr1=db;
  friends* curr2;
  user_db* temp1;
  friends* temp2;
  while(curr1){
    curr2 = curr1->f;
    while(curr2){
      temp2 = curr2->next;
      free(curr2->fname);
      free(curr2);
      curr2 = temp2;
    } temp1 = db->next;
    free(db->name);
    free(db);
    curr1 = temp1;
  } return;
}

/* WRITING TO THE DATABASE  */
/****************************
 ****************************
 ****************************
 ****************************/

int getNumUsers(user_db* db) {
   int user_count = 0;
   user_db* curr = db;
   if (curr->next != NULL){
       while (curr){
           user_count++;
           curr = curr->next;
       }
   } 
   return user_count;
}

void getFriends(friends* f,FILE* file){
  friends* curr=f;
  if(curr==NULL){
    return;
  }
  char buf[2048];
  memset(buf, '\0', 2048);
  while(curr){
    fwrite(curr->fname, 1, strlen(curr->fname), file);
    // check friend status
    switch(curr->fstatus){
    case FRIEND:
      fwrite("\n",1,2,file);
      break;
    case REQUESTED:
      fwrite(" requested\n", 1, strlen(" requested\n"), file);
      break;
    case TOANSWER:
      fwrite(" toanswer\n", 1, strlen(" toanswer\n"),file);
      break;
    }
    curr=curr->next;
  }
}

//get the string for each user to write to DB
void getUserString(user_db* db, FILE* file) {
  user_db* curr=db;
  while(curr){
    fwrite(curr->name, 1, strlen(curr->name), file);
    fwrite("\n", 1, 2, file);
    getFriends(curr->f,file);
    fwrite(".\n",1,3,file); 
    curr=curr->next;
  }
}

void writeDb(FILE* f, user_db* db) {
    //write the first line
    char line_one[20];
    memset(line_one, '\0', 20);
    sprintf(line_one, "%d users:\n", getNumUsers(db));
    fwrite(line_one, 1, sizeof(line_one), f);
    
    //traverse the user LL and write for each user
    getUserString(db, f);
    return;
}

void fcloser(FILE *ff) {
  if (ff && stdin != ff) {
    fclose(ff);
  }
  return;
}



/* UPDATING THE DATABASE */

/* FRIEND REQUEST */

void friendRequest(char* sender, char* requested, user_db* db)
{
  user_db* curr = db;
  friends* tmp;
  int check=0;
  while(strcmp(sender, curr->name)){
    curr = curr->next;
  } // found the sender, look for redundancies
  tmp = curr->f;
  check = checkFriends(requested,tmp);
    if(check){
    return 3;
    // there is a redundancy
    } char buffer[strlen(requested)+15];
    memset(buffer,'\0', sizeof(buffer));
    strcpy(buffer, requested);
    strcat(buffer, " requested");
    registerFriend(curr, buffer);
    
    // add request to the requested

    curr = db;
    while(strcmp(requested, curr->name)){
      curr = curr->next;
    } // found requested
    if(curr == NULL){
      fprintf(stderr, "USER NOT FOUND");
      return 1;
    } char buffer[strlen(sender)+15];
    memset(buffer,'\0', sizeof(buffer));
    strcpy(buffer, sender);
    strcat(buffer, " toanswer");
    registerFriend(curr, buffer);

    return 0;
    // successful completion
}
