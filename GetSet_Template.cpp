/*  A Pet project; Creating a Class that keeps track of same type objects
	to allow the user to access the "Child" objects using Get() and Set() methods.
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
*/
#include <stdio.h>
#include <iostream>

/*** Define these to give easy to remember types for Get() & Set() ***/
const int tId = 0;
const int tSize = 1;
const int tAge = 2; 
const int tWeight = 3;
const std::string tName = "Name";
const std::string tDescr = "Descr";

/***  Define class Mammal  ***/
class Mammal {
private:
	Mammal** Ptr = NULL; // This Pointer will keep track of all the Objects created by the Base Object
	int Id = -1; // Used together with Size
	int Size = 0; // Counter keeping track of Objects created - But for the user it reports the Size
public:

	// Variables used for the "Child" object properties
	int Weight = NULL; 
	int Age = NULL;
	std::string Name;
	std::string Descr;

	// Constructor - In this scenario only used by the Base Object
	Mammal() {
		Name.clear(); // Not really necessary ??
	};

	// Destructor
	~Mammal() {
		if (Id == -1) { // The Base Object has Id -1 and only the Base object can have Children
			for (int I = 0; I < Size; I++) { // For Loop to delete all Objects before deleting the Base Object
				delete Ptr[I];
			}
			if (Size > 0) { // Just in case user free'd all child Instances already
				delete Ptr;
			}
		}
	};

	// Functions
	Mammal* Create(std::string NewName);
	void Free(void) {  // Free's the whole Base Instance and children Instances
		delete this;
	}; 
	void Free(std::string Name); // Free one of the Child Instances by Name
	void Free(int Id); // Free one of the Child Instances by Id
	int Get(int Type) { // Returns the number of Child Instances for the Base Instance
		if (Type == tSize) {
			return Size;
		}
	}
	
	// Reduce this using templates ?!?
	// Get property of Child Instance - ( x4 Overloads)
	int Get(std::string Obj, int Type); 

	int Get(int ObjId, int Type);
	std::string Get(std::string Obj, std::string Type);
	std::string Get(int ObjId, std::string Type);
	
	// Set property of Child Instance - ( x4 Overloads)
	//template <class VarType, class VarVal>
	bool Set(std::string Obj, int Var, int Val); 
	bool Set(std::string Obj, std::string Var, std::string Val);
	bool Set(int ObjId, int Var, int Val);
	bool Set(int ObjId, std::string Var, std::string Val);
};
	


/****************************************************************************************
										MAIN
****************************************************************************************/
int main()
{
	while(1){
		Mammal Cats;

		// In this Mammal class, Create() returns a Pointer to the newly created class. 
		// This might be usefull but obviously dangerous because if you delete this pointer instead of using the Free() command.
		// It's gonna break the memory. So rather don't use the following format. (And remove the return from Create() function)

		Mammal* MyCat = Cats.Create("Ginger"); // Add a new cat with his name;
		MyCat->Age = 7;  // You can now access the variables directly like this
	
		std::cout << "My cat's name is " << MyCat->Name << " and he is " << MyCat->Age << " years old." << std::endl;

		//// The preffered way of using the Mammal class

		Cats.Create("Socks"); // Use the Create function to add a new cat
		Cats.Create("Garfield");

		Cats.Set("Socks", tAge, 13);  // void Set(string <InstanceName>, string <Property>, Value)
		Cats.Set("Socks", tWeight, 8);  // void Set(string <InstanceName>, string <Property>, Value)
		Cats.Set("Socks", tDescr, "Dark Brown");  // void Set(string <InstanceName>, string <Property>, Value)

		Cats.Set("Garfield", tAge, 22);
		Cats.Set("Garfield", tWeight, 16);
		Cats.Set("Garfield", tDescr, "Ginger stripes");

		Cats.Set("Ginger", tWeight, 3); // We haven't given Ginger a weight previously
		Cats.Set("Ginger", tDescr, "Black");

		std::cout << std::endl;

		std::cout << Cats.Get("Socks", tName) << " is " << Cats.Get("Garfield", tAge) << " years old and weigh " << Cats.Get("Ginger", tWeight) << " kg." << std::endl;

		for (int I = 0; I < Cats.Get(tSize); I++) {
			std::cout << Cats.Get(I, tName) << " is " << Cats.Get(I, tAge) << " years old, " << Cats.Get(I, tDescr) << " and weigh " << Cats.Get(I, tWeight) << " kg." << std::endl;
		}

		Cats.Free("Garfield"); // Removing an Object by Name
		Cats.Free(0); // Removing an Object by Id
		Cats.Create("New");

		// Do the same but this time create pointer to Base Instance
		Mammal* Dogs = new Mammal;
		Dogs->Create("Sparky");
		Dogs->Set("Sparky", tAge, 13);
		Dogs->Set("Sparky", tDescr, "Black & White");
		Dogs->Set("Sparky", tWeight, 9);

		Dogs->Create("Foxy");
		Dogs->Set("Foxy", tWeight, 24);
		Dogs->Set(1, tAge, 2);
		Dogs->Set(1, tDescr, "White");

		Dogs->Create("Eddie");
		Dogs->Set("Eddie", tAge, Dogs->Get("Sparky", tWeight));

		Dogs->Create("Spot");
		Dogs->Set("Spot", tAge, 5);


		std::cout << "Eddie's Age is: " << Dogs->Get("Eddie", tAge) << std::endl;
		std::cout << "Spot's Id is: " << Dogs->Get("Spot", tId) << std::endl;

		Dogs->Free("Foxy");

		std::cout << Dogs->Get(2, tName) << " chases " << Cats.Get(0, tName) << std::endl;

		Dogs->Free(); // Free's all of Dogs Memory

		}

	return 0;
}

/*---------------------------------------------------------------
	FUNCTION: Create "Child" Instance
---------------------------------------------------------------*/
Mammal* Mammal::Create(std::string NewName) {
	
	if (Id != -1) {		// ONLY FIRTS INSTANCE may create New ones.
		return NULL;	// Athough it is impossible for a Child Instance to call Create 
	}					// So I can safely remove this piece of code
	
	// Check that it is not a duplicate Name
	for (int I = 0; I < Size; I++) {
		if (NewName.find(Ptr[I]->Name) == 0) {
			return NULL;
		}
	}
	Mammal* New = new Mammal; // Create Child Instance

	New->Id = Size;
	New->Name.assign(NewName);
	if (Size == 0) {
		Ptr = new Mammal*; // Create first Instance of pointer to "Mammal pointer"
		Ptr[Size++] = New; // Assign Child Instance to this pointer
	}
	else {
		Mammal** Tmp = new Mammal * [(double)Size + 1]; // Assign new memmory one bigger than before
		for (int I = 0; I < Size; I++) {
			Tmp[I] = Ptr[I]; 
		}
		delete Ptr;
		Ptr = Tmp;
		Ptr[Size++] = New;
	}
	// Should not return this pointer gives user power to break things, But just for S&Gs I'll leave it in
	return New; 
}

/*---------------------------------------------------------------
FUNCTION: Free Instance by Instance Name
---------------------------------------------------------------*/
void Mammal::Free(std::string Name) {
	bool flag = false;

	Mammal** Tmp = new Mammal * [(double)Size - 1]; // Assign new memmory one smaller than before
	int Len = Size; // Because Size is going to change
	for (int I = 0; I < Len; I++) {
		if (flag == false) {
			if (Ptr[I]->Name.find(Name, 0) == 0) {
				flag = true;
				Size--;
				delete Ptr[I]; // Delete this specific memory block

				if (I < Len - 1) {  // If it is the last one there's nothing to move
					Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
					Tmp[I]->Id--; // Decrement Id as well
				}
			}
			else {
				if (I < Len - 1) {  // If it is the last one there's no memory available in Tmp
					Tmp[I] = Ptr[I]; // And clearly the Instance was not found
				}					 // So the call to Free() was wrong
			}
		}
		else {
			if (I < Len - 1) {  // If it is the last one there's nothing to move
				Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
				Tmp[I]->Id--; // Decrement Id as well
			}
		}
	}	
	if (flag == true) {
		delete Ptr; // Delete old Memory
		Ptr = Tmp;  // Set Ptr to new Memory
	}
	else {
		delete Tmp;
	}
	// If it is the last Child Instance also delete this
	if(Size == 0){
		delete Ptr;
	}
}

/*---------------------------------------------------------------
FUNCTION: Free Instance by Id
---------------------------------------------------------------*/
void Mammal::Free(int ObjId) {
	if (ObjId < Size) {
		Free(Ptr[ObjId]->Name);
	}
}

/*---------------------------------------------------------------
FUNCTION: Get()  (Overload 1)
	Get Property by Instance Name 
	Return: Int 
---------------------------------------------------------------*/
int Mammal::Get(std::string Obj, int Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			switch (Type) {
			case tId:
				return Ptr[I]->Id;
				break;
			case tAge: 
				return Ptr[I]->Age;
				break;
			case tWeight: 
				return Ptr[I]->Weight;
				break;
			default:
				return NULL;
			}
		}
	}
	return NULL;
}

/*---------------------------------------------------------------
FUNCTION: Get()  (Overload 3)
	Get Property by Instance Name
	Return: string
---------------------------------------------------------------*/
std::string Mammal::Get(std::string Obj, std::string Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			if (Type.find("Name", 0) == 0) {
				return Ptr[I]->Name;
			}
			else if (Type.find("Descr", 0) == 0) {
				return Ptr[I]->Descr;
			}
		}
	}
	return "";
}

/*---------------------------------------------------------------
FUNCTION: Get()  (Overload 2)
	Get Property by Instance Id
	Return: Int
---------------------------------------------------------------*/
int Mammal::Get(int ObjId, int Type) {
	if (ObjId < Size) {
		return Get(Ptr[ObjId]->Name, Type);
	}
	else {
		return NULL;
	}
}
/*---------------------------------------------------------------
FUNCTION: Get()  (Overload 4)
	Get Property by Instance Id
	Return: string
---------------------------------------------------------------*/
std::string Mammal::Get(int ObjId, std::string Type) {
	if (ObjId < Size) {
		return Get(Ptr[ObjId]->Name, Type);
	}
	else {
		return "";
	}
}

/*---------------------------------------------------------------
FUNCTION: Set()   (Overload 1)
	Set String Property, Reference object by Name
---------------------------------------------------------------*/
//template <class VarType, class VarVal>
bool Mammal::Set(std::string Obj, std::string  Var, std::string Val) {

	bool Flag = false;

	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			if (Var.find("Name", 0) == 0) { // Don't  allow user to change the Name
		//		Ptr[I]->Name.assign(Val);	// It makes Free-ing the Instances hard to keep track of.
		//		Flag = true;            
			}
			else if (Var.find("Descr", 0) == 0) {
				Ptr[I]->Descr.assign(Val);
				Flag = true;
			}
		}	
	}
	return Flag;
}

/*---------------------------------------------------------------
FUNCTION: Set()   (Overload 2)
	Set Int Property, Reference object by Name
---------------------------------------------------------------*/
// template <class VarType, class VarVal>
bool Mammal::Set(std::string Obj, int Var, int Val) {

	bool Flag = false;

	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			if (Var == tAge) {
				Ptr[I]->Age = Val;
				Flag = true;
			}
			else if (Var == tWeight) {
				Ptr[I]->Weight = Val;
				Flag = true;
			}
		}
	}
	return Flag;
}

/*---------------------------------------------------------------
FUNCTION: Set()   (Overload 4)
	Set String Property, Reference object by Id
---------------------------------------------------------------*/
bool Mammal::Set(int ObjId, std::string  Var, std::string Val) {
	bool Flag = false;
	if (ObjId < Size) {
		Flag = Set(Ptr[ObjId]->Name, Var, Val);
	}
	return Flag;
}

/*---------------------------------------------------------------
FUNCTION: Set()   (Overload 4)
	Set Int Property, Reference object by Id
---------------------------------------------------------------*/
bool Mammal::Set(int ObjId, int Var, int Val) {
	bool Flag = false;
	if (ObjId < Size) {
		Flag = Set(Ptr[ObjId]->Name, Var, Val);
	}
	return Flag;
}