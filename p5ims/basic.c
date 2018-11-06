/*
 ** cs154-2015 Project 5 ("p5ims") Instant Message Server
 ** basic.c: for functions that aren't called per-client-thread
 */

#include "ims.h"
#define PORT "1540"
#define BACKLOG 10


/* call serverStop() upon getting "q" or "quit" on stdin */
void readQuitFromStdin(void) {
    char *line=NULL;
    size_t lsize=0;
    while (1) {
        ssize_t glret;
        if (verbose) {
            printf("Type \"q\" or \"quit\" to cleanly quit the server\n");
        }
        glret = getline(&line, &lsize, stdin);
        /* Ctrl-D or EOF will also break out of this loop */
        if (glret <= 0 || !strcmp("quit\n", line) || !strcmp("q\n", line)) {
            /* tell things to quit gracefully */
            free(line);
            quitting = 1;
            /* anything else to do here? */
            break;
        }
        
        /* ... else !strcmp("foo\n", line) to see if user typed "foo" and then
         return. You can use this to add your own additional commands here, like
         for querying globals or other aspects of server's internal state */
        
    }
    return;
}

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s) {
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int serverStart(impEm *iem) {
  static const char me[]="serverStart";

  if (verbose > 1) {
    printf("%s: hi\n", me);
  }
  //read the database file
  if (udbaseRead(iem)) {
    impEmAdd(iem, "%s: failed to read database file \"%s\"",
             me, udbaseFilename);
    return 1;
  }
  /* immediately try writing database, so that any errors here can be
     reported as a failure of server start-up. Whether and how you do
     error handling for subsequent calls to udbaseWrite() is up to you */
    if (udbaseWrite(iem)) {
        impEmAdd(iem, "%s: failed to write database file \"%s\"",
                 me, udbaseFilename);
        return 1;
    }
    
    /* YOUR CODE HERE:
     -- create listening file descriptor for listenPort and listen() on it
     See http://beej.us/guide/bgnet/output/html/multipage/syscalls.html
     and May 18 2015 class slides
     -- start a thread to periodically save database
     -- figure out whether looking for "quit" on stdin should be done
     in a thread that is started here, or in main.c
     */
    
  int listenfd, connfd;  // listen on sock_fd, new connection on connfd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((listenfd = socket(p->ai_family, p->ai_socktype,
             p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes,
           sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(listenfd);
      perror("server: bind");
      continue;
    }
    break;
  }
  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }
  printf("server: listenfd = %d\n", listenfd);
  if (listen(listenfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections on port %s...\n", PORT);
  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    connfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
    if (connfd == -1) {
        perror("accept");
        exit(1);
    } else {
        //pthreadcreate(third arg is function name, fourth is input to that
        //function) need a function like THREAD
        //create a new thread
    }
    printf("server: connfd = %d\n", connfd);
    close(connfd);  // parent doesn't need this
  }


  if (verbose) {
    printf("%s: server started on part %u from %s\n",
           me, listenPort, udbaseFilename);
  }
  if (verbose > 1) {
    printf("%s: bye\n", me);
  }
  return 0;
}

void serverStop(void) {
    static const char me[]="serverStop";
    
    if (verbose > 1) {
        printf("%s: hi\n", me);
    }
    
    
    /* ... YOUR CODE HERE. What needs to be done to clean up
     resources created during server execution? */
    
    
    if (verbose > 1) {
        printf("%s: bye\n", me);
    }
    return;
}

