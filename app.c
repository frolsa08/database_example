#include "list.h"
#include "user.h"
#include "app.h"
#include "database.h"


int cmp_up(const void *a, const void *b) {
    return *(double*)a - *(double*)b;
 }

 int cmp_down(const void *a, const void *b) {
    return  *(double*)b - *(double*)a ;
 }

void App_sort_by_salary(List* list, int direction){
    int total = List_count(list);
    int order[total];
    double salaries[total];
    double old_salaries[total];

    // get the salary from list
    for (int i=0; i<total; i++) {
		User* user = (User*)List_at(list, i);
		order[i] = i;
		salaries[i] = user->salary;
        old_salaries[i] = user->salary;
	}

 	// qsor of salary
	if (direction == 1) {
    	qsort(salaries, total, sizeof(double), cmp_up);
		}
	else{
		qsort(salaries, total, sizeof(double), cmp_down);
	}


    //compare old and new salary
    for (int i = 0; i < total; i++){
        for (int j = 0; j < total; j++){
            if (salaries[i] == old_salaries[j]){
                order[i] = j;
            }
        }  
    }

    // copy structure
    User* pUsers = (User*) malloc(sizeof(User)*total);

    for (int i=0; i<total; i++){
		User* user = (User*)List_at(list, i);
		strcpy(pUsers[i].name, user->name);
		strcpy(pUsers[i].surname, user->surname);
		pUsers[i].age = user->age;
		pUsers[i].salary = user->salary;
	}

    // new value from sturcture
    for (int i=0; i<total; i++) {
        User* user = (User*)List_at(list, i);
		strcpy(user->name, pUsers[order[i]].name);
		strcpy(user->surname, pUsers[order[i]].surname);
        user -> age = pUsers[order[i]].age;
		user -> salary = pUsers[order[i]].salary;
	}
    //free memory
    free(pUsers);

}


void App_deleteAllUsersFromList(List* root) {
	for (int i=List_count(root)-1; i>=0; i--){
		User* user = (User*)List_at(root, i);

		printf("%.1d) %s deleted\n", i, user->name);
		User_delete(user);
		root = List_delete(root, i);
	}
}

void App_printAllUserFromList(List* root) {
	printf("+---------------+---------------+---------------+---------------+\n");
	printf("|      id       |     name      |      age      |     salary    |\n");
	printf("+---------------+---------------+---------------+---------------+\n");
	
	for (int i=0; i<List_count(root); i++){
		User* user = (User*)List_at(root, i);

		printf("|%15i|%15s|%15i|%15.lf|\n", i, user->name, user->age, user->salary);
		printf("+---------------+---------------+---------------+---------------+\n");
	}
	printf("\n");
}

int App_showMenu() {
	printf("\n---------    Menu     ---------\n");
	printf("[1] Print all users\n");
	printf("[2] Add new user\n");
	printf("[3] Delete user\n");
	printf("[4] Load from DB\n");
	printf("[5] Save to DB\n");
	printf("[6] Exit\n");
	printf("[7] Sort by salary\n");
	printf("[8] Filter by salary\n");
	printf("[9] Filter by name\n");
	printf("[10] Sort by name\n");
	printf("\n");
	printf("Input action [1-10]:\n");
	printf("\n-------------------------------\n");
	int id;
	scanf("%d", &id);
	return id;
}


int App_run() {
	List* root = NULL;
	int action = 0;
	DataBase_load(root);
	while (action!=6) {
		action = App_showMenu();
		
		switch(action) {
			case 1: {
				App_printAllUserFromList(root);
			} break;

			case 2: {
				User tmp;
				
				printf("Input user name: ");
				scanf("%s", tmp.name);
			
				printf("Input user surname: ");
				scanf("%s", tmp.surname);
				
				printf("Input user age: ");
				scanf("%d", &tmp.age);
				
				printf("Input user salary: ");
				scanf("%lf", &tmp.salary);
				
				root = List_push(root, User_create(tmp.name, tmp.surname, tmp.age, tmp.salary));
			} break;

			case 3: {
				int id;
				
				printf("Input user id: ");
				scanf("%d", &id);
				
				User_delete((User*)List_at(root, id));
				root = List_delete(root, id);
			} break;

			case 4: {
				root = DataBase_load(root);
			} break;

			case 5: {
				root = DataBase_save(root);
			} break;

			case 7: {
				int direction = 0;
				printf("[1] From min to max\n");
				printf("[2] From max to min\n");
				scanf("%d", &direction);

				printf("Sorted by salary.\n");
				App_sort_by_salary(root, direction);
			} break;

			case 8: {
				double min_val;
				double max_val;

				printf("Enter min value of salary:\n");
				scanf("%lf", &min_val);
				
				printf("Enter max value of salary:\n");
				scanf("%lf", &max_val);

				//App_filter_by_salary(root, min_val, max_val);
			} break;
            
			case 9: {
				char name[NAME_LENGTH];

				printf("Enter name:\n");
				scanf("%s", name);

				//App_filter_by_name(root, name);
			} break;

			case 10: {
				int direction;
				printf("[1] From min to max\n");
				printf("[2] From max to min\n");
				scanf("%d", &direction);

				printf("Sorted by name.\n");
				//App_sort_by_name(root, direction);
			} break;
		}
	}
    App_deleteAllUsersFromList(root);
    
    return 0;
}