#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_HOUSINGS 100

// Структура для зберігання інформації про акаунт
typedef struct {
    char username[50];
    char email[50];
    char phone[15];
    char orderHistory[500];
    char currentBookings[200];
    char preferences[200];
} Account;

// Структура для зберігання інформації про житло
typedef struct {
    char name[50];
    char description[500];
    char category[50];
    char district[50];
    int floors;
    char repairStatus[50];
    int buildYear;
    char address[100];
    float price;
    int isBooked;
    Account postedBy;
} Housing;

// Глобальні змінні
Account accounts[MAX_ACCOUNTS];
Housing housings[MAX_HOUSINGS];
int accountCount = 0;
int housingCount = 0;

const char *housingTypes[] = {
    "Apartment",
    "House",
    "Villa",
    "Studio",
    "Loft"
};
int housingTypesCount = sizeof(housingTypes) / sizeof(housingTypes[0]);

// Прототипи функцій
void displayMenu();
void createAccount();
void viewAccount();
void addHousing();
void viewHousing();
void findHousing();
void evaluateService();
void bookHousing();
void printBuildYearOptions();
void printHousingTypeOptions();

int main() {
    int choice;
    do {
        displayMenu();
        printf("Введіть ваш вибір: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccount();
                break;
            case 3:
                addHousing();
                break;
            case 4:
                viewHousing();
                break;
            case 5:
                findHousing();
                break;
            case 6:
                evaluateService();
                break;
            case 7:
                bookHousing();
                break;
            case 0:
                printf("Вихід з програми.\n");
                break;
            default:
                printf("Неправильний вибір. Спробуйте ще раз.\n");
        }
    } while (choice != 0);

    return 0;
}

void displayMenu() {
    printf("\n--- Сервіс оренди житла ---\n");
    printf("1. Створити акаунт\n");
    printf("2. Переглянути акаунт\n");
    printf("3. Додати житло\n");
    printf("4. Переглянути житло\n");
    printf("5. Знайти житло\n");
    printf("6. Оцінити сервіс\n");
    printf("7. Забронювати житло\n");
    printf("0. Вихід\n");
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Ліміт акаунтів досягнуто. Не можна створити більше акаунтів.\n");
        return;
    }

    Account *newAccount = &accounts[accountCount++];
    printf("Введіть ім'я користувача: ");
    scanf("%s", newAccount->username);
    printf("Введіть електронну пошту: ");
    scanf("%s", newAccount->email);
    printf("Введіть номер телефону: ");
    scanf("%s", newAccount->phone);
    strcpy(newAccount->orderHistory, "Ще немає замовлень.");
    strcpy(newAccount->currentBookings, "Ще немає бронювань.");
    strcpy(newAccount->preferences, "Ще немає вподобань.");
    printf("Акаунт успішно створено.\n");
}

void viewAccount() {
    char username[50];
    printf("Введіть ім'я користувача для перегляду акаунту: ");
    scanf("%s", username);

    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            printf("Ім'я користувача: %s\n", accounts[i].username);
            printf("Електронна пошта: %s\n", accounts[i].email);
            printf("Номер телефону: %s\n", accounts[i].phone);
            printf("Історія замовлень: %s\n", accounts[i].orderHistory);
            printf("Поточні бронювання: %s\n", accounts[i].currentBookings);
            printf("Вподобання: %s\n", accounts[i].preferences);
            return;
        }
    }
    printf("Акаунт не знайдено.\n");
}

void addHousing() {
    if (housingCount >= MAX_HOUSINGS) {
        printf("Ліміт житла досягнуто. Не можна додати більше житла.\n");
        return;
    }

    Housing *newHousing = &housings[housingCount++];
    printf("Введіть назву житла: ");
    scanf(" %[^\n]s", newHousing->name);
    printf("Введіть опис: ");
    scanf(" %[^\n]s", newHousing->description);

    printf("Виберіть тип житла:\n");
    printHousingTypeOptions();
    int typeChoice;
    scanf("%d", &typeChoice);
    if (typeChoice >= 1 && typeChoice <= housingTypesCount) {
        strcpy(newHousing->category, housingTypes[typeChoice - 1]);
    } else {
        printf("Неправильний вибір типу житла. Житло не додано.\n");
        housingCount--;
        return;
    }

    printf("Введіть район: ");
    scanf("%s", newHousing->district);
    printf("Введіть кількість поверхів: ");
    scanf("%d", &newHousing->floors);
    printf("Введіть стан ремонту: ");
    scanf("%s", newHousing->repairStatus);
    printf("Виберіть рік побудови з наступних варіантів:\n");
    printBuildYearOptions();
    scanf("%d", &newHousing->buildYear);
    printf("Введіть адресу: ");
    scanf(" %[^\n]s", newHousing->address);
    printf("Введіть ціну: ");
    scanf("%f", &newHousing->price);
    newHousing->isBooked = 0;

    printf("Введіть ім'я користувача, який викладає оголошення: ");
    char username[50];
    scanf("%s", username);
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            newHousing->postedBy = accounts[i];
            printf("Дякуємо! Ваше оголошення опубліковано:\n");
            printf("Район: %s\n", newHousing->district);
            printf("Поверхів: %d\n", newHousing->floors);
            printf("Стан ремонту: %s\n", newHousing->repairStatus);
            printf("Рік побудови: %d\n", newHousing->buildYear);
            printf("Опис: %s\n", newHousing->description);
            printf("Адреса: %s\n", newHousing->address);
            printf("Телефон: %s\n", newHousing->postedBy.phone);
            return;
        }
    }
    printf("Користувача не знайдено. Житло не додано.\n");
    housingCount--; // Відкат збільшення лічильника житла
}

void viewHousing() {
    for (int i = 0; i < housingCount; i++) {
        printf("\n--- Житло %d ---\n", i + 1);
        printf("Назва: %s\n", housings[i].name);
        printf("Опис: %s\n", housings[i].description);
        printf("Категорія: %s\n", housings[i].category);
        printf("Район: %s\n", housings[i].district);
        printf("Поверхів: %d\n", housings[i].floors);
        printf("Стан ремонту: %s\n", housings[i].repairStatus);
        printf("Рік побудови: %d\n", housings[i].buildYear);
        printf("Адреса: %s\n", housings[i].address);
        printf("Ціна: %.2f\n", housings[i].price);
        printf("Статус: %s\n", housings[i].isBooked ? "Заброньовано" : "Доступне");
        printf("Виклав: %s (Email: %s, Телефон: %s)\n", housings[i].postedBy.username, housings[i].postedBy.email, housings[i].postedBy.phone);
    }
}

void findHousing() {
    char category[50];
    printf("Введіть категорію для пошуку житла: ");
    scanf("%s", category);

    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].category, category) == 0) {
            printf("\n--- Житло %d ---\n", i + 1);
            printf("Назва: %s\n", housings[i].name);
            printf("Опис: %s\n", housings[i].description);
            printf("Категорія: %s\n", housings[i].category);
            printf("Район: %s\n", housings[i].district);
            printf("Поверхів: %d\n", housings[i].floors);
            printf("Стан ремонту: %s\n", housings[i].repairStatus);
            printf("Рік побудови: %d\n", housings[i].buildYear);
            printf("Адреса: %s\n", housings[i].address);
            printf("Ціна: %.2f\n", housings[i].price);
            printf("Статус: %s\n", housings[i].isBooked ? "Заброньовано" : "Доступне");
            printf("Виклав: %s (Email: %s, Телефон: %s)\n", housings[i].postedBy.username, housings[i].postedBy.email, housings[i].postedBy.phone);
            found = 1;
        }
    }

    if (!found) {
        printf("Житла з вказаною категорією не знайдено.\n");
    }
}

void evaluateService() {
    printf("Оцінка сервісу поки не реалізована.\n");
}

void bookHousing() {
    char username[50];
    char housingName[50];
    printf("Введіть ім'я користувача для бронювання: ");
    scanf("%s", username);
    printf("Введіть назву житла для бронювання: ");
    scanf("%s", housingName);

    int userIndex = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("Користувача не знайдено.\n");
        return;
    }

    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].name, housingName) == 0) {
            if (housings[i].isBooked) {
                printf("Житло вже заброньовано.\n");
                return;
            }

            housings[i].isBooked = 1;
            strcat(accounts[userIndex].currentBookings, housings[i].name);
            strcat(accounts[userIndex].currentBookings, ", ");
            printf("Житло успішно заброньовано.\n");
            return;
        }
    }

    printf("Житло не знайдено.\n");
}

void printBuildYearOptions() {
    printf("1. 2000\n");
    printf("2. 2005\n");
    printf("3. 2010\n");
    printf("4. 2015\n");
    printf("5. 2020\n");
}

void printHousingTypeOptions() {
    for (int i = 0; i < housingTypesCount; i++) {
        printf("%d. %s\n", i + 1, housingTypes[i]);
    }
}
