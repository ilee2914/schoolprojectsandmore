#include<iostream>

using namespace std;

struct st

{

int roll;

char name[10];

char grade;

struct st *next;

};

struct st *hptr=0; // declare null header of linked list

int addStudent();

int delete_student();

void display();

int update();

int search();

int main()

{

char ch;

int opt;

do

{

cout<<"Choose option:\n\t1. Add Student\n"

"\t2. Remove Student\n"

"\t3. Update grade\n"

"\t4. Search Student\n"

"\t5. Display Records\n";

cin>>opt;

switch(opt)

{

case 1: addStudent();

break;

case 2: delete_student();

break;

case 3: update();

break;

case 4: search();

break;

case 5: display();

break;

default:

cout<<"Invaild Option, Try again...!\n";

break;

}

cout<<"Do you wants another operation(y/n)? : ";

cin>>ch;

}while(ch=='Y'||ch=='y');

}

/* Add Student */

int addStudent()

{

st *temp = new st;

st *temp1 = new st;

cout<<"Enter roll no:";

cin>>temp->roll;

if(cin.fail()){

cout<<"Not numeric\n";

return 0;

}

cout<<"Enter name:";

cin>>temp->name;

cout<<"Enter grade:";

cin>>temp->grade;

// Head of linked list is null

if(hptr==0)

{

temp->next=hptr;

hptr=temp;

}

else

{

temp1=hptr;

while(temp1->next)

temp1=temp1->next;

temp->next=temp1->next;//NULL

temp1->next=temp;

}

}

/* Delete Student */

int delete_student()

{

int rollno;

st *temp = new st;

st *temp1 = new st;

cout<<"Do U wants to delete record ,then enter Roll No.:";

cin>>rollno;

//Rollno is not numeric

if(cin.fail()){

cout<<"Not numeric\n";

return 0;

}

temp=hptr;

while(temp)

{

if(temp->roll==rollno)

{

// searching record is head

if(temp==hptr)

{

hptr=temp->next;

free(temp);

return 0;

}

else

{

temp1->next=temp->next;

free(temp);

return 0;

}

}

temp1=temp;

temp=temp->next;

}

cout<<"Record is not found..!\n";

}

/* Update the grade of the student */

int update()

{

int rollno;

st *temp = new st;

st *temp1 = new st;

cout<<"Enter roll number.:";

cin>>rollno;

if(cin.fail()){

cout<<"Not numeric\n";

return 0;

}

temp=hptr;

while(temp)

{

if(temp->roll==rollno)

{

cout<<"Enter grade to update:";

cin>>temp->grade;

return 0;

}

temp1=temp;

temp=temp->next;

}

cout<<"Record not found..!\n";

}

/* search the student */

int search()

{

int rollno;

st *temp = new st;

st *temp1 = new st;

cout<<"Enter roll number:";

cin>>rollno;

if(cin.fail()){

cout<<"Not numeric\n";

return 0;

}

temp=hptr;

cout<<"RollNo.\t\t|Name\t\t|Grade\n";

cout<<"-------------------------------------------\n";

while(temp)

{

if(temp->roll==rollno)

{

cout<<temp->roll<<"\t\t|"<<temp->name<<"\t\t|"<<temp->grade<<"\n";

return 0;

}

temp1=temp;

temp=temp->next;

}

cout<<"Record is not present..!\n";

}

/* Display the record */

void display()

{

cout<<"RollNo.\t\t|Name\t\t|Grade\n";

cout<<"-------------------------------------------\n";

struct st *ptr;

ptr=hptr;

while(ptr)

{

cout<<ptr->roll<<"\t\t|"<<ptr->name<<"\t\t|"<<ptr->grade<<"\n";

ptr=ptr->next;

}

}
