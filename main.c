#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_HOUSINGS 100
#define ACCOUNTS_FILE "accounts.dat"
#define HOUSINGS_FILE "housings.dat"

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
    char region[50];
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
    "Квартира",
    "Будинок",
    "Вілла",
    "Студія",
    "Лофт"
};
int housingTypesCount = sizeof(housingTypes) / sizeof(housingTypes[0]);

const char *regions[] = {
    "Київ",
    "Одеса",
    "Харків",
    "Львів",
    "Івано-Франківськ"
};
const char *districts[][10] = {
    {"Голосіївський", "Дарницький", "Деснянський", "Дніпровський", "Оболонський", "Печерський", "Подільський", "Святошинський", "Солом'янський", "Шевченківський"},
    {"Київський", "Малиновський", "Приморський", "Суворовський"},
    {"Індустріальний", "Київський", "Московський", "Немишлянський", "Новобаварський", "Слобідський", "Холодногірський", "Шевченківський"},
    {"Галицький", "Залізничний", "Личаківський", "Сихівський", "Франківський", "Шевченківський"},
    {"Бистриця", "Вовчинець", "Крихівці", "Микитинці", "Пасічна", "Угорники", "Хриплин"}
};
int regionCount = sizeof(regions) / sizeof(regions[0]);

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
void printRegionOptions();
void printDistrictOptions(int regionIndex);
void loadAccounts();
void loadHousings();
void saveAccounts();
void saveHousings();

int main() {
    loadAccounts();
    loadHousings();

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

    saveAccounts();
    saveHousings();

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

    printf("Виберіть регіон:\n");
    printRegionOptions();
    int regionChoice;
    scanf("%d", &regionChoice);
    if (regionChoice >= 1 && regionChoice <= regionCount) {
        strcpy(newHousing->region, regions[regionChoice - 1]);
        printf("Виберіть район:\n");
        printDistrictOptions(regionChoice - 1);
        int districtChoice;
        scanf("%d", &districtChoice);
        if (districtChoice >= 1 && districtChoice <= 10) {
            strcpy(newHousing->district, districts[regionChoice - 1][districtChoice - 1]);
        } else {
            printf("Неправильний вибір району. Житло не додано.\n");
            housingCount--;
            return;
        }
    } else {
        printf("Неправильний вибір регіону. Житло не додано.\n");
        housingCount--;
        return;
    }

    printf("Введіть кількість поверхів: ");
    scanf("%d", &newHousing->floors);
    printf("Введіть статус ремонту: ");
    scanf(" %[^\n]s", newHousing->repairStatus);
    printf("Введіть рік будівництва: ");
    scanf("%d", &newHousing->buildYear);
    printf("Введіть адресу: ");
    scanf(" %[^\n]s", newHousing->address);
    printf("Введіть ціну за добу оренди: ");
    scanf("%f", &newHousing->price);
    newHousing->isBooked = 0;
    newHousing->postedBy = accounts[rand() % accountCount];

    printf("Житло успішно додано.\n");
}

void viewHousing() {
    char housingName[50];
    printf("Введіть назву житла для перегляду: ");
    scanf(" %[^\n]s", housingName);

    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].name, housingName) == 0) {
            printf("Назва: %s\n", housings[i].name);
            printf("Опис: %s\n", housings[i].description);
            printf("Тип: %s\n", housings[i].category);
            printf("Регіон: %s, Район: %s\n", housings[i].region, housings[i].district);
            printf("Поверхів: %d\n", housings[i].floors);
            printf("Статус ремонту: %s\n", housings[i].repairStatus);
            printf("Рік будівництва: %d\n", housings[i].buildYear);
            printf("Адреса: %s\n", housings[i].address);
            printf("Ціна за добу: %.2f\n", housings[i].price);
            printf("Доступність: %s\n", housings[i].isBooked ? "Не доступно" : "Доступно");
            printf("Оголошення розмістив: %s\n", housings[i].postedBy.username);
            return;
        }
    }
    printf("Житло не знайдено.\n");
}

void findHousing() {
    char region[50];
    printf("Введіть регіон для пошуку житла: ");
    scanf(" %[^\n]s", region);

    printf("Результати пошуку житла в регіоні %s:\n", region);
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].region, region) == 0) {
            printf("Назва: %s, Ціна за добу: %.2f, Доступність: %s\n", housings[i].name, housings[i].price, housings[i].isBooked ? "Не доступно" : "Доступно");
        }
    }
}

void evaluateService() {
    int rating;
    printf("Оцініть сервіс від 1 до 5: ");
    scanf("%d", &rating);

    if (rating >= 1 && rating <= 5) {
        printf("Дякуємо за вашу оцінку %d!\n", rating);
    } else {
        printf("Неправильний рейтинг. Будь ласка, введіть число від 1 до 5.\n");
    }
}

void bookHousing() {
    char housingName[50];
    printf("Введіть назву житла для бронювання: ");
    scanf(" %[^\n]s", housingName);

    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].name, housingName) == 0) {
            if (housings[i].isBooked) {
                printf("Житло вже заброньоване.\n");
            } else {
                housings[i].isBooked = 1;
                printf("Житло успішно заброньоване.\n");
            }
            return;
        }
    }
    printf("Житло не знайдено.\n");
}

void printBuildYearOptions() {
    printf("1. 2000-е роки\n");
    printf("2. 2010-е роки\n");
    printf("3. 2020-е роки\n");
}

void printHousingTypeOptions() {
    for (int i = 0; i < housingTypesCount; i++) {
        printf("%d. %s\n", i + 1, housingTypes[i]);
    }
}

void printRegionOptions() {
    for (int i = 0; i < regionCount; i++) {
        printf("%d. %s\n", i + 1, regions[i]);
    }
}

void printDistrictOptions(int regionIndex) {
    for (int i = 0; i < 10; i++) {
        printf("%d. %s\n", i + 1, districts[regionIndex][i]);
    }
}

void loadAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (file == NULL) {
        // Якщо файл не існує, створюємо пустий файл
        file = fopen(ACCOUNTS_FILE, "wb");
        if (file == NULL) {
            printf("Не вдалося створити файл акаунтів.\n");
            return;
        }
        fclose(file);
        printf("Створено новий файл акаунтів.\n");
        return;
    }

    // Якщо файл існує, читаємо дані з нього
    if (fread(&accountCount, sizeof(int), 1, file) != 1) {
        printf("Помилка при читанні кількості акаунтів.\n");
        fclose(file);
        return;
    }
    if (fread(accounts, sizeof(Account), accountCount, file) != accountCount) {
        printf("Помилка при читанні акаунтів.\n");
        fclose(file);
        return;
    }
    fclose(file);
}

void loadHousings() {
    FILE *file = fopen(HOUSINGS_FILE, "rb");
    if (file == NULL) {
        // Якщо файл не існує, створюємо пустий файл
        file = fopen(HOUSINGS_FILE, "wb");
        if (file == NULL) {
            printf("Не вдалося створити файл житла.\n");
            return;
        }
        fclose(file);
        printf("Створено новий файл житла.\n");
        return;
    }

    // Якщо файл існує, читаємо дані з нього
    if (fread(&housingCount, sizeof(int), 1, file) != 1) {
        printf("Помилка при читанні кількості житла.\n");
        fclose(file);
        return;
    }
    if (fread(housings, sizeof(Housing), housingCount, file) != housingCount) {
        printf("Помилка при читанні житла.\n");
        fclose(file);
        return;
    }
    fclose(file);
}

void saveAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "wb");
    if (file == NULL) {
        printf("Помилка при відкритті файлу акаунтів для запису.\n");
        return;
    }

    if (fwrite(&accountCount, sizeof(int), 1, file) != 1) {
        printf("Помилка при записі кількості акаунтів.\n");
        fclose(file);
        return;
    }
    if (fwrite(accounts, sizeof(Account), accountCount, file) != accountCount) {
        printf("Помилка при записі акаунтів.\n");
        fclose(file);
        return;
    }
    fclose(file);
}

void saveHousings() {
    FILE *file = fopen(HOUSINGS_FILE, "wb");
    if (file == NULL) {
        printf("Помилка при відкритті файлу житла для запису.\n");
        return;
    }

    if (fwrite(&housingCount, sizeof(int), 1, file) != 1) {
        printf("Помилка при записі кількості житла.\n");
        fclose(file);
        return;
    }
    if (fwrite(housings, sizeof(Housing), housingCount, file) != housingCount) {
        printf("Помилка при записі житла.\n");
        fclose(file);
        return;
    }
    fclose(file);
}