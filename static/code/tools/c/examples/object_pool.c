// Pooling

#define LIST_SIZE 10
Person *list[LIST_SIZE];

void initializeList() {
 for(int i=0; i<LIST_SIZE; i++) {
 list[i] = NULL;
 }
}

Person *getPerson() {
 for(int i=0; i<LIST_SIZE; i++) {
 if(list[i] != NULL) {
 Person *ptr = list[i];
 list[i] = NULL;
 return ptr;
 }
 }
 Person *person = (Person*)malloc(sizeof(Person));
 return person;
}

Person *returnPerson(Person *person) {
 for(int i=0; i<LIST_SIZE; i++) {
 if(list[i] == NULL) {
 list[i] = person;
 return person;
 }
 }
 deallocatePerson(person);
 free(person);
 return NULL;
}

initializeList();
Person *ptrPerson;
ptrPerson = getPerson();
initializePerson(ptrPerson,"Ralph","Fitsgerald","Mr.",35);
displayPerson(*ptrPerson);
returnPerson(ptrPerson);