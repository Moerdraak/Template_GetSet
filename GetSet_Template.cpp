
#include <stdio.h>
#include <iostream>
//#include <string>

class Mammal {
public:
	int Id = -1; // Might have to cleanup the use of Id ....
	int PtrCnt = 0; // Change to Size ...
	int Weight = NULL;
	int Age = NULL;
	std::string Name;
	Mammal** Ptr = NULL;

	Mammal() {
		Name.clear();
	};

	~Mammal() {
		if (Id == -1) { 
			for (int I = 0; I < PtrCnt; I++) {
				delete Ptr[I];
			}
		}
	};

	/*---------------------------------------------
		FUNCTION: Create
	----------------------------------------------*/
	Mammal* Create(std::string NewName) {
		Mammal* New = new Mammal;
		if (Id != -1) { // ONLY FIRTS INSTANCE may create New ones
			return NULL;
		}

		// Prevent duplicate Names 
		for (int I = 0; I < PtrCnt; I++) {
			if (NewName.find(Ptr[I]->Name) == 0) {
				return NULL;
			}
		}

		New->Id = PtrCnt;
		New->Name.assign(NewName);
		if (PtrCnt == 0) {
			Ptr = new Mammal*;
			Ptr[PtrCnt] = new Mammal;
			Ptr[PtrCnt++] = New;
		}
		else {
			Mammal** Tmp = new Mammal * [PtrCnt + 1]; // Assign new memmory one bigger than before
			for (int I = 0; I < PtrCnt; I++) {
				Tmp[I] = Ptr[I];
			}
			Tmp[PtrCnt] = new Mammal;
			delete Ptr;
			Ptr = Tmp;
			Ptr[PtrCnt++] = New;
		}
		return New;
	}

	/*---------------------------------------------
	FUNCTION: Free with Name
	----------------------------------------------*/
	void Free(std::string Name) {
		bool flag = false;
		Mammal** Tmp = new Mammal * [PtrCnt - 1]; // Assign new memmory one smaller than before
		for (int I = 0; I < PtrCnt - 1; I++) {
			if (flag == false) {
				if (Ptr[I]->Name.find(Name, 0) == 0) {
					flag = true;
					PtrCnt--;
					Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
				}
				else {
					Tmp[I] = Ptr[I]; // Copy same position over from Ptr to Tmp
				}
			}
			else {
				Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
			}
		}
		delete Ptr; // Delete old Memory
		Ptr = Tmp;  // Set Ptr to new Memory
	}

	/*---------------------------------------------
	FUNCTION: Free with Id
	----------------------------------------------*/
	void Free(int Id) {
		bool flag = false;
		Mammal** Tmp = new Mammal * [PtrCnt - 1]; // Assign new memmory one smaller than before
		for (int I = 0; I < PtrCnt - 1; I++) {
			static int J = 0;
			if (flag == false) {
				if (Ptr[I]->Id == Id) {
					flag = true;
					PtrCnt--;
					Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
				}
				else {
					Tmp[I] = Ptr[I]; // Copy same position over from Ptr to Tmp
				}
			}
			else {
				Tmp[I] = Ptr[I + 1]; // Copy the next position from Ptr to Tmp
			}
		}
		delete Ptr; // Delete old Memory
		Ptr = Tmp;  // Set Ptr to new Memory
	}

	/*---------------------------------------------
	FUNCTION: Get using Obj name
	----------------------------------------------*/
//	template
	int Get(std::string Obj, std::string Var) {
		bool flag = false;
		for (int I = 0; I < PtrCnt; I++) {
			if (Ptr[I]->Name.find(Obj, 0) == 0) {
				flag = true;
				if (Var.find("Weight", 0) == 0) {
					return Ptr[I]->Weight;
				}
				else if (Var.find("Age", 0) == 0) {
					return Ptr[I]->Age;
				}
			}
		}
		return NULL; // Not good because this might be valid
	}

	/*---------------------------------------------
	FUNCTION: Get using Id
	----------------------------------------------*/
//	template
	int Get(int GetId, std::string Var) {
		bool flag = false;
		for (int I = 0; I < PtrCnt; I++) {
			if (Ptr[I]->Id == GetId) {
				flag = true;
				if (Var.find("Name", 0) == 0) {
				//	return Ptr[I]->Name;
				}
				else if (Var.find("Weight", 0) == 0) {
					return Ptr[I]->Weight;
				}
				else if (Var.find("Age", 0) == 0) {
					return Ptr[I]->Age;
				}
			}
		}
		return NULL; // Not good because this might be valid
	}


	/*---------------------------------------------
	FUNCTION: Set
	----------------------------------------------*/
	bool Set(std::string Obj, std::string Var, int Val) {

		bool Flag = false;

		for (int I = 0; I < PtrCnt; I++) {
			if (Ptr[I]->Name.find(Obj, 0) == 0) {
				if (Var.find("Weight", 0) == 0) {
					Ptr[I]->Weight = Val;
					Flag = true;
				}
				else if (Var.find("Age", 0) == 0) {
					Ptr[I]->Age = Val;
					Flag = true;
				}
			}
		}
		return Flag; // Not good because this might be valid
	}

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

	Cats.Set("Socks", "Age", 13);  // void Set(string <InstanceName>, string <Property>, Value)
	Cats.Set("Socks", "Weight", 8);  // void Set(string <InstanceName>, string <Property>, Value)

	Cats.Set("Garfield", "Age", 22);
	Cats.Set("Garfield", "Weight", 16);

	Cats.Set("Ginger", "Weight", 3); // We haven't given Ginger a weight previously
	
	std::cout << std::endl;

	for (int I = 0; I < Cats.PtrCnt; I ++ )
	std::cout << Cats.Get(I, "Name") << " is " << Cats.Get(I, "Age") << " years old and weigh " << Cats.Get(I, "Weight") << " kg." << std::endl;


	Cats.Free("Socks"); // Removing an Object from the list and freeing the memory





	return 0;
}



