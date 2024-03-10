#include <iostream>
#include <iomanip>
#include <time.h>
#include <iomanip>
using namespace std;

#define MenuSpace() cout << setw(27) << setfill(' ') << " ";
#define SpaceOption() cout << setw(16) << setfill(' ') << " ";
#define DisplaySpace() cout << setw(5) << setfill(' ') << " ";
#define MenuSplitter() cout << setw(16) << setfill(' ') << " " << setw(90) << setfill('-') << " \n\n";


struct DataHolder {
	const static int MAX = 2;
	double val[MAX], result;
	string mathOperator;
    
	void Display() {
	    if (this->mathOperator.length() != 1)  cout << this->mathOperator << "\n";
	    else  cout << this->val[0] << " " << this->mathOperator << " " << this->val[1] << " = " << this->result << "\n";
	}
};

struct History {
	History* next = NULL;
    History* prev = NULL;
	DataHolder* data = new DataHolder;
};

int MainMenu();
int MathMenu();
int HistoryMenu(int, int, int);

void AddHistory(History*&, History*&, History*&);
void DisplayHistory(History*, int, int);
void UpdateAHistory(History*&, History*&);
History* DuplicateAHistory(History*, int);
History* AddUpdatedHistory(History*&, History*, History*&);
void ClearHistory(History*&, int&);
void DeleteAHistory(History*&);
void InputValue(History*&);
void Computation(int, History*&);
int HistoryFunctionalities(int&, History*&, History*&);
void JumpToPage(int&, int);


double Add(double[], const int);
double Subtract(double[], const int);
double Division(double[], const int);
double Multiplication(double[], const int);

int main() {
    int choice, historyCounter = 0;
    History* temp = NULL; 
	History* head = NULL,* tail = NULL; 

	while (true) {
		switch (MainMenu()) {
		case 1:
			do {
				choice = MathMenu();

				if (!choice){system("cls"); break;}

                InputValue(temp);

                Computation(choice, temp);

                AddHistory(head, tail, temp);

                historyCounter++;
            }
			while (true);
			break;
		case 2:
            HistoryFunctionalities(historyCounter, head, tail);
			break;
		case 0:
			exit(0);
		default:
			break;
		}
	}
}

int MainMenu() {
	int choice;

	do {
		 SpaceOption(); cout << "1. Calculator\n";
		 SpaceOption(); cout << "2. History\n\n";
         SpaceOption(); cout << "0. Exit\n\n";
		 SpaceOption(); cout << ":: ";
		 cin >> choice;
	} while (choice < 0 || choice > 2);

	system("cls");

	return choice;
}

int MathMenu() {
	int choice;

	do {
        SpaceOption(); cout << "1. Add\n";
        SpaceOption(); cout << "2. Subtract\n";
        SpaceOption(); cout << "3. Multiply\n";
        SpaceOption(); cout << "4. Divide\n";
        SpaceOption(); cout << "0. Return\n\n";
        SpaceOption(); cout << ":: ";
        cin >> choice;
	} while (choice < 0 || choice > 4);

	system("cls");

	return choice;
}

int HistoryMenu(int currentPage, int lastPage, int historyCounter) {
	int choice, instructionCounter = 0;
	string instructions = "";

    if (historyCounter) {
        if (currentPage != 1) {
            instructions.append(to_string(++instructionCounter) + ". Prev | ");
        }
        if (currentPage!= lastPage || currentPage!= 1) {
            instructions.append(to_string(++instructionCounter) + ". Jump | ");
        }
        if (currentPage != lastPage) {
            instructions.append(to_string(++instructionCounter) + ". Next | ");
        }
        
        instructions.append(to_string(++instructionCounter) + ". Delete a History | ");
        instructions.append(to_string(++instructionCounter) + ". Delete History | ");
        instructions.append(to_string(++instructionCounter) + ". Update a History | ");
    }

	do	{
        SpaceOption(); cout << instructions << "0. Exit\n\n";
        SpaceOption(); cout << ":: ";
        cin >> choice;

        system("cls");

        if (!choice) return 0;
        else break;
	} while (choice < 1 && choice > instructionCounter);

	system("cls");

	if (currentPage == 1 && currentPage == lastPage) return choice + 2;
	else if (currentPage == 1) return ++choice;
	else if (currentPage == lastPage && choice != 2) return ++choice;
    else return choice;
}

void AddHistory(History*& head, History*& tail, History*& temp) {
    SpaceOption();
    if (temp->data->mathOperator.length() == 1)  cout
        << temp->data->val[0] << " "
        << temp->data->mathOperator << " "
        << temp->data->val[1] << " = "
        << temp->data->result << "\n";

	if (!head)
		head = temp;
	else{
		tail->next = temp;
        temp->prev = tail;
    }
    tail = temp;

    tail->next = NULL;

	system("pause");
	system("cls");
}

void DisplayHistory(History* head, int currentPage, int lastPage) {
    const int SIZE = 100;
    time_t now = time(NULL);
    tm *currTime = localtime(&now);
    char time[SIZE];
    int counter = 0, startingIndex = (currentPage - 1) * 5 + 1, index = 1;

    strftime(time, SIZE, "[%I:%M %p]", currTime);

    if (!head) {
        MenuSpace(); cout << "-- No History Yet! --\n";
        return;
    }
    else {
        MenuSpace(); SpaceOption(); DisplaySpace(); DisplaySpace(); cout << "-- History --\n\n";
        MenuSplitter();
        SpaceOption(); cout << "Page " << currentPage << " of " << lastPage << "\n\n";
    }

    SpaceOption(); cout << "Index"; DisplaySpace(); cout << "Time";
            DisplaySpace(); DisplaySpace(); cout << "Calculation\n\n";
    while (head && counter < 5) {
        if (startingIndex <= index) {
            SpaceOption(); cout << index << setw(9) << " "  << time << setw(4) << " "; head->data->Display();
            counter++;
        } 

        head = head->next;
        index++;
    }
    cout << "\n";
}

History* DuplicateAHistory(History* head, int index) {
    History* temp = head;
    History* duplicatedCalculation = new History;
    duplicatedCalculation->data = new DataHolder;

    for (int i = 1; i < index && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        SpaceOption(); cout << "Invalid index." << endl;
        return NULL;
    }

    *duplicatedCalculation->data = *temp->data;

    return duplicatedCalculation;
}

void UpdateAHistory(History*& head, History*& tail) {
    int index;
    double newValue;
    History* duplicatedCalculation;

    MenuSpace(); cout << "-- Update a History --\n\n";
    SpaceOption(); cout << "Enter the index of the history you want to update: ";
    cin >> index;

    duplicatedCalculation = DuplicateAHistory(head, index);
    if (duplicatedCalculation == NULL) {
        return;
    }

    SpaceOption(); cout << "Current Calculation: "; duplicatedCalculation->data->Display();

    SpaceOption(); cout << "Enter a new value for the second value: ";
    cin >> newValue;

    duplicatedCalculation->data->val[1] = newValue;
    switch (duplicatedCalculation->data->mathOperator[0]) {
        case '+':
            duplicatedCalculation->data->result = Add(duplicatedCalculation->data->val, DataHolder::MAX);
            break;
        case '-':
            duplicatedCalculation->data->result = Subtract(duplicatedCalculation->data->val, DataHolder::MAX);
            break;
        case '*':
            duplicatedCalculation->data->result = Multiplication(duplicatedCalculation->data->val, DataHolder::MAX);
            break;
        case '/':
            if (newValue == 0) {
                cout << "Error: Division by zero.\n";
                return;
            }
            duplicatedCalculation->data->result = Division(duplicatedCalculation->data->val, DataHolder::MAX);
            break;
        default:
            cout << "Error: Invalid operator.\n";
            return;
    }

    *duplicatedCalculation->data = DataHolder(*duplicatedCalculation->data);

    AddHistory(head, tail, duplicatedCalculation);

    system("pause");
    system("cls");
    return;
}

void ClearHistory(History*& head, int& historyCounter) {
    History* temp,* current = head;
    if (!head){
        MenuSpace(); cout << "-- No History Yet! --\n\n";
        }
    else {
        while(temp) {
            temp = current->next;
            delete current;
            current = temp;
        }
        SpaceOption(); cout << "-- History Clear! --\n";
        historyCounter = 0;
    }
    system("pause");
    system("cls");
}

void DeleteAHistory(History*& head) {
    History* temp = head;
    History* prev = NULL;
    int count = 1, index;

    MenuSpace(); cout << "-- Delete a History --\n\n"; 
    SpaceOption(); cout << "Enter the index of the history you want to delete: ";
    cin >> index;

    if (index == 1) {
        head = head->next;
        delete temp;
        return;
    }

    while (temp != NULL && count < index) {
        prev = temp;
        temp = temp->next;
        count++;
    }

    if (temp == NULL) {
        SpaceOption(); cout << "Invalid index.\n";
        return;
    }

    prev->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = prev;
    }

    delete temp;

    SpaceOption(); cout << "Deleted history at index " << index << ".\n";
    system("pause");
    system("cls");
}

void InputValue(History*& history) {
    history = new History;
    cout << "\n\n";
    for (int i = 0; i < history->data->MAX; i++) {
		 SpaceOption(); cout << "Value " << i + 1 << ": ";
		 cin >> history->data->val[i];
	}
    cout << "\n";
}

void Computation(int choice, History*& history) {
    switch (choice) {
	case 1:
		history->data->mathOperator = "+";
		history->data->result = Add(
                                history->data->val,
                                history->data->MAX
                        );
		break;
	case 2:
		history->data->mathOperator = "-";
		history->data->result = Subtract(
                                history->data->val,
                                history->data->MAX
                        );
		break;
	case 3:
		history->data->mathOperator = "*";
		history->data->result = Multiplication(
                                history->data->val,
                                history->data->MAX
                        );
		break;
	case 4:
		if (!history->data->val[1]) {
			cout << "Division Input Error!\n";
			history->data->mathOperator = "Division Input Error!";
			break;
		}
		history->data->mathOperator = "/";
		history->data->result = Division(
                                history->data->val,
                                history->data->MAX
                        );
		break;
	default:
        system("cls");
		break;
	}
}

int HistoryFunctionalities(int& historyCounter, History*& head, History*& tail) {
    History* currentNode = head;
    int currentPage = 1, lastPage = (historyCounter - 1) / 5 + 1;

    while(currentPage <= lastPage) {
        DisplayHistory(currentNode, currentPage, lastPage);
            switch(HistoryMenu(currentPage, lastPage, historyCounter)) {
            case 1: 
                if (currentPage > 1) 
                currentPage--;
                break;
            case 2: 
                JumpToPage(currentPage, lastPage);
                break;
            case 3:
                if (currentPage < lastPage) 
                currentPage++;
                break;
            case 4:
                DeleteAHistory(head);
                system("pause");
                break;
            case 5:
                ClearHistory(head, historyCounter);
                tail = NULL;
                head = NULL;
                system("pause");
                return 0;
                break;
            case 6:
                UpdateAHistory(head, tail);
                system("pause");
                break;
            case 0:
                return 0;
            default:
            cout << "Invalid choice" << endl;
            system("cls");
            break;
            }
        }
    return 0;
}

void JumpToPage(int& currentPage, int lastPage){
    int pageNumber;

    MenuSpace(); cout << "-- Page Jump --\n\n";
    SpaceOption(); cout << "Enter the page number you want to jump to: ";
    cin >> pageNumber;

    if (pageNumber > 0 && pageNumber <= lastPage) {
        currentPage = pageNumber;
    } else {
        cout << "Invalid page number\n";
    }

    system("pause");
    system("cls");
}


double Add(double val[], const int MAX) {
    double total = 0;
    for (int i = 0; i < MAX; i++)
        total += val[i];
    return total;
}

double Subtract(double val[], const int MAX) {
    double total = val[0];
    for (int i = 1; i < MAX; i++)
        total -= val[i];
    return total;
}

double Division(double val[], const int MAX) {
    double total = val[0];
    for (int i = 1; i < MAX; i++)
        total /= val[i];
    return total;
}

double Multiplication(double val[], const int MAX) {
    double total = val[0];
    for (int i = 1; i < MAX; i++)
        total *= val[i];
    return total;
}
