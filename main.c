#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_HOUSINGS 100
#define ACCOUNTS_FILE "accounts.dat"
#define HOUSINGS_FILE "housings.dat"

// Структура для зберігання інформації про акаунт
typedef struct {
    char email[50];
    char password[50];
    char username[50];
    char phone[15];
    char orderHistory[500];
    char currentBookings[200];
    char preferences[200];
    float rating;
    int ratingCount;
} Account;

// Структура для зберігання інформації про житло
typedef struct {
    char name[100];
    char description[500];
    char category[50];
    char region[50];
    char district[50];
    int floors;
    int totalFloors;
    int rooms;
    char repairStatus[50];
    int buildYear;
    char address[100];
    float price;
    int isBooked;
    char bookedBy[50];
    Account postedBy;
    float rating;
    int ratingCount;
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
    "Лофт",
    "Таун-хаус"
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
int districtCounts[] = {10, 4, 8, 6, 7};

void displayMenu();
void createAccount();
void viewAccount();
void addHousing();
void viewHousing();
void findHousing();
void rateService();
void rateTenant(int landlordIndex);
void rateLandlord(int tenantIndex);
void printHousingTypeOptions();
void printRegionOptions();
void printDistrictOptions(int regionIndex);
void loadAccounts();
void loadHousings();
void saveAccounts();
void saveHousings();
int login(char *email, char *password);
void listUserHousings(char *email);
void viewHousingDetails(int index);
int selectRegion();
int selectDistrict(int regionIndex);
void listHousingsInDistrict(int regionIndex, int districtIndex);
void bookHousing(int index, char *email);
void viewBookings(char *email);
void cancelBooking(char *email);
void cancelTenantBooking(int housingIndex);

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
                saveAccounts();
                break;
            case 2:
                viewAccount();
                break;
            case 3:
                addHousing();
                saveHousings();
                break;
            case 4:
                viewHousing();
                break;
            case 5:
                findHousing();
                saveHousings();
                break;
            case 6:
                rateService();
                saveAccounts();
                saveHousings();
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
    printf("4. Переглянути моє житло\n");
    printf("5. Знайти житло\n");
    printf("6. Оцінити сервіс\n");
    printf("0. Вихід\n");
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Ліміт акаунтів досягнуто. Не можна створити більше акаунтів.\n");
        return;
    }

    Account *newAccount = &accounts[accountCount++];
    printf("\nВведіть електронну пошту: ");
    scanf("%s", newAccount->email);
    printf("Введіть пароль: ");
    scanf("%s", newAccount->password);
    printf("Введіть ім'я користувача: ");
    scanf("%s", newAccount->username);
    printf("Введіть номер телефону: ");
    scanf("%s", newAccount->phone);
    strcpy(newAccount->orderHistory, "Ще немає замовлень.");
    strcpy(newAccount->currentBookings, "Ще немає бронювань.");
    strcpy(newAccount->preferences, "Ще немає вподобань.");
    newAccount->rating = 0;
    newAccount->ratingCount = 0;
    printf("Акаунт успішно створено.\n");
}

void viewAccount() {
    char email[50], password[50];
    printf("Введіть електронну пошту: ");
    scanf("%s", email);
    printf("Введіть пароль: ");
    scanf("%s", password);

    int index = login(email, password);
    if (index != -1) {
        printf("Електронна пошта: %s\n", accounts[index].email);
        printf("Ім'я користувача: %s\n", accounts[index].username);
        printf("Номер телефону: %s\n", accounts[index].phone);
        printf("Рейтинг: %.1f\n", accounts[index].rating / (accounts[index].ratingCount ? accounts[index].ratingCount : 1));

        int choice;
        do {
            printf("\n1. Переглянути заброньовані об'єкти\n");
            printf("2. Відмовитись від бронювання\n");
            printf("0. Повернутися до головного меню\n");
            printf("Ваш вибір: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    viewBookings(accounts[index].email);
                    break;
                case 2:
                    cancelBooking(accounts[index].email);
                    break;
                case 0:
                    break;
                default:
                    printf("Неправильний вибір. Спробуйте ще раз.\n");
            }
        } while (choice != 0);
    } else {
        printf("Неправильна електронна пошта або пароль.\n");
    }
}

void addHousing() {
    if (housingCount >= MAX_HOUSINGS) {
        printf("Ліміт житла досягнуто. Не можна додати більше житла.\n");
        return;
    }

    char email[50], password[50];
    printf("Введіть електронну пошту: ");
    scanf("%s", email);
    printf("Введіть пароль: ");
    scanf("%s", password);

    int index = login(email, password);
    if (index == -1) {
        printf("Неправильна електронна пошта або пароль.\n");
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
        if (districtChoice >= 1 && districtChoice <= districtCounts[regionChoice - 1]) {
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

    if (strcmp(newHousing->category, "Квартира") == 0) {
        printf("Введіть кількість поверхів квартири: ");
        scanf("%d", &newHousing->floors);
        printf("Введіть загальну кількість поверхів у будинку: ");
        scanf("%d", &newHousing->totalFloors);
    } else {
        printf("Введіть кількість поверхів у будинку: ");
        scanf("%d", &newHousing->floors);
        newHousing->totalFloors = newHousing->floors;
    }

    printf("Введіть кількість кімнат: ");
    scanf("%d", &newHousing->rooms);
    printf("Введіть статус ремонту: ");
    scanf(" %[^\n]s", newHousing->repairStatus);
    printf("Введіть рік будівництва: ");
    scanf("%d", &newHousing->buildYear);
    printf("Введіть адресу: ");
    scanf(" %[^\n]s", newHousing->address);
    printf("Введіть ціну за добу оренди (у гривнях): ");
    scanf("%f", &newHousing->price);
    newHousing->isBooked = 0;
    strcpy(newHousing->bookedBy, "");
    newHousing->postedBy = accounts[index];
    newHousing->rating = 0;
    newHousing->ratingCount = 0;

    printf("Житло успішно додано.\n");
}

void viewHousing() {
    char email[50], password[50];
    printf("Введіть електронну пошту: ");
    scanf("%s", email);
    printf("Введіть пароль: ");
    scanf("%s", password);

    int index = login(email, password);
    if (index != -1) {
        listUserHousings(email);
    } else {
        printf("Неправильна електронна пошта або пароль.\n");
    }
}

void findHousing() {
    int regionIndex = selectRegion();
    if (regionIndex == -1) return;

    int districtIndex = selectDistrict(regionIndex);
    if (districtIndex == -1) return;

    listHousingsInDistrict(regionIndex, districtIndex);

    int choice;
    do {
        printf("Виберіть номер житла для перегляду деталей (0 для повернення): ");
        scanf("%d", &choice);
        if (choice > 0 && choice <= housingCount) {
            viewHousingDetails(choice - 1);
            if (!housings[choice - 1].isBooked) {
                printf("1 - Забронювати, 0 - Назад до списку: ");
                int bookChoice;
                scanf("%d", &bookChoice);
                if (bookChoice == 1) {
                    char email[50];
                    printf("Введіть вашу електронну пошту для бронювання: ");
                    scanf("%s", email);
                    bookHousing(choice - 1, email);
                    break;
                }
            } else {
                printf("Це житло вже заброньоване.\n");
            }
        } else if (choice != 0) {
            printf("Неправильний вибір. Спробуйте ще раз.\n");
        }
    } while (choice != 0);
}

void rateService() {
    char email[50], password[50];
    printf("Введіть електронну пошту: ");
    scanf("%s", email);
    printf("Введіть пароль: ");
    scanf("%s", password);

    int userIndex = login(email, password);
    if (userIndex == -1) {
        printf("Неправильна електронна пошта або пароль.\n");
        return;
    }

    int choice;
    printf("Виберіть опцію:\n");
    printf("1. Оцінити орендаря\n");
    printf("2. Оцінити орендодавця\n");
    printf("0. Повернутися\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            rateTenant(userIndex);
            break;
        case 2:
            rateLandlord(userIndex);
            break;
        case 0:
            return;
        default:
            printf("Неправильний вибір.\n");
            return;
    }
}

void rateTenant(int landlordIndex) {
    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].postedBy.email, accounts[landlordIndex].email) == 0 && housings[i].isBooked) {
            printf("%d. %s, %s, %s (Заброньовано: %s)\n", i + 1, housings[i].category, housings[i].name, housings[i].address, housings[i].bookedBy);
            found = 1;
        }
    }

    if (!found) {
        printf("У вас немає заброньованого житла для оцінки орендаря.\n");
        return;
    }

    int housingChoice;
    printf("Виберіть номер житла, орендаря якого ви хочете оцінити: ");
    scanf("%d", &housingChoice);

    if (housingChoice < 1 || housingChoice > housingCount || !housings[housingChoice - 1].isBooked || 
        strcmp(housings[housingChoice - 1].postedBy.email, accounts[landlordIndex].email) != 0) {
        printf("Неправильний вибір житла.\n");
        return;
    }

    float tenantRating;
    printf("Оцініть орендаря від 1 до 5: ");
    scanf("%f", &tenantRating);

    if (tenantRating < 1 || tenantRating > 5) {
        printf("Неправильний рейтинг. Будь ласка, введіть число від 1 до 5.\n");
        return;
    }

    // Знаходження орендаря та оновлення його рейтингу
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].email, housings[housingChoice - 1].bookedBy) == 0) {
            accounts[i].rating += tenantRating;
            accounts[i].ratingCount++;
            printf("Дякуємо за вашу оцінку орендаря!\n");
            return;
        }
    }

    printf("Помилка: орендар не знайдений.\n");
}

void rateLandlord(int tenantIndex) {
    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].bookedBy, accounts[tenantIndex].email) == 0) {
            printf("%d. %s, %s, %s (Орендодавець: %s)\n", i + 1, housings[i].category, housings[i].name, housings[i].address, housings[i].postedBy.username);
            found = 1;
        }
    }

    if (!found) {
        printf("У вас немає заброньованого житла для оцінки орендодавця.\n");
        return;
    }

    int housingChoice;
    printf("Виберіть номер житла, орендодавця якого ви хочете оцінити: ");
    scanf("%d", &housingChoice);

    if (housingChoice < 1 || housingChoice > housingCount || 
        strcmp(housings[housingChoice - 1].bookedBy, accounts[tenantIndex].email) != 0) {
        printf("Неправильний вибір житла.\n");
        return;
    }

    float landlordRating;
    printf("Оцініть орендодавця від 1 до 5: ");
    scanf("%f", &landlordRating);

    if (landlordRating < 1 || landlordRating > 5) {
        printf("Неправильний рейтинг. Будь ласка, введіть число від 1 до 5.\n");
        return;
    }

    // Оновлення рейтингу орендодавця
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].email, housings[housingChoice - 1].postedBy.email) == 0) {
            accounts[i].rating += landlordRating;
            accounts[i].ratingCount++;
            printf("Дякуємо за вашу оцінку орендодавця!\n");
            return;
        }
    }

    printf("Помилка: орендодавець не знайдений.\n");
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
    for (int i = 0; i < districtCounts[regionIndex]; i++) {
        printf("%d. %s\n", i + 1, districts[regionIndex][i]);
    }
}

void loadAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (file == NULL) {
        file = fopen(ACCOUNTS_FILE, "wb");
        if (file == NULL) {
            printf("Не вдалося створити файл акаунтів.\n");
            return;
        }
        fclose(file);
        printf("Створено новий файл акаунтів.\n");
        return;
    }

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
        file = fopen(HOUSINGS_FILE, "wb");
        if (file == NULL) {
            printf("Не вдалося створити файл житла.\n");
            return;
        }
        fclose(file);
        printf("Створено новий файл житла.\n");
        return;
    }

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

int login(char *email, char *password) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].email, email) == 0 && strcmp(accounts[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

void listUserHousings(char *email) {
    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].postedBy.email, email) == 0) {
            printf("%d. %s, %s, %s, %s", i + 1, housings[i].region, housings[i].district, housings[i].category, housings[i].name);
            if (housings[i].isBooked) {
                printf(" (Заброньовано: %s)\n", housings[i].bookedBy);
            } else {
                printf(" (Доступно)\n");
            }
            found = 1;
        }
    }
    if (!found) {
        printf("У вас ще немає доданого житла.\n");
    } else {
        int choice;
        printf("Виберіть номер житла для перегляду деталей (0 для повернення): ");
        scanf("%d", &choice);
        if (choice > 0 && choice <= housingCount) {
            viewHousingDetails(choice - 1);
            if (housings[choice - 1].isBooked) {
                printf("Бажаєте відмінити бронювання? (1 - Так, 0 - Ні): ");
                int cancelChoice;
                scanf("%d", &cancelChoice);
                if (cancelChoice == 1) {
                    cancelTenantBooking(choice - 1);
                }
            }
        }
    }
}

void viewHousingDetails(int index) {
    printf("\nНазва: %s\n", housings[index].name);
    printf("Опис: %s\n", housings[index].description);
    printf("Тип: %s\n", housings[index].category);
    printf("Регіон: %s, Район: %s\n", housings[index].region, housings[index].district);
    printf("Поверхів: %d", housings[index].floors);
    if (strcmp(housings[index].category, "Квартира") == 0) {
        printf(" (з %d)\n", housings[index].totalFloors);
    } else {
        printf("\n");
    }
    printf("Кількість кімнат: %d\n", housings[index].rooms);
    printf("Статус ремонту: %s\n", housings[index].repairStatus);
    printf("Рік будівництва: %d\n", housings[index].buildYear);
    printf("Адреса: %s\n", housings[index].address);
    printf("Ціна за добу: %.2f грн\n", housings[index].price);
    printf("Доступність: %s\n", housings[index].isBooked ? "Не доступно" : "Доступно");
    if (housings[index].isBooked) {
        printf("Заброньовано користувачем: %s\n", housings[index].bookedBy);
    }
    printf("Оголошення розмістив: %s\n", housings[index].postedBy.username);
    printf("Номер телефону: %s\n", housings[index].postedBy.phone);
    printf("Рейтинг: %.1f\n", housings[index].rating / (housings[index].ratingCount ? housings[index].ratingCount : 1));
}

int selectRegion() {
    printf("Виберіть регіон:\n");
    printRegionOptions();
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > regionCount) {
        printf("Неправильний вибір регіону.\n");
        return -1;
    }
    return choice - 1;
}

int selectDistrict(int regionIndex) {
    printf("Виберіть район:\n");
    printDistrictOptions(regionIndex);
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > districtCounts[regionIndex]) {
        printf("Неправильний вибір району.\n");
        return -1;
    }
    return choice - 1;
}

void listHousingsInDistrict(int regionIndex, int districtIndex) {
    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].region, regions[regionIndex]) == 0 &&
            strcmp(housings[i].district, districts[regionIndex][districtIndex]) == 0) {
            printf("%d. %s, %s, кімнат: %d, %.2f грн/добу, %s\n", 
                   i + 1, housings[i].category, housings[i].name, housings[i].rooms, housings[i].price,
                   housings[i].isBooked ? "Заброньовано" : "Доступно");
            found = 1;
        }
    }
    if (!found) {
        printf("У вибраному районі немає доступного житла.\n");
    }
}

void bookHousing(int index, char *email) {
    if (housings[index].isBooked) {
        printf("На жаль, це житло вже заброньоване.\n");
    } else {
        housings[index].isBooked = 1;
        strcpy(housings[index].bookedBy, email);
        printf("Ви успішно забронювали це житло.\n");
    }
}

void viewBookings(char *email) {
    int found = 0;
    for (int i = 0; i < housingCount; i++) {
        if (strcmp(housings[i].bookedBy, email) == 0) {
            printf("%d. %s, %s, %s, %s\n", i + 1, housings[i].region, housings[i].district, housings[i].category, housings[i].name);
            found = 1;
        }
    }
    if (!found) {
        printf("У вас немає заброньованих об'єктів.\n");
    }
}

void cancelBooking(char *email) {
    viewBookings(email);
    int choice;
    printf("Виберіть номер бронювання для скасування (0 для повернення): ");
    scanf("%d", &choice);
    if (choice > 0 && choice <= housingCount) {
        if (strcmp(housings[choice - 1].bookedBy, email) == 0) {
            housings[choice - 1].isBooked = 0;
            strcpy(housings[choice - 1].bookedBy, "");
            printf("Бронювання успішно скасовано.\n");
        } else {
            printf("Ви не можете скасувати це бронювання.\n");
        }
    } else if (choice != 0) {
        printf("Неправильний вибір.\n");
    }
}

void cancelTenantBooking(int housingIndex) {
    if (housings[housingIndex].isBooked) {
        housings[housingIndex].isBooked = 0;
        strcpy(housings[housingIndex].bookedBy, "");
        printf("Бронювання успішно скасовано.\n");
    } else {
        printf("Це житло не заброньоване.\n");
    }
}