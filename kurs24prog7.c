//�����: �������� ����
//��������: ���� ���� �������� ���������� ������� ����������� ������ � ����� ���������������

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
        printf("����� ���������� � ����!\n\n");
        printf("1. ��������� ������ �� �����\n");
        printf("2. �������� ������ �������\n");
        printf("3. ��������� �������� ������\n");
        printf("4. ������� ��� ������ (c ������� ����������)\n");
        printf("5. ����� �����������\n");
        printf("6. �������� ������ ������ �� ID\n");
        printf("7. ������� ������ �� ID\n");
        printf("8. ������� ��� ������\n");
        printf("9. C������� � ���������\n");
        printf("10.�����\n\n");
        printf("�������� �����: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1: {
            system("cls");
            forcase1(&data);
            printf("\n������� ����� ������� ��� �����������...");
            _getch(); // �������� ������� �������
            break;
        }
        case 2: {
            char inputData[100];
            system("cls");
            printf("������� ������ ������ (������ DD.MM.YY_HH:MM:SS_ID): ");
            fgets(inputData, sizeof(inputData), stdin);
            inputData[strcspn(inputData, "\n")] = '\0';
            addCamera(&data, inputData);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 3:
            system("cls");
            generateTestData(&data);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        case 4:
            system("cls");
            palld(&data);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        case 5:
            system("cls");
            monitoring(&data);
            break;
        case 6: {
            system("cls");
            printf("������� ID ������ ��� ��������: ");
            int checkId;
            scanf("%d", &checkId);
            checkcamID(&data, checkId);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 7: {
            system("cls");
            printf("������� ID ������ ��� ��������: ");
            int deleteId;
            scanf("%d", &deleteId);
            delcamID(&data, deleteId);
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
        case 8:
            system("cls");
            delallc(&data);
            printf("\n������� ����� ������� ��� �����������...");
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
            printf("������������ �����.\n");
            printf("\n������� ����� ������� ��� �����������...");
            _getch();
            break;
        }
    }
    return 0;
}

//������� prData ��������� ������������ ���� � ������� "��.��.��"
int prData(const char* date) {
    int day, month, year;
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) != 3) {
        return 0; //������
    }
    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return 0; //�������� ����
    }
    return 1; //���� �����
}

//������� prTime ��������� ������������ ������� � ������� "��:��:��"
int prTime(const char* time) {
    int hour, minute, second;
    if (sscanf(time, "%d:%d:%d", &hour, &minute, &second) != 3) {
        return 0; //������
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; //�������� �����
    }
    return 1; //���������� �����
}

//������� addCamera ��������� ����� ������ ��� ����� ������ ��� ������������ ������
void addCamera(MonitoringData* data, const char* inputData) {
    int id;
    char date[11], time[9];
    if (sscanf(inputData, "%10[^_]_%8[^_]_%d", date, time, &id) != 3) {
        printf("������: ������ ������ ���� � ������� DD.MM.YY_HH:MM:SS_ID.\n");
        return;
    }

    if (!prData(date) || !prTime(time)) {
        printf("������: ������������ ���� ��� �����.\n");
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
                printf("������ %d �������� ������������� ���������� �������.\n", id);
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

//������� generateTestData ���������� �������� ������ ��� �����
void generateTestData(MonitoringData* data) {
    int numLines;
    printf("������� ���������� c���� (������� � ������ �����), ������� ����� �������������: ");
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

    printf("�������� ������ �������������.\n");
}


//������� loadDataFromFile ��������� ������ � ������� �� �����
void loadDataFromFile(MonitoringData* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("������ ��� �������� ����� %s.\n", filename);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        addCamera(data, line);
    }
    fclose(file);
    printf("������ �� ����� %s ���������.\n", filename);
}

//������� getFileSize ��������� ������ �����
long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb"); //�������� ���� � �������� ������
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file); //��������� ������ � ������� �����
    fclose(file);
    return size;
}

//������� reloadFileIfUpdated ���������, ���� �� ��������� � ����� � ������� ��������� ��������
void reloadFileIfUpdated(MonitoringData* data, const char* filename, long* lastSize) {
    long currentSize = getFileSize(filename);
    if (currentSize != *lastSize) {
        *lastSize = currentSize;
        printf("���������� ��������� � ����� %s. �������� ����� ������...\n", filename);
        loadDataFromFile(data, filename);
    }
}

//������� generateSingleSnapshot ���������� ���� ������ ������ ��� ������ � ���������� ����������,
// � ����� ��������� � � �������
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
    printf("����� ������ ������ �������������: %s\n", testData);
}

//������� sortCamerasByID ��������� ���������� ����� �� id
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

//������� sortCamerasByDate ��������� ���������� ������� �� ����
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

//��� ���������� �� �����������
int compareAscending(int a, int b) {
    return a - b;
}

//��� ���������� �� ��������
int compareDescending(int a, int b) {
    return b - a;
}

//������� palld ������� ������ � ������� � �� ������� � ������������ ����������
void palld(MonitoringData* data) {
    if (data->cameraCount == 0) {
        printf("��� ������ � �������.\n");
        return;
    }

    int sortOption;
    printf("\n�������� ������� ����������:\n");
    printf("1. �� ��������� (��� ����������, ��� ���������)\n");
    printf("2. �� ID (�����������)\n");
    printf("3. �� ID (��������)\n");
    printf("4. �� ���� ������ (������ �������)\n");
    printf("5. �� ���� ������ (����� �������)\n");
    printf("��� �����: ");
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
        printf("�������� �����. ������ �� �������������.\n");
        return;
    }

    printf("\n��� ������:\n");
    for (int i = 0; i < data->cameraCount; i++) {
        printf("������ %d:\n", data->cameras[i].id);
        for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
            printf("����: %s, �����: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}


/*
 * ������� pallda ������� ��� ������ � ������� � �� �������
 * ���� ��� ������ � �������, ��������� ��������� �� ����
 */
void pallda(MonitoringData* data) {
    if (data->cameraCount == 0) {
        printf("��� ������ � �������.\n");
        return;
    }
    printf("��� ������:\n");
    for (int i = 0; i < data->cameraCount; i++) {
        printf("������ %d:\n", data->cameras[i].id);
        for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
            printf("����: %s, �����: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
        }
        printf("\n");
    }
}

/*
 * ������� monitoring ��������� ����� �����������, � �������:
 * 1. ����������� ������� ����� ��� ����������� � ����������� ������
 * 2. ������ 5 ������ �����������, ��������� �� ������ �����
 *    - ���� ���� ��� �������, ���������� ������������ ������
 * 3. ������ 20 ������ ������������ ����� ������ (������)
 * 4. ��� ������ �� ������ ����������� ���������� ������ ����� �������
 */
void monitoring(MonitoringData* data) {
    if (strlen(data->monitoringFile) == 0) {
        printf("���� ��� ����������� �� ��������. ������� ��������� ���� ����� ����.\n");
        printf("������� ����� ������� ��� �������� � ����...\n");
        _getch();
        return;
    }

    long lastSize = getFileSize(data->monitoringFile);
    system("cls");
    printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");

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
            printf("����� �� ������ �����������.\n");
            break;
        }

        if (elapsed5 == 5) {
            elapsed5 = 0;
            reloadFileIfUpdated(data, data->monitoringFile, &lastSize);
            system("cls");
            printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");
            pallda(data);
        }

        if (elapsed20 == 20) {
            elapsed20 = 0;
            generateSingleSnapshot(data);
            system("cls");
            printf("����� ����������� �������. ������� ����� ������� ��� ������.\n");
            pallda(data);
        }
    }
}

//������� delallc ������������ ��� �������� ���� �����
void delallc(MonitoringData* data) {
    data->cameraCount = 0;
    printf("��� ������ �������.\n");
}

//������ checkcamID ������������ ����� � ����� ������ ��� ������ �� ��������� ID
void checkcamID(MonitoringData* data, int id) {
    int found = 0;

    for (int i = 0; i < data->cameraCount; i++) {
        if (data->cameras[i].id == id) {
            printf("������ ��� ������ %d:\n", id);
            for (int j = 0; j < data->cameras[i].snapshotCount; j++) {
                printf("����: %s, �����: %s\n", data->cameras[i].snapshots[j].date, data->cameras[i].snapshots[j].time);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("������ � ID %d �� �������.\n", id);
    }
}

//������� delcamID ������� ������ � �������� ID �� ������ �����
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
        printf("������ � ID %d ���� �������.\n", id);
    }
    else {
        printf("������ � ID %d �� �������.\n", id);
    }
}

//������� rndmpoka ������� ��������� ���������� ����� �� ��������������� ������
void rndmpoka() {
    const char* goodbyes[] = {
        "����!", "�� ��������!", "��������!", "�� �������!", "������!",
        "����� �������!", "�����!", "�����!", "�����!", "��������� ����!",
        "����-����!", "������ ����!", "�� �������!",
        "�� ������ �������!", "����� ����!", "����� ��������!", "�� �����!",
        "����� ����������!", "������, ����!", "����� ��� �������!", "�� ���������!",
        "����� ��������!", "����, �� �����!", "����, �����!", "����, �� ������!",
        "�� ����� ������!", "����� ����������!", "���!",
        "�� ������!", "���������!", "������, �� �������!", "����, �� �������!",
        "�������� �����!", "����� �����!", "������ ����!",
        "���� ������!", "�� ������� � �������!", "�� ���������� ���!",
        "�������!", "����� ���� �������!", "��� ����!",
        "�������� ���!", "������, �� �������!", "������!"
    };
    int numGoodbyes = sizeof(goodbyes) / sizeof(goodbyes[0]);  // ����������� ������� �������
    srand(time(NULL));
    int randomIndex = rand() % numGoodbyes;
    printf("%s\n", goodbyes[randomIndex]);
}

//������� privsv ������� �������� � ���������
void privsv() {
    system("cls");
    printf("==============================================================================\n");
    printf("����� ���������� � ��������� ����������� ������ � ������ Camtrol!\n");
    printf("������: 1.0\n");
    printf("�����������: �������� �.�. \n");
    printf("������: ���-241 \n");
    printf("******************************************************************************\n");
    printf("�������� ���������: ������ ��������� � ����� ����� ������,\n");
    printf("������� � �������������� ��������� ������ �� ��������\n");
    printf("������������ � �����. ��� ��������� ��������� ������ ������ ������,\n");
    printf("������ �������������, �������� ����� ����� ��������� � �������� ����������.\n");
    printf("� ������ ����������� ��� ��������� ������ �� �����������\n");
    printf("� �������� ������� �, ��� ���, ���������� ����� ������,\n");
    printf("�������� ������������ �������. � ���� ����� ��� ����� ������� ���\n");
    printf("����� ���-�� ����� ��������� �������, ��� ��� ����������� �������� ������. \n");
    printf("==============================================================================\n");

    printf("\n������� ����� ������� ��� �����������...");
    _getch();
}

//������� forcase1 ��������� ������������ ������ ��� ����� ��� �����������
void forcase1(MonitoringData* data) {
    printf("������� ��� �����: ");
    fgets(data->monitoringFile, sizeof(data->monitoringFile), stdin);
    data->monitoringFile[strcspn(data->monitoringFile, "\n")] = '\0';
    loadDataFromFile(data, data->monitoringFile);
}