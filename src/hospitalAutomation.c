#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_STRING_LENGTH  10

struct patient
{
    char* name;
    int needed_staff;
};

typedef struct patient patient;

struct staff
{
    int min_staff;
    int max_staff;
    patient* patients;
    int patient_count;
};

typedef struct staff staff;

struct zone
{
    char* id;
    staff* staffs;
    int staff_count;
};

typedef struct zone zone;

void print_all_patients(zone z) {
    printf("%s:\n", z.id);

    for(int i = 0; i < z.staff_count; i++){
        printf("\t%d:\n", i);
        for(int j = 0; j < z.staffs[i].patient_count; j++){
            printf("\t\t%s\n", z.staffs[i].patients[j].name);
        }
    }
}

void send_staff_to_zones(zone* source, int source_staff_index, zone* target, int target_staff_index) {
    int topStafftoSend = source->staffs[source_staff_index].patient_count;
    int minStaff = target->staffs[target_staff_index].min_staff;
    int maxStaff = target->staffs[target_staff_index].max_staff;

    for(int i = 0; i < topStafftoSend; i++){
        if(source->staffs[source_staff_index].patients[i].needed_staff <= maxStaff && source->staffs[source_staff_index].patients[i].needed_staff >= minStaff){
            int nPatients_target = ++target->staffs[target_staff_index].patient_count;
            source->staffs[target_staff_index].patient_count--;

            target->staffs[target_staff_index].patients = realloc(target->staffs[target_staff_index].patients, sizeof(staff) * topStafftoSend);

            target->staffs[target_staff_index].patients[nPatients_target - 1] =         source->staffs[source_staff_index].patients[i];

            patient fill;
            
            fill.name = "XXX";

            source->staffs[source_staff_index].patients[i] = fill;

        }

    }

    for(int i = 0; i < topStafftoSend; i++){
        if(strcmp(source->staffs[source_staff_index].patients[i].name, "XXX") == 0){
            for(int j = i; j < topStafftoSend - 1; j++){
                source->staffs[source_staff_index].patients[j] = source->staffs[source_staff_index].patients[j + 1];
            }
        }
    }

    source->staffs[source_staff_index].patients = realloc(source->staffs[source_staff_index].patients, sizeof(patient) * source->staffs[source_staff_index].patient_count);

}

zone zone_with_most_patients(zone* zones, int zones_count){
    int max_patients = 0, mpatient_zone;


    for(int i = 0; i < zones_count; i++){
        int sum = 0;

        for(int j = 0; j < zones[i].staff_count; j++){
            sum += zones[i].staffs[j].patient_count;
        }

        if(sum > max_patients){
            max_patients = sum;
            mpatient_zone = i;
        }
    }

    return zones[mpatient_zone];

}

zone find_town(zone* zones, int zones_count, char* id) {
    int i;
    for(i = 0; i < zones_count; i++){
        if(strcmp(zones[i].id, id) == 0)
            break;
    }

    return zones[i];
}

int main()
{
    int zones_count;
    scanf("%d", &zones_count);
    zone* zones = malloc(sizeof(zone)*zones_count);
    for (int i = 0; i < zones_count; i++) {
        zones[i].id = malloc(sizeof(char) * MAX_STRING_LENGTH);
        scanf("%s", zones[i].id);
        scanf("%d", &zones[i].staff_count);
        zones[i].staffs = malloc(sizeof(staff)*zones[i].staff_count);
        for (int j = 0; j < zones[i].staff_count; j++) {
            scanf("%d%d%d", &zones[i].staffs[j].patient_count, &zones[i].staffs[j].min_staff, &zones[i].staffs[j].max_staff);
            zones[i].staffs[j].patients = malloc(sizeof(patient)*zones[i].staffs[j].patient_count);
            for (int k = 0; k < zones[i].staffs[j].patient_count; k++) {
                zones[i].staffs[j].patients[k].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
                scanf("%s", zones[i].staffs[j].patients[k].name);
                scanf("%d", &zones[i].staffs[j].patients[k].needed_staff);
            }
        }
    }
    int operations;
    scanf("%d", &operations);
    char zone_name[MAX_STRING_LENGTH];
    while (operations--) {
        int operation_type;
        scanf("%d", &operation_type);
        switch (operation_type) {
        case 1:
            scanf("%s", zone_name);
            zone zn = find_town(zones, zones_count, zone_name);
            print_all_patients(zn);
            break;
        case 2:
            scanf("%s", zone_name);
            zone source = find_town(zones, zones_count, zone_name);
            int source_index;
            scanf("%d", &source_index);
            scanf("%s", zone_name);
            zone target = find_town(zones, zones_count, zone_name);
            int target_index;
            scanf("%d", &target_index);
            send_staff_to_zones(&source, source_index, &target, target_index);
            break;
        case 3:
            printf("Zone with the most number of patients is %s\n", zone_with_most_patients(zones, zones_count).id);
            break;
        }
    }
    free zones;
    free staffs;
    free patients;
    return 0;
}
