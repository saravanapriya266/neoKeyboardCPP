   #include <pthread.h>
   #include <iostream>
   #include <termios.h>
   #include <unistd.h>
   using namespace std;
   template <class T>
   class SmartPtr {
       T* ptr;
       public:
	       explicit SmartPtr(T* p = NULL) { ptr = p; }
	       ~SmartPtr() { delete (ptr); }
	       T& operator*() { return *ptr; }
	       T* operator->() { return ptr; }
   };

   int getch() {
	    struct termios oldt, newt;
	    int ch;
	    tcgetattr(STDIN_FILENO, &oldt);
	    newt = oldt;
	    newt.c_lflag &= ~(ICANON | ECHO);
	    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	    ch = getchar();
	    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	    return ch;
   }

   static volatile bool keep_running = true;

   static volatile int getInput;

   static void* userInput_thread(void*)
   {
       while(keep_running) {
           if (getInput == 'q')
           {
               keep_running = false;
           }
       }
   }

   int main()
   {
      pthread_t tId, tId1;
      (void) pthread_create(&tId, 0, userInput_thread, 0);
      SmartPtr<int> ptr(new int());
      *ptr = 10; //Enter key
      while ( keep_running ) {
	      string result = "";
	      do {
		getInput = getch();
		result += getInput;
	      } while (getInput != int(*ptr));
	      if (getInput == int(*ptr)) {
		cout << result;
		result = "";
		getInput = 0;
	      }
      }
      (void) pthread_join(tId, NULL);

      return 0;
   }
