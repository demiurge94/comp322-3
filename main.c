//
//  main.c
//  comp322-3
//
//  Created by Erick Vicencio on 3/23/21.
//

#include <stdio.h>
#include <stdlib.h>
/* Declare dyanmic arrays/vectorsand global variables */
int * resource = NULL;
int * available = NULL;
int * allocation = NULL;
int * need = NULL;
int * max_claim = NULL;
int num_processes;
int num_resources;
/*function prototypes*/
void printResourceUnitsVector(void);
void printMaxClaimArray(void);
void printAllocated(int process);
void printNeed(int process);
/*need th rest of the print functions*/
void enterParameters(void);
void bankers_algorithm(void);
void garbage_collection(void);
int main(int argc, const char * argv[]) {
    // insert code here...
    /* Declare local variables */
    int choice = 0;
    /* Until the user quits, print the menu, prompt for the menu choice, call the appropriate procedure */
    while( choice !=3 ){
        
        printf("\nBanker's Algorithm\n");
        printf("--------------------------------\n");
        printf("1) Enter Parameters\n2) Determine safe sequence \n3) Quit Program");
        printf("\nEnter Selection: ");
        scanf("%d", &choice);
        switch(choice){
            case 1: enterParameters(); break;
            case 2: bankers_algorithm(); break;
            case 3: garbage_collection(); break;
            default: printf("invalid choice\n");
        }
    }
    printf("Hello, World!\n");
    return 0;
}

void printResourceUnitsVector(){
    /* declare local variables*/
    int i;
    printf("Resources: \n");
    //for loop: print each resource index
    for(i = 0; i< num_resources; i++){
        printf("\tr%d", i);
    }
    printf("\n");
    //for loop pring the number of total unites of each resource
    for(i = 0; i< num_resources; i++){
        printf("\t%d", resource[i]);
    }
    printf("\n");
    return;
}
void printMaxClaimArray(void){
    int i, j;
    printf("Max Claim: \n");
    for(i = 0; i < num_resources; i++){
        printf("\tr%d", i );
        printf("\n");
        for(j = 0; j < num_processes; j++){
            printf("p%d\t", j);
            for(j = 0; j < num_resources; j++){
                printf("\t%d ", max_claim[i * num_resources +j]);
            }
        }
    }
}
void printAllocated(int process){
    int i, j;
    if(process!=-1){
        printf("testing");
    }
    else{
        printf("Allocated: \n");
        for(i = 0; i < num_resources; i++){
            printf("\tr%d", i);
            printf("\n");
            for(j = 0; j < num_processes; j++){
                printf("\t%d ", max_claim[i * num_resources + j]);
            }
        }
    }
    
}
void printNeed(int process){
    int j, i;
    if(process != -1 )
        for(j = 0; j < num_resources; j++){
            printf("%d ", need[process * num_resources + j]);
        }
    else{
        for(i = 0; i < num_resources; i++){
            printf("\tr%d", i);
            printf("\n");
            for(j = 0; j < num_processes; j++){
                printf("\t%d ", need[i * num_resources + j]);
            }
        }
    }
}

void enterParameters(){
    /* delcare local variables*/
    int i, j, units;
    /* prompt for number of process and number of resources */
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);
    printf("\nEnter the number of resources: ");
    scanf("%d", &num_resources);
    /* printf/scanf combination */
    /* allocate memory for vectors and arrays: resource, avaialbe, max claim, allocation, need */
    resource = (int * ) malloc(num_resources * sizeof(int));
    available = (int *) malloc(num_resources * sizeof(int));
    max_claim = (int * ) malloc(num_processes * num_resources * sizeof(int));
    allocation = (int *) malloc(num_processes * num_resources * sizeof(int));
    need = (int * ) malloc(num_processes * num_resources * sizeof(int));
    
    /* for each resource, prompt for number of units, set resource and avaialbe vector indices*/
    printf("Enter the number of units for resources r0 - r%d ", num_resources-1);
    for(i = 0; i < num_resources; i++){
        scanf("%d", &units);
        resource[i] = units;
        available[i] = units;
    }
    /* for each process, for each resource, prompt for maximum number of units requested by process, update max_claim and need arrays */
    for( i = 0; i < num_processes; i++){
        /*prompt message*/
        printf("Enter maximum number of units process p%d will request from each resource (r0 to r%d): ", i, num_resources-1);
        for(j = 0; j < num_resources; j++){
            /* scanf*/
            scanf("%d", &units);
            max_claim[i*num_resources+j] = units;
            need[i*num_resources+j] = units;
        }
    }
    /* for each process, for each resource, prompt for number of resource units allocated to process */
    for(i = 0; i < num_processes; i++){
        printf("Enter the number of units of each resource (r0 to r%d) allocated to process p%d", num_processes-1, i);
        for(j = 0; j < num_resources; j++){
            scanf("%d", &units);
            allocation[i*num_resources+j] = units;
            available[j] -= units;
            need[i*num_resources+j] -=units;
        }
    }
    
    /* print resource vector, available vector, max_claim array, allocated array, need array */
    printResourceUnitsVector();
    printMaxClaimArray();
    printAllocated(-1);
    printNeed(-1);
}
void bankers_algorithm(void){
    /* declare local variables, including vector in indicate if process is safely sequenced
     and "num_sequenced count */
    int num_sequenced = 0;
    int i, j, less_than;
    int * safe = (int * )calloc(num_processes, sizeof(int));
    
    /* While not all processes are sequenced */
    while(num_sequenced < num_processes){
        /*for each process */
        for(i = 0; i< num_processes; i++){
            less_than = 1;
            printf("Checking: ");
            /* if process hasn't been safely sequenced yet */
            if(safe[i] == 0){
                /* if the process has been sequenced it'll be a 1*/
                /* for each resource*/
                printf("< ");
                for(j = 0; j <num_resources; j++){
                    /* check for safe sequenceing by comparing process' need vector to available vector */
                    printf("%d ", need[i*num_resources +j ]);
                    less_than &= (need[i*num_resources+j]<available[j]);

                    
                }
                
                /* after done with for loop if each resource is available */
                /*update number of available units of resource*/
                if (less_than == 1){
                    /* process p[i] was safely sequenced printf */
                    /* print message that process has been safely sequenced*/
                    
                    for(int j = 0; j<num_resources; j++){
                        available[j] += allocation[i * num_resources + j];
                    /* free all the resources allocated to the process */
                    allocation[i*num_resources+j] = 0;
                    }/*for*/
                    num_sequenced++;

                }/* if lessthan==1*/
                printf("\n");
            } /*if safe = = 0 */
            
        }/* for-loop */
    }/*while*/
    return;
}
void garbage_collection(void){
    if(resource != NULL)
        free(resource);
    /*for all of them*/
}

