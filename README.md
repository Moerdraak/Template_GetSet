# Template_GetSet
A Pet project: Creating a Class that keeps track of same type objects 
to allow the user to Create() and Free() "Child" objects and to access the properties 
of the "Child" objects using Get() and Set() methods. Never worrying about new and delete.

This was driven by my desire to create a C++ environment which looks
similar to the matlab GUI code, since I'm busy translating a Matlab program
that I have written over many years (over 30 000 lines of code) into C++ 
using the SDL libraries. 

The first instance of this class is basically empty and only serves as
an Object/Instance manager for all the following "Child" Instances which
will hold the "actual" data. I realise there are other spiffy C++ libraries 
that I could have used to make it easier and faster, but I've done it this way
as an excercise in getting ontop of CLASSES, POINTERS and MEMORY LEAKS.

The user will create the Base object and the from there use it to:
Base.Create() : Creating a new Instance
Base.Set() : Set the properties of Instance 
Base.Get() : Get the properties of Instance
Base.Free() : Free and Delete the memory of an Instance
