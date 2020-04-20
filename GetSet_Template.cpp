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
#include <any>
#include <iostream>

/*** Define these to give easy to remember types for Get() & Set() ***/
const int tAge = 0; 
const int tWeight = 1;
const std::string tName = "Name";
const std::string tColor = "Color";

/***  Define class Mammal  ***/
class Mammal {
public:
	int Id = -1; // Used together with Size
	int Size = 0; // Counter keeping track of Objects created - But for the user it reports the Size
	Mammal** Ptr = NULL; // This Pointer will keep track of all the Objects created by the Base Object

	// Variables used for the "Child" object properties
	int Weight = NULL; 
	int Age = NULL;
	std::string Name;
	std::string Color;

	// Constructor - In this scenario only used by the Base Object
	Mammal() {
		Name.clear(); // Not really necessary ??
	};

	// Destructor
	~Mammal() {
		if (Id == -1) { 
			for (int I = 0; I < Size; I++) { // For Loop to delete all Objects before deleting the Base Object
				delete Ptr[I];
			}
		}
	};

	// Functions
	Mammal* Create(std::string NewName);
	void Free(std::string Name);
	void Free(int Id);
	int Get(std::string Obj, int Type);
	int Get(int GetId, int Type);
	std::string Get(std::string Obj, std::string Type);
	std::string Get(int GetId, std::string Type);
	//template <class VarType, class VarVal>
	bool Set(std::string Obj, int Var, int Val); // Reduce this using templates ?!?
	bool Set(std::string Obj, std::string Var, std::string Val);
};
	


/****************************************************************************************
										MAIN
****************************************************************************************/
int main()
{

	Mammal Cats;

	// In this template the Create returns a Pointer to the newly cretaed class. 
	//This might be usefull but obviously dangerous because if you delete this pointer instead of using the Free() command.
	//It's gonna break the memory. So rather don't use the following format. (And remove the return from Create() function)

	Mammal* MyCat = Cats.Create("Ginger"); // Add a new cat with his name;
	MyCat->Age = 7;  // You can now access the variables directly like this

	std::cout << "My cat's name is " << MyCat->Name << " and he is " << MyCat->Age << " years old." << std::endl;

	// The preffered way of using the Mammal class

	Cats.Create("Socks"); // Use the Create function to add a new cat
	Cats.Create("Garfield");

	Cats.Set("Socks", tAge, 13);  // void Set(string <InstanceName>, string <Property>, Value)
	Cats.Set("Socks", tWeight, 8);  // void Set(string <InstanceName>, string <Property>, Value)
	Cats.Set("Socks", tColor, "Dark Brown");  // void Set(string <InstanceName>, string <Property>, Value)

	Cats.Set("Garfield", tAge, 22);
	Cats.Set("Garfield", tWeight, 16);
	Cats.Set("Garfield", tColor, "Ginger stripes");

	Cats.Set("Ginger", tWeight, 3); // We haven't given Ginger a weight previously
	Cats.Set("Ginger", tColor, "Black");

	std::cout << std::endl;

	std::cout << Cats.Get("Socks", tName) << " is " << Cats.Get("Garfield", tAge) << " years old and weigh " << Cats.Get("Ginger", tWeight) << " kg." << std::endl;


	for (int I = 0; I < Cats.Size; I++){
		std::cout << Cats.Get(I, tName) << " is " << Cats.Get(I, tAge) << " years old, " << Cats.Get(I, tColor) << " and weigh " << Cats.Get(I, tWeight) << " kg." << std::endl;
	}

	Cats.Free("Socks"); // Removing an Object from the list and freeing the memory
	Cats.Free("Ginger");

	return 0;
}




/*---------------------------------------------
		FUNCTION: Create "Child" Instance
	----------------------------------------------*/
Mammal* Mammal::Create(std::string NewName) {
	Mammal* New = new Mammal;
	if (Id != -1) { // ONLY FIRTS INSTANCE may create New ones
		return NULL;
	}

	// Prevent duplicate Names 
	for (int I = 0; I < Size; I++) {
		if (NewName.find(Ptr[I]->Name) == 0) {
			return NULL;
		}
	}

	New->Id = Size;
	New->Name.assign(NewName);
	if (Size == 0) {
		Ptr = new Mammal*;
		Ptr[Size] = new Mammal;
		Ptr[Size++] = New;
	}
	else {
		Mammal** Tmp = new Mammal * [Size + 1]; // Assign new memmory one bigger than before
		for (int I = 0; I < Size; I++) {
			Tmp[I] = Ptr[I];
		}
		Tmp[Size] = new Mammal;
		delete Ptr;
		Ptr = Tmp;
		Ptr[Size++] = New;
	}
	return New;
}

/*---------------------------------------------
FUNCTION: Free Instance by Instance Name
----------------------------------------------*/
void Mammal::Free(std::string Name) {
	bool flag = false;
	Mammal** Tmp = new Mammal * [Size - 1]; // Assign new memmory one smaller than before
	for (int I = 0; I < Size - 1; I++) {
		if (flag == false) {
			if (Ptr[I]->Name.find(Name, 0) == 0) {
				flag = true;
				Size--;
				Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
				Tmp[I]->Id--; // Decrement Id as well
			}
			else {
				Tmp[I] = Ptr[I]; // Copy same position over from Ptr to Tmp
			}
		}
		else {
			Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
			Tmp[I]->Id--; // Decrement Id as well
		}
	}
	delete Ptr; // Delete old Memory
	Ptr = Tmp;  // Set Ptr to new Memory
}

/*---------------------------------------------
FUNCTION: Free Instance by Id
----------------------------------------------*/
void Mammal::Free(int Id) {
	bool flag = false;
	Mammal** Tmp = new Mammal * [Size - 1]; // Assign new memmory one smaller than before
	for (int I = 0; I < Size - 1; I++) {
		static int J = 0;
		if (flag == false) {
			if (Ptr[I]->Id == Id) {
				flag = true;
				Size--;
				Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
				Tmp[I]->Id--; // Decrement Id as well
			}
			else {
				Tmp[I] = Ptr[I]; // Copy same position over from Ptr to Tmp
				Tmp[I]->Id--; // Decrement Id as well
			}
		}
		else {
			Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
			Tmp[I]->Id--; // Decrement Id as well
		}
	}
	delete Ptr; // Delete old Memory
	Ptr = Tmp;  // Set Ptr to new Memory
}

/*---------------------------------------------
FUNCTION: Get()  (Overload 1)
	Get Property by Instance Name 
	Return: Int 
----------------------------------------------*/

int Mammal::Get(std::string Obj, int Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			switch (Type) {
			case 0: // tAge = 0;
				return Ptr[I]->Age;
				break;
			case 1: // tWeight = 1
				return Ptr[I]->Weight;
				break;
			default:
				return NULL;
			}
		}
	}
	return NULL;
}
/*---------------------------------------------
FUNCTION: Get()  (Overload 2)
	Get Property by Instance Id
	Return: Int
----------------------------------------------*/

int Mammal::Get(int GetId, int Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Id == GetId) {
			switch (Type) {
			case 0: // tAge = 0;
				return Ptr[I]->Age;
				break;
			case 1: // tWeight = 1
				return Ptr[I]->Weight;
				break;
			default:
				return NULL;
			}
		}
	}
	return NULL;
}
/*---------------------------------------------
FUNCTION: Get()  (Overload 3)
	Get Property by Instance Name
	Return: string
----------------------------------------------*/

std::string Mammal::Get(std::string Obj, std::string Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			if (Type.find("Name", 0) == 0) {
				return Ptr[I]->Name;
			}
			else if (Type.find("Color", 0) == 0) {
				return Ptr[I]->Color;
			}
		}
	}
	return NULL;
}

/*---------------------------------------------
FUNCTION: Get()  (Overload 4)
	Get Property by Instance Id
	Return: string
----------------------------------------------*/
std::string Mammal::Get(int GetId, std::string Type) {
	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Id == GetId) {
			if (Type.find("Name", 0) == 0) {
				return Ptr[I]->Name;
			}
			else if (Type.find("Color", 0) == 0) {
				return Ptr[I]->Color;
			}
		}
	}
	return NULL;
}

/*---------------------------------------------
FUNCTION: Set()   (Overload 1)
	Set String Property by Name
----------------------------------------------*/
//template <class VarType, class VarVal>
bool Mammal::Set(std::string Obj, std::string  Var, std::string Val) {

	bool Flag = false;

	for (int I = 0; I < Size; I++) {
		if (Ptr[I]->Name.find(Obj, 0) == 0) {
			if (Var.find("Name", 0) == 0) { // Don't actually allow user to change the Name
		//		Ptr[I]->Name.assign(Val);			// It makes Free-ing the Instances hard to keep track of.
		//		Flag = true;            
			}
			else if (Var.find("Color", 0) == 0) {
				Ptr[I]->Color.assign(Val);
				Flag = true;
			}
		}	
	}
	return Flag;
}

/*---------------------------------------------
FUNCTION: Set()   (Overload 1)
	Set Property  by Id
----------------------------------------------*/
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

