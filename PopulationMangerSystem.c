#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned char Day;
	unsigned char Month;
	unsigned int Year;
}Date;

typedef struct {
	long long Id;
	char* Name;
	char* Family;
	Date DateOfBirth;
	long long PartenerId;
	long long MotherId;
	long long FatherId;
	char NumOfchildren;
	long long* ChildrenPtr;
}Person;

typedef struct {
	int Db_size;
	int Num_Of_Persons;
	Person* PersonPtr;
}Db_Mgr;

void Print_Person(const Person* p);
void Init_Db(Db_Mgr* Db_Ptr);
Person* Search_Id(const Db_Mgr* Db_Ptr, long long UserId);
void Search_Person(const Db_Mgr* Db_Ptr);
void Add_Person(Db_Mgr* Db_Ptr);
void Search_Parents(const Db_Mgr* Db_Ptr);
void Print_Db(const Db_Mgr* Db_Ptr);
void Search_By_Name(const Db_Mgr* Db_Ptr);
void Sort_Db_By_Id(Db_Mgr* Db_Ptr);
void Delete_Person(Db_Mgr* Db_Ptr);
char Menu();
long long Get_Id();
void Get_Gen(const Db_Mgr* Db_Ptr);
void Quit(Db_Mgr* Db_Ptr);
void Free_Person(Person *Person_P);
void Delete_Children(Db_Mgr* Db_Ptr, long long Child_Id);
void Get_Person_details(Db_Mgr * Db_Ptr, int spot);
int Check_for_Room(Db_Mgr* Db_Ptr);
void Allocate_Db_After_Delete(Db_Mgr* Db_Ptr, Person* ptr);
void Delete_Parent_Id(Db_Mgr* Db_Ptr, Person* ptr);

void main()
{
	Db_Mgr Data_Base;
	printf("Please enter the number of citizens in the country: ");
	scanf("%d", &Data_Base.Db_size);
	Init_Db(&Data_Base);
	Data_Base.Num_Of_Persons = 0;
	char Check;
	do {
		Check = Menu();
		switch (Check)
		{
		case 1:
			Add_Person(&Data_Base);
			break;
		case 2:
			Search_Person(&Data_Base);
			break;
		case 3:
			Search_Parents(&Data_Base);
			break;
		case 4:
			Delete_Person(&Data_Base);
			break;
		case 5:
			Get_Gen(&Data_Base);
			break;
		case 6:
			Print_Db(&Data_Base);
			break;
		case 7:
			Search_By_Name(&Data_Base);
			break;
		case 8:
			Quit(&Data_Base);
			break;
		default: printf("Wrong input. Please enter another: ");
			break;
		}
	} while (Check != 8);
	return;
}

/*print menu to user, requst menue number from user
  return: Menu choice*/
char Menu()
{
	int index = 0;
	printf("\nDatabase System Menu:\n");
	printf("1. Add person\n");
	printf("2. Search a person\n");
	printf("3. Search Parents\n");
	printf("4. Delete a person\n");
	printf("5. Get generation\n");
	printf("6. Print database\n");
	printf("7. Search by name\n");
	printf("8. Quit\n");
	printf("Please enter requested input: ");
	scanf("%d", &index);
	while (index < 1 || index > 8)//if user index is not between 1-8, try again
	{
		printf("Wrong inpute, try again: ");
		scanf("%d", &index);
	}
	return index;
}

/*print person from data base
input: pointer to person from data base*/
void Print_Person(const Person* p)
{
	printf("\n");
	if (p->Id == 0)// if the id is 0 return
	{
		return;
	}
	printf("ID: %lld\nFirst Name: %s\nFamily Name: %s\nDate Of Birth: %d/%d/%d\n",
		p->Id, p->Name, p->Family, p->DateOfBirth.Day, p->DateOfBirth.Month, p->DateOfBirth.Year);
	// **if id is 0 dont print**
	if (!(p->PartenerId == 0)){
		printf("Partner ID: %lld\n", p->PartenerId);}
	if (!(p->MotherId == 0)){
			printf("Mother ID: %lld\n", p->MotherId);}
	if (!(p->FatherId == 0)){
			printf("Father ID: %lld\n", p->FatherId);}
	printf("Number Of Children: %d\n", p->NumOfchildren);
	if (p->NumOfchildren > 0)
	{
		for (int i = 0; i < p->NumOfchildren; i++)// print children id from children array
			printf("Child Num %d ID: %lld\n", i + 1, p->ChildrenPtr[i]);
	}
	printf("\n");
}

/*get id from user, check if id valid
return: the id the user entered*/
long long Get_Id()
{
	long long Id = 0;
	scanf("%lld", &Id);
	while (Id < 1)// if the id < 1  try again
	{
		printf("You entered an invalid number please try again: ");
		scanf("%lld", &Id);
	}
	return Id;
}

/*allocate memory for PersonPtr array according to data base size
input: pointer to data base*/
void Init_Db(Db_Mgr* Db_Ptr)
{
	while (Db_Ptr->Db_size < 0)// if the number <0 try again
	{
		printf("You entered an invalid number please try again: ");
		scanf("%d", &Db_Ptr->Db_size);
	}
	Db_Ptr->PersonPtr = (Person*)calloc(Db_Ptr->Db_size , sizeof(Person));// allocate memory
	if (!(Db_Ptr->PersonPtr))
	{
		puts("Allocation failed");
		return;
	}
	printf("\n");
	return;
}

/* ---check if there is room in the data base---
input: pointer to data base
return: person location index
logic:
1. check if data base is empty (in case user enters 0 people in db/ last person was deleted)
2. check if there is room in the data base
3. if there is room return current location
4. if there is no room
- copy persons to tmp array
- allocate person ptr array with current size + 1
- copy persons from tmp array to new allocated persons ptr array */
int Check_for_Room(Db_Mgr* Db_Ptr)
{
	int spot = 0;
	if (Db_Ptr->Num_Of_Persons == 0 && Db_Ptr->Db_size == 0)// if data base is empty
	{
		Db_Ptr->PersonPtr = (Person*)calloc(1, sizeof(Person));// allocate memory
		if (!(Db_Ptr->PersonPtr))
		{
			puts("Allocation failed");
			return;
		}
		Db_Ptr->Db_size = Db_Ptr->Db_size + 1;
		return spot = 0;
	}
	/* ---check if there is room in the data base--*/
	for (int idx = 0; idx < Db_Ptr->Db_size; idx++)
	{
		if (!Db_Ptr->PersonPtr[idx].Id)// if id = 0 >>there is room>> continue
		{
			spot = idx;
			return spot;
		}
		if (idx == Db_Ptr->Db_size - 1)// we are in the last person in array>>no more room
		{
			Db_Mgr tmp_Data_Base;
			tmp_Data_Base.PersonPtr = (Person*)calloc(Db_Ptr->Db_size, sizeof(Person));// allocate tmp data base person ptr array
			if (!(tmp_Data_Base.PersonPtr))
			{
				puts("Allocation failed");
				return;
			}
			tmp_Data_Base.Db_size = Db_Ptr->Db_size;
			for (int k = 0; k < Db_Ptr->Db_size; k++)// copying persons to tmp array
			{
				tmp_Data_Base.PersonPtr[k] = Db_Ptr->PersonPtr[k];
			}
			Db_Ptr->Db_size = Db_Ptr->Db_size + 1;
			Db_Ptr->PersonPtr = (Person*)calloc(Db_Ptr->Db_size, sizeof(Person));//allocate person ptr array with new size
			if (!(Db_Ptr->PersonPtr))
			{
				puts("Allocation failed");
				return;
			}
			for (int r = 0; r < Db_Ptr->Db_size - 1; r++)// coping person from tmp db to new allocated db
			{
				Db_Ptr->PersonPtr[r] = tmp_Data_Base.PersonPtr[r];
			}
			free(tmp_Data_Base.PersonPtr);// free tmp person ptr memory
			return spot = Db_Ptr->Db_size - 1;	
		}
	}
}

/* ---add new person to data base---
input: pointer to data base
logic:
1. check if there is room in the data base
2. get person details from user
3. increase data base num of people size
4. sort data base person array by id number*/
void Add_Person(Db_Mgr* Db_Ptr)
{
	int spot = Check_for_Room(Db_Ptr);
	Get_Person_details(Db_Ptr, spot);
	Db_Ptr->Num_Of_Persons++; // increase data base num of person in 1
	Sort_Db_By_Id(Db_Ptr); // sort persons in data base by id number
}

/* geting person details
 input: pointer to data base, location of the person to write to*/
void Get_Person_details(Db_Mgr * Db_Ptr, int spot)
{
	int i = spot;
	printf("\n*Please enter the folowing details*\n");
	printf("ID Number: ");
	Db_Ptr->PersonPtr[i].Id = Get_Id();
	printf("First Name: ");
	char tmpname[20];
	scanf(" %s", tmpname);
	int NameSize = strlen(tmpname);
	Db_Ptr->PersonPtr[i].Name = (char*)malloc((NameSize + 1) * sizeof(char)); // allocate person name
	if (!(Db_Ptr->PersonPtr[i].Name))
	{
		puts("Allocation failed");
		return;
	}
	strcpy(Db_Ptr->PersonPtr[i].Name, tmpname);
	printf("Last Name: ");
	char tmpfamily[20];
	scanf(" %s", tmpfamily);
	int FamilySize = strlen(tmpfamily);
	Db_Ptr->PersonPtr[i].Family = (char*)malloc((FamilySize + 1) * sizeof(char)); // allocate person family name
	if (!(Db_Ptr->PersonPtr[i].Family))
	{
		puts("Allocation failed");
		return;
	}
	strcpy(Db_Ptr->PersonPtr[i].Family, tmpfamily);
	printf("Date Of Birth: \n");
	printf("Please enter day: ");
	scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Day);
	// check if day is between 1-30
	while ((Db_Ptr->PersonPtr[i].DateOfBirth.Day < 1) || (Db_Ptr->PersonPtr[i].DateOfBirth.Day > 30)) 
	{
		printf("Invalid day. Please enter another: ");
		scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Day);
	}
	printf("Please enter month: ");
	scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Month);
	// check if month is between 1-12
	while ((Db_Ptr->PersonPtr[i].DateOfBirth.Month < 1) || (Db_Ptr->PersonPtr[i].DateOfBirth.Month > 12))
	{
		printf("Invalid month. Please enter another: ");
		scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Month);
	}
	printf("Please enter year: ");
	scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Year);
	// check if year is between 1000-9999
	while (Db_Ptr->PersonPtr[i].DateOfBirth.Year < 1000 || Db_Ptr->PersonPtr[i].DateOfBirth.Year > 9999) 
	{
		printf("Invalid year. Please enter another: ");
		scanf("%d", &Db_Ptr->PersonPtr[i].DateOfBirth.Year);
	}
	printf("Partner ID: ");
	scanf("%lld", &Db_Ptr->PersonPtr[i].PartenerId);
	while (Db_Ptr->PersonPtr[i].PartenerId < 0) // check if id >0
	{
		printf("Invalid ID. Please enter another: ");
		scanf("%lld", &Db_Ptr->PersonPtr[i].PartenerId);
	}
	printf("Mother ID: ");
	scanf("%lld", &Db_Ptr->PersonPtr[i].MotherId);
	while (Db_Ptr->PersonPtr[i].MotherId < 0) // check if id >0
	{
		printf("Invalid ID. Please enter another: ");
		scanf("%lld", &Db_Ptr->PersonPtr[i].MotherId);
	}
	printf("Father ID: ");
	scanf("%lld", &Db_Ptr->PersonPtr[i].FatherId);
	while (Db_Ptr->PersonPtr[i].FatherId < 0) // check if id >0
	{
		printf("Invalid ID. Please enter another: ");
		scanf("%lld", &Db_Ptr->PersonPtr[i].FatherId);
	}
	printf("Number of Children: ");
	scanf("%d", &Db_Ptr->PersonPtr[i].NumOfchildren);
	while (Db_Ptr->PersonPtr[i].NumOfchildren < 0) // check if number of children >0
	{
		printf("Invalid number. Please enter another: ");
		scanf("%d", &Db_Ptr->PersonPtr[i].NumOfchildren);
	}
	if (Db_Ptr->PersonPtr[i].NumOfchildren > 0)
	{
		// allocate children ptr array
		Db_Ptr->PersonPtr[i].ChildrenPtr = (long long int *)malloc(Db_Ptr->PersonPtr[i].NumOfchildren * sizeof(long long int));
		if (!(Db_Ptr->PersonPtr[i].ChildrenPtr))
		{
			puts("Allocation failed");
			return;
		}
		for (int j = 0; j < Db_Ptr->PersonPtr[i].NumOfchildren; j++)// get child id from usere
		{
			printf("Enter Child Num %d ID: ", j + 1);
			Db_Ptr->PersonPtr[i].ChildrenPtr[j] = Get_Id();
		}
	}
	return;
}

/* search id in data base, if found return person pointer, if not return NULL
input: pointer to data base, user id
return: pointer to person if found*/
Person* Search_Id(const Db_Mgr* Db_Ptr, long long UserId)
{
	for (int i = 0; i < Db_Ptr->Db_size; i++) // search in data base for id
	{
		if (Db_Ptr->PersonPtr[i].Id == UserId)
		{
			return (&Db_Ptr->PersonPtr[i]); // id is fount return person pointer
		}
	}
	return NULL; // id is not found return NULL
}

/* search person in data base by id number
input: pointer to data base
 1.if found print a message and person details
 2.if not found print a message*/
void Search_Person(const Db_Mgr* Db_Ptr)
{
	printf("Please enter the ID number you want to search: ");
	long long userId = Get_Id();
	Person* ptr = Search_Id(Db_Ptr, userId);// search person by id
	if (ptr) // if ptr is not NULL person found
	{
		printf("The system found a person with the ID you enterd: \n");
		Print_Person(ptr); // print person details
		printf("\n");
	}
	else // if  person not found print message
		printf("There is no matching ID in the Data Base\n");
}

/* --search person parents in the data base--
input: pointer to data base
1. if father found print father details, if not print message
2. if mother found print mother details, if not print message*/
void Search_Parents(const Db_Mgr* Db_Ptr)
{
	printf("\nPlease enter the Id number of the person you want to search for parents: ");
	long long userId = Get_Id();
	Person* ptr = Search_Id(Db_Ptr, userId);
	if (ptr) // if ptr is not NULL, person found
	{
		printf("The system found a person with the ID you enterd\n");
		long long MotherId = ptr->MotherId;
		if (MotherId == 0)  // if mother id is 0 ,mother is not found
		{
			printf("There is no matching mother ID in the Data Base\n");
		}
		else 
		{
			Person* ptrMother = Search_Id(Db_Ptr, MotherId);
			if (ptrMother) // if ptrMother is not NULL ,mother is found
			{
				printf("This is the person's mother: \n");
				Print_Person(ptrMother); // print mother details
			}
			else
			{
				printf("There is no matching Mother ID in the Data Base\n");
			}
		}
		long long FatherId = ptr->FatherId;
		if (FatherId == 0) // if father id is 0, father is not found
		{
			printf("There is no matching father ID in the Data Base\n");
		}
		else
		{
			Person* ptrFather = Search_Id(Db_Ptr, FatherId);
			if (ptrFather)  // if FatherId id is not NULL , father is found
			{
				printf("This is the person's father: \n");
				Print_Person(ptrFather);  // print father details
			}
			else
			{
				printf("There is no matching father ID in the Data Base\n");
			}
		}
		printf("\n");
	}
	else // if ptr is NULL, person not found
		printf("There is no matching ID in the Data Base\n");
}

/* print all persons in the data base
input: pointer to data base*/
void Print_Db(const Db_Mgr* Db_Ptr)
{
	if (Db_Ptr->Num_Of_Persons)
	{
		printf("\n****Data Base People****\n");
		for (int i = 0; i < Db_Ptr->Db_size; i++)
		{
			Person* ptr = &Db_Ptr->PersonPtr[i];
			if (!(Db_Ptr->PersonPtr[i].Id == 0)) // if person id is not 0, print person
			{
				printf("\n");
				Print_Person(ptr);
			}
		}
	}
	else // if data base is empty
	{
		printf("\nData base is empty, there are no people to print\n");
	}
}

/*search all the persons in the data base with first and last name
input: pointer to data base*/
void Search_By_Name(const Db_Mgr* Db_Ptr)
{
	printf("Please enter the person full name: \n"); // get name from user
	printf("First Name: ");
	char username[20];
	scanf("%s", username);
	printf("Last Name: ");
	char userFamily[20];
	scanf("%s", userFamily);
	printf("\n");
	int counter = 0;
	for (int i = 0; i < Db_Ptr->Db_size; i++)
	{
		if (Db_Ptr->PersonPtr[i].Name)// if person name is not 0
		{
			int NameTest = strcmp(Db_Ptr->PersonPtr[i].Name, username); // check if person first name is the same as user's name
			int FamilyTest = strcmp(Db_Ptr->PersonPtr[i].Family, userFamily); // check if person last name is the same as user's name
			if (NameTest == 0 && FamilyTest == 0) // if person last and first name are the same as user's >> person found
			{ 
				printf("The system found a Person with that name: \n");
				Person* ptr = &Db_Ptr->PersonPtr[i];
				Print_Person(ptr);
				counter++; // raise counter if system found a person
			}
		}
	}
	if (counter == 0) // if counter is 0 , there is no person with that name in the data base
	{
		printf("There is no person with matching details in the Data Base\n");
	}
	printf("\n");
}

/* sort persons in the data base by id number
input: pointer to data base*/
void Sort_Db_By_Id(Db_Mgr* Db_Ptr)
{
	Person tmpPerson;
	for (int i = 0; i < Db_Ptr->Db_size - 1; i++)
	{
		for (int j = 0; j < Db_Ptr->Db_size - 1; j++)
			if (Db_Ptr->PersonPtr[j].Id > Db_Ptr->PersonPtr[j + 1].Id)
			{
				tmpPerson = Db_Ptr->PersonPtr[j + 1];
				Db_Ptr->PersonPtr[j + 1] = Db_Ptr->PersonPtr[j];
				Db_Ptr->PersonPtr[j] = tmpPerson;
			}
	}
}

/*delete parent id from deleted person childrens
input: pointer to data base, pointer to the deleted person*/
void Delete_Parent_Id(Db_Mgr* Db_Ptr, Person* ptr)
{
	for (int i = 0; i < ptr->NumOfchildren; i++)
	{
		Person* ChildPtr = Search_Id(Db_Ptr, ptr->ChildrenPtr[i]);
		if (ChildPtr) // if child was found in data base
		{
			if (ChildPtr->MotherId == ptr->Id)// if the ptr is the mother delete mother id
			{
				ChildPtr->MotherId = 0;
			}
			if (ChildPtr->FatherId == ptr->Id)// if the ptr is the Father delete mother id
			{
				ChildPtr->FatherId = 0;
			}
		}
	}
}

/*delete child from children ptr array
input: pointer to data base, child id to delete*/
void Delete_Children(Db_Mgr* Db_Ptr ,long long Child_Id)
{
	Person* ParentPtr;
	ParentPtr = Db_Ptr;
	Person tmpParentChildPtr;
	for (int i = 0; i < ParentPtr->NumOfchildren; i++)
	{
		if (ParentPtr->ChildrenPtr[i] == Child_Id)
		{
			ParentPtr->ChildrenPtr[i] = 0;
			// allocate tmp parent children ptr
			tmpParentChildPtr.ChildrenPtr = (long long int*)malloc((ParentPtr->NumOfchildren - 1) * sizeof(long long int));
			if (!(tmpParentChildPtr.ChildrenPtr))
			{
				puts("Allocation failed");
				return;
			}
			int tmp2 = -1;
			for (int k = 0; k < ParentPtr->NumOfchildren - 1; k++) // copying childptr to tmp array
			{
				tmp2++;
				if (!ParentPtr->ChildrenPtr[k])
				{
					tmp2++;
					tmpParentChildPtr.ChildrenPtr[k] = ParentPtr->ChildrenPtr[k + 1];
				}
				if (ParentPtr->ChildrenPtr[k]) // copying children id to array apart from the one we deleted 
				{
					tmpParentChildPtr.ChildrenPtr[k] = ParentPtr->ChildrenPtr[tmp2];
				}
			}
			ParentPtr->NumOfchildren = ParentPtr->NumOfchildren - 1; // reducing num of children in 1
			// allocate children ptr with new reduced size
			ParentPtr->ChildrenPtr = (long long int*)malloc(ParentPtr->NumOfchildren * sizeof(long long int));
			if (!(ParentPtr->ChildrenPtr))
			{
				puts("Allocation failed");
				return;
			}
			for (int r = 0; r < ParentPtr->NumOfchildren; r++)// coping childptr from tmp db to new allocated db
			{
				ParentPtr->ChildrenPtr[r] = tmpParentChildPtr.ChildrenPtr[r];
			}
			break;
		}
	}
	free(tmpParentChildPtr.ChildrenPtr);
}

/*allcate data base persons array after deleting person
input: pointer to data base, pointer to the deleted person 
logic:
- copy persons to tmp array accepts the person to delete
- allocate person ptr array with size - 1
- copy persons from tmp array to new allocated persons ptr array
- free memory of deleted person*/
void Allocate_Db_After_Delete(Db_Mgr* Db_Ptr, Person* ptr)
{
	Db_Mgr tmp_Data_Base;
	tmp_Data_Base.PersonPtr = (Person*)calloc(Db_Ptr->Db_size, sizeof(Person)); // allocate temp data base
	if (!(tmp_Data_Base.PersonPtr))
	{
		puts("Allocation failed");
		return;
	}
	tmp_Data_Base.Db_size = Db_Ptr->Db_size - 1;
	int tmp4 = -1;
	for (int k = 0; k < tmp_Data_Base.Db_size; k++) // copying persons to tmp array
	{
		tmp4++;
		if (Db_Ptr->PersonPtr[k].Id == ptr->Id)
		{
			tmp4++;
			tmp_Data_Base.PersonPtr[k] = Db_Ptr->PersonPtr[k + 1];
		}
		if (!(Db_Ptr->PersonPtr[k].Id == ptr->Id))
			tmp_Data_Base.PersonPtr[k] = Db_Ptr->PersonPtr[tmp4];
	}
	Db_Ptr->Db_size = Db_Ptr->Db_size - 1;
	Db_Ptr->PersonPtr = (Person*)calloc(Db_Ptr->Db_size, sizeof(Person)); // allocate data base with reduced size
	if (!(Db_Ptr->PersonPtr))
	{
		puts("Allocation failed");
		return;
	}
	for (int r = 0; r < Db_Ptr->Db_size; r++)// coping person from tmp db to new allocated db
	{
		Db_Ptr->PersonPtr[r] = tmp_Data_Base.PersonPtr[r];
	}
	free(tmp_Data_Base.PersonPtr);
	Db_Ptr->Num_Of_Persons--; // reduce num of person size
	Free_Person(ptr); // free deleted person memory
}

/*--delete person from data base--
input: pointer to data base
logic:
1.search person in data base
2.if person found:
 - if there is partner id >> replace to 0
 - if there are parents >> delete the person from children ptr array, and reallocate it
 - if there are children >> replace father/mother if of children to 0
3.if person not found, show message*/
void Delete_Person(Db_Mgr* Db_Ptr)
{
	if (Db_Ptr->Num_Of_Persons) // if data base is not empty
	{
		printf("Please enter the ID number of the person you want to delete: ");
		long long userId = Get_Id();
		Person* ptr = Search_Id(Db_Ptr, userId);
		if (ptr) // if ptr is not NULL person found
		{
			printf("The system found a person with the ID you enterd\n");
			printf("\n");
			if (ptr->PartenerId) //if partner id is not NULL 
			{
				Person* PartenrtPtr;
				PartenrtPtr = Search_Id(Db_Ptr, ptr->PartenerId);
				if (PartenrtPtr) // if partner was found in data base
				{
					PartenrtPtr->PartenerId = 0; // replace partner id to 0
				}
			}
			if (ptr->NumOfchildren) // if num of children is not NULL >> children parent is 0 
			{
				Delete_Parent_Id(Db_Ptr, ptr);
			}
			if (ptr->FatherId) //if father id is not NULL
			{
				Person* FatherPtr;
				FatherPtr = Search_Id(Db_Ptr, ptr->FatherId);
				if (FatherPtr) // if FatherPtr found >> delete the child from children ptr array 
				{
					Delete_Children(FatherPtr, ptr->Id);
				}
			}
			if (ptr->MotherId) //if mother id is not NULL
			{
				Person* MotherPtr;
				MotherPtr = Search_Id(Db_Ptr, ptr->MotherId);
				if (MotherPtr)  // if MotherPtr found >> delete the child from children ptr array 
				{
					Delete_Children(MotherPtr, ptr->Id);
				}
			}
			Allocate_Db_After_Delete(Db_Ptr, ptr);// aloctaing data base after delete
			Sort_Db_By_Id(Db_Ptr); // sort persons in data base by id number
			printf("Delete completed successfully\n");
		}
		else
			printf("There is no matching ID in the Data Base\n");
	}
	else // if data base is empty
	{
		printf("\nData base is empty, there are no people to delete\n");
	}
}

/*get a person number of generations
input: pointer to data base
logic:
1. check if the person has children
2. if perosn doesnt have children gen is 1 >> return 
3. if person has children copy person to next level array
4. copy the person from next level array to current path
5. determine what is the next step in each level with - next level array
6. copy children of last person in current path array to next level array
7. move person from current path to seen cells array if the all person's children are in seen cells array, 
or if he dont have children
8. repeat the process until first cell of next level array is 0
9. the longest current path is the person number of generations
10. print number of generations*/
void Get_Gen(const Db_Mgr* Db_Ptr)
{
	printf("\nPlease enter the ID number of the person you want to search for generation number: ");
	long long UserId = Get_Id();
	Person* ParentPtr = Search_Id(Db_Ptr, UserId), *TempPerson;
	int MaxPath = 0;
	if (ParentPtr) // if person was found
	{
		printf("The system found a person with the ID you enterd\n");
		if (ParentPtr->NumOfchildren) //if there are children
		{
			// allocate arrays 
			long long *Next_Level, *Current_Path, *Seen_Cells;
			int Next_Level_Size = 1;
			Next_Level = (long long*)calloc(Next_Level_Size, sizeof(long long));
			if (!Next_Level)
			{
				puts("Allocation failed");
				return;
			}
			int Current_Path_Size = 1;
			Current_Path = (long long*)calloc(Current_Path_Size, sizeof(long long));
			if (!Current_Path)
			{
				puts("Allocation failed");
				return;
			}
			Next_Level[0] = ParentPtr->Id; 
			int Seen_Cells_Size = 0;
			Seen_Cells = (long long*)calloc(Seen_Cells_Size, sizeof(long long));
			if (!Seen_Cells)
			{
				puts("Allocation failed");
				return;
			}
			while (Next_Level[0]) // run until first person id in Next level array is 0
			{
				Current_Path = (long long*)realloc(Current_Path, Current_Path_Size * sizeof(long long));
				if (!Current_Path)
				{
					puts("Allocation failed");
					return;
				}
				Current_Path[Current_Path_Size - 1] = Next_Level[Next_Level_Size - 1]; // copy person from next level to current path
				if (Current_Path_Size > MaxPath)
				{
					MaxPath = Current_Path_Size; // save max current path = person num of generations
				}
				TempPerson = Search_Id(Db_Ptr, Current_Path[Current_Path_Size - 1]);
				if (TempPerson->NumOfchildren)
				{
					Next_Level_Size = Next_Level_Size + (TempPerson->NumOfchildren - 1); // increse next level according to number of children
					Next_Level = (long long*)realloc(Next_Level, Next_Level_Size * sizeof(long long));
					if (!Next_Level)
					{
						puts("Allocation failed");
						return;
					}
					for (int k = TempPerson->NumOfchildren; k > 0; k--) // copy children to next level
					{
						Next_Level[(Next_Level_Size - k)] = TempPerson->ChildrenPtr[k - 1];
					}
				}
				else //if the person dont have children move preson from current path to seen cells list
				{
					Seen_Cells_Size++;
					Seen_Cells = (long long*)realloc(Seen_Cells, Seen_Cells_Size * sizeof(long long));
					if (!Seen_Cells)
					{
						puts("Allocation failed");
						return;
					}
					Seen_Cells[Seen_Cells_Size - 1] = Current_Path[Current_Path_Size - 1]; // move person to seen list
					Next_Level[Next_Level_Size - 1] = NULL;
					Current_Path[Current_Path_Size - 1] = NULL;
					if (!(Next_Level_Size == 1))
					{
						Next_Level_Size--;
					}
					// allocate arrays after changing size
					Next_Level = (long long*)realloc(Next_Level, Next_Level_Size * sizeof(long long));
					if (!Next_Level)
					{
						puts("Allocation failed");
						return;
					}
					Current_Path_Size--;
					Current_Path = (long long*)realloc(Current_Path, Current_Path_Size * sizeof(long long));
					if (!Current_Path)
					{
						puts("Allocation failed");
						return;
					}
				}
				// check if all person children are in the seen list
				int idx = 0;
				while (!idx)
				{
					if (Current_Path[Current_Path_Size - 1])
					{
						TempPerson = Search_Id(Db_Ptr, Current_Path[Current_Path_Size - 1]);
						idx = TempPerson->NumOfchildren;
						int found = 1;
						while (idx > 0 && found == 1)
						{ //check each child if is in seen cells list
							found = 0;
							for (int l = 0; l < Seen_Cells_Size; l++)
							{
								if (Seen_Cells[l] == TempPerson->ChildrenPtr[idx - 1]) // found the child
								{
									found = 1; // mark as found
								}
							}
							if (found)
							{
								idx--;
							}
						}
						// if all children were found move to seen list from current path
						if (!idx)
						{
							Seen_Cells_Size++;
							Seen_Cells = (long long*)realloc(Seen_Cells, Seen_Cells_Size * sizeof(long long));
							if (!Seen_Cells)
							{
								puts("Allocation failed");
								return;
							}
							Seen_Cells[Seen_Cells_Size - 1] = Current_Path[Current_Path_Size - 1]; // move person to seen list
							Current_Path[Current_Path_Size - 1] = NULL;
							if (!(Current_Path_Size == 1))
							{
								Current_Path_Size--;
							}
							Current_Path = (long long*)realloc(Current_Path, Current_Path_Size * sizeof(long long));
							if (!Current_Path)
							{
								puts("Allocation failed");
								return;
							}
						}
					}
					else // if not all children found in seen cells list
					{
						break;
					}
				}
				if (!Next_Level[0])
				{
					break;
				}
				Current_Path_Size++;
			}
			printf("The generation number is: %d\n", MaxPath );
			free(Next_Level);
			free(Current_Path);
			free(Seen_Cells);
			return;
		}
		else //  in case there are no children
		{
			printf("The generation number is: 1\n");
			return;
		}
	}
	else // in case there is no matching id in the data base
	{
		printf("The system did not find a person with the ID you entered\n");
		return;
	}
}

/*free memory and close program
input: pointer to data base*/
void Quit(Db_Mgr* Db_Ptr)
{
	printf("Closing program and cleaning memory\n");
	for (int i = 0; i < Db_Ptr->Db_size; i++)// free memory for all persons in the data base
	{
		Free_Person(&Db_Ptr->PersonPtr[i]); // free person related memory
	}
	free(Db_Ptr->PersonPtr); // free data base person ptr array memory
}

/*free person realted memory
input: pointer to Person*/
void Free_Person(Person* Person_p)
{
	free(Person_p->Name); // free person name memory
	free(Person_p->Family); // free person family name memory
	if (Person_p->NumOfchildren) // if num of children is not NULL free children ptr array
	{
		free(Person_p->ChildrenPtr); // free children ptr array
	}
}