//Автор: Ястребов Глеб
//Описание: Этот файл содержит реализацию системы мониторинга данных с камер видеонаблюдения

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

#define MAX_CAMERAS 10
#define MAX_SNAPSHOTS 10
#define MAX_FILENAME_LENGTH 100
#define MAX_CAMERA_ID 100
#define MAX_DAY 31
#define MAX_MONTH 12
#define MAX_YEAR 100
#define MAX_HOUR 24
#define MAX_MINUTE_SECOND 60
#define MAX_STRING_LENGTH 100

typedef struct {
    char date[11];
    char time[9];
} Snapshot;

typedef struct {
    int id;
    Snapshot snapshots[MAX_SNAPSHOTS];
    int snapshotCount;
    char originalData[100];
} Camera;

typedef struct {
    Camera cameras[MAX_CAMERAS];
    int cameraCount;
    char monitoringFile[MAX_FILENAME_LENGTH];
} MonitoringData;

int prData(const char* date);
int prTime(const char* time);
void addCamera(MonitoringData* data, const char* inputData);
void generateTestData(MonitoringData* data);
void loadDataFromFile(MonitoringData* data, const char* filename);
long getFileSize(const char* filename);
void reloadFileIfUpdated(MonitoringData* data, const char* filename, long* lastSize);
void generateSingleSnapshot(MonitoringData* data);
void sortCamerasByID(MonitoringData* data, int (*compare)(int a, int b));
void sortCamerasByDate(MonitoringData* data, int (*compare)(int a, int b));
int compareAscending(int a, int b);
int compareDescending(int a, int b);
void palld(MonitoringData* data);
void pallda(MonitoringData* data);
void monitoring(MonitoringData* data);
void delallc(MonitoringData* data);
void checkcamID(MonitoringData* data, int id);
void delcamID(MonitoringData* data, int id);
void rndmpoka();
void privsv();
void forcase1(MonitoringData* data);

int main() {
    setlocale(LC_CTYPE, "RUS");
    MonitoringData data = { .cameraCount = 0, .monitoringFile = "" };

    int choice;
    privsv();

    while (1) {
        system("cls");
        printf("Добро пожаловать в меню!\n\n");
        printf("1. Загрузить данные из файла\n");
        printf("2. Добавить камеру вручную\n");
        printf("3. Генерация тестовых данных\n");
        printf("4. Вывести все данные (c выбором сортировки)\n");
        printf("5. Режим мониторинга\n");
        printf("6. Проверка данных камеры по ID\n");
        printf("7. Удалить камеру по ID\n");
        printf("8. Удалить все камеры\n");
        printf("9. Cведения о программе\n");
        printf("10.Выход\n\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: {
            system("cls");
            forcase1(&data);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch(); // ожидание нажатия клавиши
            break;
        }
        case 2: {
            char inputData[100];
            system("cls");
            printf("Введите данные камеры (формат DD.MM.YY_HH:MM:SS_ID): ");
            fgets(inputData, sizeof(inputData), stdin);
            inputData[strcspn(inputData, "\n")] = '\0';
            addCamera(&data, inputData);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 3:
            system("cls");
            generateTestData(&data);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 4:
            system("cls");
            palld(&data);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 5:
            system("cls");
            monitoring(&data);
            break;
        case 6: {
            system("cls");
            printf("Введите ID камеры для проверки: ");
            int checkId;
            scanf("%d", &checkId);
            checkcamID(&data, checkId);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 7: {
            system("cls");
            printf("Введите ID камеры для удаления: ");
            int deleteId;
            scanf("%d", &deleteId);
            delcamID(&data, deleteId);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
        case 8:
            system("cls");
            delallc(&data);
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        case 9:
            privsv();
            break;
        case 10:
            system("cls");
            rndmpoka();
            return 0;
        default:
            system("cls");
            printf("Некорректный выбор.\n");
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
            break;
        }
    }
    return 0;
}

//Функция prData проверяет корректность даты в формате "дд.мм.гг"
int prData(const char* date) {
    int day, month, year;
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) != 3) {
        return 0; //ошибка
    }
    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return 0; //неверная дата
    }
    return 1; //дата верна
}

//Функция prTime проверяет корректность времени в формате "чч:мм:сс"
int prTime(const char* time) {
    int hour, minute, second;
    if (sscanf(time, "%d:%d:%d", &hour, &minute, &second) != 3) {
        return 0; //ошибка
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; //неверное время
    }
    return 1; //корректное время
}

//Функция addCamera добавляет новую камеру или новый снимок для существующей камеры
void addCamera(MonitoringData* data, const char* inputData) {
    int id;
    char date[11], time[9];
    if (sscanf(inputData, "%10[^_]_%8[^_]_%d", date, time, &id) != 3) {
        printf("Ошибка: данные должны быть в формате DD.MM.YY_HH:MM:SS_ID.\n");
        return;
    }

    if (!prData(date) || !prTime(time)) {
        printf("Ошибка: некорректные дата или время.\n");
        return;
    }

    for (int i = 0; i < data->cameraCount; i++) {
        if (data->cameras[i].id == id) {
            for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
                if (strcmp(data->cameras[i].snapshots[j].date, date) == 0 &&
                    strcmp(data->cameras[i].snapshots[j].time, time) == 0) {
                    return;
                }
            }

            if (data->cameras[i].snapshotCount < MAX_SNAPSHOTS) {
                strncpy(data->cameras[i].snapshots[data->cameras[i].snapshotCount].date, date, sizeof(date));
                strncpy(data->cameras[i].snapshots[data->cameras[i].snapshotCount].time, time, sizeof(time));
                data->cameras[i].snapshotCount++;
            }
            else {
                printf("Камера %d достигла максимального количества снимков.\n", id);
            }
            return;
        }
    }

    if (data->cameraCount < MAX_CAMERAS) {
        data->cameras[data->cameraCount].id = id;
        strncpy(data->cameras[data->cameraCount].snapshots[0].date, date, sizeof(date));
        strncpy(data->cameras[data->cameraCount].snapshots[0].time, time, sizeof(time));
        data->cameras[data->cameraCount].snapshotCount = 1;
        strncpy(data->cameras[data->cameraCount].originalData, inputData, sizeof(data->cameras[data->cameraCount].originalData));
        data->cameraCount++;
    }
}

//Функция generateTestData генерирует тестовые данные для камер
void generateTestData(MonitoringData* data) {
    int numLines;
    printf("Введите количество cтрок (снимков с разных камер), которые нужно сгенерировать: ");
    scanf("%d", &numLines);

    srand(time(NULL));

    for (int i = 0; i < numLines; i++) {
        int cameraId = rand() % MAX_CAMERA_ID + 1;
        int day = rand() % MAX_DAY + 1;
        int month = rand() % MAX_MONTH + 1;
        int year = rand() % MAX_YEAR;
        int hour = rand() % MAX_HOUR;
        int minute = rand() % MAX_MINUTE_SECOND;
        int second = rand() % MAX_MINUTE_SECOND;

        char testData[MAX_STRING_LENGTH];
        snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, cameraId);

        addCamera(data, testData);
    }

    printf("Тестовые данные сгенерированы.\n");
}


//Функция loadDataFromFile загружает данные о камерах из файла
void loadDataFromFile(MonitoringData* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка при открытии файла %s.\n", filename);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        addCamera(data, line);
    }
    fclose(file);
    printf("Данные из файла %s загружены.\n", filename);
}

//Функция getFileSize проверяет размер файла
long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb"); //открытие файл в бинарном режиме
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file); //получение данных о размере файла
    fclose(file);
    return size;
}

//Функция reloadFileIfUpdated проверяет, были ли изменения в файле с момента последней проверки
void reloadFileIfUpdated(MonitoringData* data, const char* filename, long* lastSize) {
    long currentSize = getFileSize(filename);
    if (currentSize != *lastSize) {
        *lastSize = currentSize;
        printf("Обнаружены изменения в файле %s. Загрузка новых данных...\n", filename);
        loadDataFromFile(data, filename);
    }
}

//Функция generateSingleSnapshot генерирует одну строку данных для камеры с случайными значениями,
// а затем добавляет её в систему
void generateSingleSnapshot(MonitoringData* data) {
    srand((unsigned int)time(NULL));
    int id = rand() % MAX_CAMERA_ID + 1;
    int day = rand() % MAX_DAY + 1;
    int month = rand() % MAX_MONTH + 1;
    int year = rand() % MAX_YEAR;
    int hour = rand() % MAX_HOUR;
    int minute = rand() % MAX_MINUTE_SECOND;
    int second = rand() % MAX_MINUTE_SECOND;

    char testData[MAX_STRING_LENGTH];
    snprintf(testData, sizeof(testData), "%02d.%02d.%02d_%02d:%02d:%02d_%d", day, month, year, hour, minute, second, id);
    addCamera(data, testData);
    printf("Новая строка данных сгенерирована: %s\n", testData);
}

//Функция sortCamerasByID выполняет сортировку камер по id
void sortCamerasByID(MonitoringData* data, int (*compare)(int a, int b)) {
    for (int i = 0; i < data->cameraCount - 1; i++) {
        for (int j = i + 1; j < data->cameraCount; j++) {
            if (compare(data->cameras[i].id, data->cameras[j].id) > 0) {
                Camera temp = data->cameras[i];
                data->cameras[i] = data->cameras[j];
                data->cameras[j] = temp;
            }
        }
    }
}

//Функция sortCamerasByDate выполняет сортировку снимков по дате
void sortCamerasByDate(MonitoringData* data, int (*compare)(int a, int b)) {
    for (int i = 0; i < data->cameraCount - 1; i++) {
        for (int j = i + 1; j < data->cameraCount; j++) {
            Camera* cameraA = &data->cameras[i];
            Camera* cameraB = &data->cameras[j];

            if (cameraA->snapshotCount == 0 || cameraB->snapshotCount == 0) {
                continue;
            }

            char* dateA = cameraA->snapshots[cameraA->snapshotCount - 1].date;
            char* timeA = cameraA->snapshots[cameraA->snapshotCount - 1].time;
            char* dateB = cameraB->snapshots[cameraB->snapshotCount - 1].date;
            char* timeB = cameraB->snapshots[cameraB->snapshotCount - 1].time;

            int dayA, monthA, yearA, hourA, minuteA, secondA;
            int dayB, monthB, yearB, hourB, minuteB, secondB;

            sscanf(dateA, "%d.%d.%d", &dayA, &monthA, &yearA);
            sscanf(timeA, "%d:%d:%d", &hourA, &minuteA, &secondA);
            sscanf(dateB, "%d.%d.%d", &dayB, &monthB, &yearB);
            sscanf(timeB, "%d:%d:%d", &hourB, &minuteB, &secondB);

            if (compare(yearA, yearB) > 0 ||
                (yearA == yearB && compare(monthA, monthB) > 0) ||
                (yearA == yearB && monthA == monthB && compare(dayA, dayB) > 0) ||
                (yearA == yearB && monthA == monthB && dayA == dayB && compare(hourA, hourB) > 0) ||
                (yearA == yearB && monthA == monthB && dayA == dayB && hourA == hourB && compare(minuteA, minuteB) > 0) ||
                (yearA == yearB && monthA == monthB && dayA == dayB && hourA == hourB && minuteA == minuteB && compare(secondA, secondB) > 0)) {
                Camera temp = data->cameras[i];
                data->cameras[i] = data->cameras[j];
                data->cameras[j] = temp;
            }
        }
    }
}

//для сортировки по возрастанию
int compareAscending(int a, int b) {
    return a - b;
}

//для сортировки по убыванию
int compareDescending(int a, int b) {
    return b - a;
}

//Функция palld выводит данные о камерах и их снимках с возможностью сортировки
void palld(MonitoringData* data) {
    if (data->cameraCount == 0) {
        printf("Нет данных о камерах.\n");
        return;
    }

    int sortOption;
    printf("\nВыберите порядок сортировки:\n");
    printf("1. По умолчанию (без сортировки, как загружено)\n");
    printf("2. По ID (возрастание)\n");
    printf("3. По ID (убывание)\n");
    printf("4. По дате снимка (старые сначала)\n");
    printf("5. По дате снимка (новые сначала)\n");
    printf("Ваш выбор: ");
    scanf("%d", &sortOption);

    switch (sortOption) {
    case 2:
        sortCamerasByID(data, compareAscending);
        break;
    case 3:
        sortCamerasByID(data, compareDescending);
        break;
    case 4:
        sortCamerasByDate(data, compareAscending);
        break;
    case 5:
        sortCamerasByDate(data, compareDescending);
        break;
    default:
        printf("Неверный выбор. Данные не отсортированы.\n");
        return;
    }

    printf("\nВсе данные:\n");
    for (int i = 0; i < data->cameraCount; i++) {
        printf("Камера %d:\n", data->cameras[i].id);
        for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
            printf("Дата: %s, Время: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}


/*
 * Функция pallda выводит все данные о камерах и их снимках
 * Если нет данных о камерах, выводится сообщение об этом
 */
void pallda(MonitoringData* data) {
    if (data->cameraCount == 0) {
        printf("Нет данных о камерах.\n");
        return;
    }
    printf("Все данные:\n");
    for (int i = 0; i < data->cameraCount; i++) {
        printf("Камера %d:\n", data->cameras[i].id);
        for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
            printf("Дата: %s, Время: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * Функция monitoring реализует режим мониторинга, в котором:
 * 1. Проверяется наличие файла для мониторинга и загружаются данные
 * 2. Каждые 5 секунд проверяется, изменился ли размер файла
 *    - Если файл был изменен, происходит перезагрузка данных
 * 3. Каждые 20 секунд генерируется новая запись (снимок)
 * 4. Для выхода из режима мониторинга достаточно нажать любую клавишу
 */
void monitoring(MonitoringData* data) {
    if (strlen(data->monitoringFile) == 0) {
        printf("Файл для мониторинга не загружен. Сначала загрузите файл через меню.\n");
        printf("Нажмите любую клавишу для возврата в меню...\n");
        _getch();
        return;
    }

    long lastSize = getFileSize(data->monitoringFile);
    system("cls");
    printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");

    loadDataFromFile(data, data->monitoringFile);
    pallda(data);

    int elapsed5 = 0;
    int elapsed20 = 0;

    while (1) {
        Sleep(1000);
        elapsed5 += 1;
        elapsed20 += 1;

        if (_kbhit()) {
            _getch();
            system("cls");
            printf("Выход из режима мониторинга.\n");
            break;
        }

        if (elapsed5 == 5) {
            elapsed5 = 0;
            reloadFileIfUpdated(data, data->monitoringFile, &lastSize);
            system("cls");
            printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");
            pallda(data);
        }

        if (elapsed20 == 20) {
            elapsed20 = 0;
            generateSingleSnapshot(data);
            system("cls");
            printf("Режим мониторинга активен. Нажмите любую клавишу для выхода.\n");
            pallda(data);
        }
    }
}

//Функция delallc используется для удаления всех камер
void delallc(MonitoringData* data) {
    data->cameraCount = 0;
    printf("Все камеры удалены.\n");
}

//ункция checkcamID осуществляет поиск и вывод данных для камеры по заданному ID
void checkcamID(MonitoringData* data, int id) {
    int found = 0;

    for (int i = 0; i < data->cameraCount; i++) {
        if (data->cameras[i].id == id) {
            printf("Данные для камеры %d:\n", id);
            for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
                printf("Дата: %s, Время: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Камера с ID %d не найдена.\n", id);
    }
}

//Функция delcamID удаляет камеру с заданным ID из списка камер
void delcamID(MonitoringData* data, int id) {
    int found = 0;
    int indexToDelete = -1;

    for (int i = 0; i < data->cameraCount; i++) {
        if (data->cameras[i].id == id) {
            found = 1;
            indexToDelete = i;
            break;
        }
    }

    if (found) {
        for (int i = indexToDelete; i < data->cameraCount - 1; i++) {
            data->cameras[i] = data->cameras[i + 1];
        }

        data->cameraCount--;
        printf("Камера с ID %d была удалена.\n", id);
    }
    else {
        printf("Камера с ID %d не найдена.\n", id);
    }
}

//Функция rndmpoka выводит случайную прощальную фразу из подготовленного списка
void rndmpoka() {
    const char* goodbyes[] = {
        "Пока!", "До свидания!", "Увидимся!", "До встречи!", "Прощай!",
        "Всего доброго!", "Бывай!", "Пойду!", "Удачи!", "Спокойной ночи!",
        "Пока-пока!", "Береги себя!", "До скорого!",
        "До скорой встречи!", "Удачи тебе!", "Всего хорошего!", "На связи!",
        "Всего наилучшего!", "Прощай, друг!", "Пусть вам повезет!", "Не прощаемся!",
        "Скоро увидимся!", "Пока, до связи!", "Пока, удачи!", "Пока, до завтра!",
        "До новых встреч!", "Всего наилучшего!", "Чао!",
        "До завтра!", "Счастливо!", "Прощай, до встречи!", "Пока, до скорого!",
        "Увидимся позже!", "Желаю удачи!", "Береги себя!",
        "Будь здоров!", "До встречи в будущем!", "До следующего дня!",
        "Держись!", "Пусть тебе повезет!", "Жду тебя!",
        "Удачного дня!", "Прощай, до скорого!", "Гудбай!"
    };
    int numGoodbyes = sizeof(goodbyes) / sizeof(goodbyes[0]);  // определение размера массива
    srand(time(NULL));
    int randomIndex = rand() % numGoodbyes;
    printf("%s\n", goodbyes[randomIndex]);
}

//Функция privsv выводит сведения о программе
void privsv() {
    system("cls");
    printf("==============================================================================\n");
    printf("Добро пожаловать в программу мониторинга данных с камеры Camtrol!\n");
    printf("Версия: 1.0\n");
    printf("Разработчик: Ястребов Г.В. \n");
    printf("Группа: бИЦ-241 \n");
    printf("******************************************************************************\n");
    printf("Описание программы: Данная программа — умный страж данных,\n");
    printf("который с неослабевающим вниманием следит за снимками\n");
    printf("поступающими с камер. Она тщательно проверяет каждую строку данных,\n");
    printf("словно исследователь, стараясь найти любые изменения и обновить информацию.\n");
    printf("В режиме мониторинга она неустанно следит за изменениями\n");
    printf("в реальном времени и, как маг, генерирует новые записи,\n");
    printf("наполняя пространство данными. И если вдруг вам нужно удалить или\n");
    printf("найти что-то среди множества снимков, она без промедлений выполнит задачу. \n");
    printf("==============================================================================\n");

    printf("\nНажмите любую клавишу для продолжения...");
    _getch();
}

//Функция forcase1 позволяет пользователю ввести имя файла для мониторинга
void forcase1(MonitoringData* data) {
    printf("Введите имя файла: ");
    fgets(data->monitoringFile, sizeof(data->monitoringFile), stdin);
    data->monitoringFile[strcspn(data->monitoringFile, "\n")] = '\0';
    loadDataFromFile(data, data->monitoringFile);
}