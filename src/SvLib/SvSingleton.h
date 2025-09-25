/*
  Project "SvLib common library."
  Author
    Alexander Sibilev
  Internet
    www.salilab.com

  Description
    Singleton pattern

    We make constructor for some class private but public svInstance fuction which contains
    and return single pointer to class object

    class SomeClass {
        //SomeClass constructor is private to disable create class object outside svInstance function
        SomeClass() {}
      public:
        //Singleton function
        static SomeClass *instance() { static SomeClass obj; return &obj; }
      };

  History
    24.08.2022 Append SV_SINGLETON macro to simplify singleton declaration
               Refactor singleton as object and object ptr
    27.08.2024 Fix error: function template is wrong way
*/
#ifndef SVSINGLETON_H
#define SVSINGLETON_H

#define SV_SINGLETON( SomeClass ) static SomeClass *instance() { static SomeClass obj; return &obj; }

#endif // SVSINGLETON_H
