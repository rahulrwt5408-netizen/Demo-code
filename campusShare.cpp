#include<iostream>
using namespace std;

class Students{
private:
    int id;
    string name;
    string email;
    string department;
    int year;
    int loginId=1001;
public:
    Students(int id , string name , string email , string department , int year ){
        this->id=id;
        this->name=name;
        this->email=email;
        this->department=department;
        this->year=year;
    }
    void getdetails(int id , string name , string email , string department , int year);
    void display();
    bool login(string);
};

void Students::getdetails(int id , string name , string email , string department , int year){
    this->id=id;
    this->name=name;
    this->email=email;
    this->department=department;
    this->year=year;
}

void Students::display(){        
    cout << "\nStudent ID   : " << id;
    cout << "\nName         : " << name;
    cout << "\nEmail        : " << email;
    cout << "\nDepartment   : " << department;
    cout << "\nYear         : " << year << endl;
}


bool Students::login(string email){
    if(this->email==email){
        printf("Login Successfull!!\n");
        int LId=loginId++;
        cout<<"Student's Login Id-> " <<LId;
        cout<<endl;
        return true;
    }
    else{
        printf("User not Registered\n");
        return false;
    }
    
}

class Items{
private:
    int id;
    string name;
    string description;
    string condition;
    double price;
public:
    Items(int id , string name , string description , string condition , double price){
        this->id=id;
        this->name=name;
        this->description=description;
        this->condition=condition;
        this->price=price;
    }
    void getdetails(int id , string name , string description , string condition , double price);
    void display();
};
void Items::getdetails(int id , string name , string description , string condition , double price){
    this->id=id;
    this->name=name;
    this->description=description;
    this->condition=condition;
    this->price=price;
}

 void Items::display(){
    cout << "\nItem ID      : " << id;
    cout << "\nName         : " << name;
    cout << "\nDescription  : " <<description;
    cout << "\nCondition    : " <<condition;
    cout << "\nPrice        : " <<price<< endl;
}


int main(){
    Students s1(0, "" , "" , "" , 0);
    Items i1(0, "" , "" , "" , 0);

    int ch;
    while(ch!=6){
        cout<<"1.Enter Students Details\n";
        cout<<"2.Enter Items Details\n";
        cout<<"3.Display Student Information\n";
        cout<<"4.Display Item Information\n";
        cout<<"5.Check Login\n";
        cout<<"6.Exit..\n";
        cout<<"6.Enter choice(1-6):: ";
        cin>>ch;

        switch(ch){
            case 1:{
                int id;
                string name;
                string email;
                string department;
                int year;

                cout<<"Enter Student Id:: ";
                cin>>id;
                cout<<"Enter Student Name:: ";
                cin.ignore();
                getline(cin,name);
                cout<<"Enter Student Email:: ";
                cin.ignore();
                getline(cin,email);
                cout<<"Enter Student Department:: ";
                cin.ignore();
                getline(cin,department);
                cout<<"Enter Student Year:: ";
                cin>>year;

                s1.getdetails(id , name , email , department , year);
                break;
            }
            case 2:{

                int id;
                string name;
                string description;
                string condition;
                double price;

                cout<<"Enter Item Id:: ";
                cin>>id;
                cout<<"Enter Item Name:: ";
                cin.ignore();
                getline(cin,name);
                cout<<"Enter Item Description:: ";
                getline(cin,description);
                cout<<"Enter Item Condition:: ";
                getline(cin,condition);
                cout<<"Enter Item Price:: ";
                cin>>price;

                i1.getdetails(id , name , description , condition , price);
                break;
            }
            case 3:
                s1.display();
                break;
            case 4:
                i1.display();
                break;
            case 5:{
                string email;
                cout<<"enter email:: ";
                cin.ignore();
                getline(cin,email);
                s1.login(email);
                break;
            }
            case 6:
                break; 
        }
    }
    return 0;
}