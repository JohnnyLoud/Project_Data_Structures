//
//  johnnylouds.c
//  Project_Tsakalidis
//
//  Created by Ioannis Loudaros on 5/6/20.
//  Copyright © 2020 Ιωάννης Λουδάρος. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>





//################ Structs #####################

//Χρόνος, για την εύκολη διαχείρηση των timestamps, ενδιαφέρον έχει το int together, σκοπός του είναι να κάνει ευκολότερο το sorting
typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    long int together;
} timej;



//Τύποι δεδομένων για την καταγραφή των μετρήσεων. Ένας από τους δύο θα μπορούσε να παραληφθεί αλλά χρησιμοποιούνται για λόγους ευαναγνωσιμότητας
typedef struct{
    float temp;
    timej timestamp;
} measuret;

typedef struct{
    int hum;
    timej timestamp;
} measureh;


//Τύποι δεδομένων για την διατήρηση πολλών μετρήσεων των παραπάνω τύπων
typedef struct{
    measureh* measurement;
    int size;
} logh;

typedef struct{
    measuret* measurement;
    int size;
} logt;







//############# Οι Parsers #################


//Φορτώνει δεδομένα μετρήσεων υγρασίας
void loadh(logh* log){
   
    //Άνοιγμα αρχείου
    FILE *fp;
    printf("Opening \"hum.txt\"...\n");
    fp = fopen("hum.txt", "r");
    
    //Έλεγχος αρχείου
    if (fp==NULL) printf("Something is wrong with this file\n");
        else printf("Opened\n");
        
    
    
    //Προετοιμασία ανάγνωσης πρώτης μέτρησης
    printf("Creating a new entry\n");
    char c = '\0' ;
    log->size=1;
    printf("Loading Data\n");
    
    
    while (c!=EOF)
    {
        
        
     
        
        //Κάνε χώρο μέσα στον πίνακα ώστε να μπει η επόμενη μέτρηση
        (*log).measurement=(measureh *)realloc((*log).measurement, (*log).size*sizeof(measureh));
        
       //Φτάσε μέχρι την αρχή της εγγραφής (Αν τελειώσει το αρχείο βγες από το loop)
        while (c!='"')
        {
                c=getc(fp);
                if (c==EOF)
                {
                    log->size--;
                    (*log).measurement=(measureh *)realloc((*log).measurement, (*log).size*sizeof(measureh));
                    goto done;
                    
                }
        }
           if(log->size % 2000==0) printf("#");
        
        //Διάβασε την ημερομηνία
    fscanf(fp,"%4d-%2d-%2dT%2d:%2d:%2d",&log->measurement[log->size-1].timestamp.year,&log->measurement[log->size-1].timestamp.month,&log->measurement[log->size-1].timestamp.day,&log->measurement[log->size-1].timestamp.hour,&log->measurement[log->size-1].timestamp.minute,&log->measurement[log->size-1].timestamp.sec);
        
        log->measurement[log->size-1].timestamp.together=(long)100000000* (long)log->measurement[log->size-1].timestamp.year+(long)1000000*log->measurement[log->size-1].timestamp.month+(long)10000*log->measurement[log->size-1].timestamp.day+100*log->measurement[log->size-1].timestamp.hour+log->measurement[log->size-1].timestamp.minute;
        
        //Πήγαινε στην μέτρηση
    c=getc(fp);
        c='0';
    while (c!='"') c=getc(fp);
        
        //Διάβασε την μέτρηση
    fscanf(fp, "%2d",&log->measurement[log->size-1].hum);
    log->size++;
    c=getc(fp);
     c='0';
    }
    
done:

    fclose(fp);
    printf("Done after %d measurements\n",log->size);
    
    
    return;
    
    
}


//Φορτώνει δεδομένα μετρήσεων θερμοκρασίας
void loadt(logt* log){
   
    //Άνοιγμα αρχείου
    FILE *fp;
    printf("Opening \"tempm.txt\"...\n");
    fp = fopen("tempm.txt", "r");
    
    //Έλεγχος αρχείου
    if (fp==NULL) printf("Something is wrong with this file\n");
        else printf("Opened\n");
        
    
    
    //Προετοιμασία ανάγνωσης πρώτης μέτρησης
    printf("Creating a new entry\n");
    char c = '\0' ;
    log->size=1;
    printf("Loading Data\n");
    
    
    while (c!=EOF)
    {
        
        
     
        
        //Κάνε χώρο μέσα στον πίνακα ώστε να μπει η επόμενη μέτρηση
        log->measurement=(measuret *)realloc((*log).measurement, log->size*sizeof(measuret));
        
        
       //Φτάσε μέχρι την αρχή της εγγραφής (Αν τελειώσει το αρχείο βγες από το loop φρόντισε να ελευθερώσεις τον επιπλέον χώρο που δέσμευσες στην προηγούμενη εντολή )
        while (c!='"')
        {
                c=getc(fp);
                if (c==EOF)
                {
                    log->size--;
                    log->measurement=(measuret *)realloc((*log).measurement, log->size*sizeof(measuret));
                    goto done;
                    
                }
        }
           if(log->size % 2000==0) printf("#");
        
        //Διάβασε την ημερομηνία
    fscanf(fp,"%4d-%2d-%2dT%2d:%2d:%2d",&log->measurement[log->size-1].timestamp.year,&log->measurement[log->size-1].timestamp.month,&log->measurement[log->size-1].timestamp.day,&log->measurement[log->size-1].timestamp.hour,&log->measurement[log->size-1].timestamp.minute,&log->measurement[log->size-1].timestamp.sec);
        
        //Δημιουργία του "together"
        log->measurement[log->size-1].timestamp.together=(long)100000000* (long)log->measurement[log->size-1].timestamp.year+(long)1000000*log->measurement[log->size-1].timestamp.month+(long)10000*log->measurement[log->size-1].timestamp.day+100*log->measurement[log->size-1].timestamp.hour+log->measurement[log->size-1].timestamp.minute;
        
        //Πήγαινε στην μέτρηση
    c=getc(fp);
        c='0';
    while (c!='"') c=getc(fp);
        
        //Διάβασε την μέτρηση
    fscanf(fp, "%f\"",&log->measurement[log->size-1].temp);
    log->size++;
    c=getc(fp);
     c='0';
    }
    
done:

    fclose(fp);
    printf("Done after %d measurements\n",log->size);
    
    
    return;
    
    
}






//################ Takedays() #####################

//Οι παρακάτω συναρτήσεις παίρνουν έναν πίνακα με μετρήσεις και έναν άλλο πίνακα τον οποίον γεμίζουν με τις μέσες μετρήσεις κάθε ημέρας του προηγούμενου πίνακα


void takedayst(logt log,logt* result){
    
    int i;
    float temp = log.measurement[0].temp;
    int measurements = 1;
    result->size=0;
    
    for (i=1;i<log.size;i++)
    {
        //Αν ακόμη βρισκόμαστε στην ίδια μέρα
        if (log.measurement[i].timestamp.day==log.measurement[i-1].timestamp.day)
            {
                measurements++;
                temp+=log.measurement[i].temp;
            }
        else
            {
                //κάνε χώρο στον πίνακα των αποτελεσμάτων για άλλη μια μέρα
                result->size++;
                result->measurement=(measuret *)realloc(result->measurement, result->size*sizeof(measuret));
                result->measurement[result->size-1].temp=temp/measurements;
                result->measurement[result->size-1].timestamp.day=log.measurement[i-1].timestamp.day;
                result->measurement[result->size-1].timestamp.year=log.measurement[i-1].timestamp.year;
                result->measurement[result->size-1].timestamp.month=log.measurement[i-1].timestamp.month;
                result->measurement[result->size-1].timestamp.hour=0;
                result->measurement[result->size-1].timestamp.minute=0;
                result->measurement[result->size-1].timestamp.sec=0;
                result->measurement[result->size-1].timestamp.together=(long)100000000* (long)result->measurement[result->size-1].timestamp.year+(long)1000000*result->measurement[result->size-1].timestamp.month+(long)10000*result->measurement[result->size-1].timestamp.day+100*result->measurement[result->size-1].timestamp.hour+result->measurement[result->size-1].timestamp.minute;
                
                temp=log.measurement[i].temp;
                measurements=1;
                
            }
        
    }
    
    //Βάλε την τελευταία μέρα
   result->size++;
   result->measurement=(measuret *)realloc((*result).measurement, result->size*sizeof(measuret));
   result->measurement[result->size-1].temp=temp/measurements;
   result->measurement[result->size-1].timestamp.day=log.measurement[i-1].timestamp.day;
   result->measurement[result->size-1].timestamp.year=log.measurement[i-1].timestamp.year;
   result->measurement[result->size-1].timestamp.month=log.measurement[i-1].timestamp.month;
   result->measurement[result->size-1].timestamp.hour=0;
   result->measurement[result->size-1].timestamp.minute=0;
   result->measurement[result->size-1].timestamp.sec=0;
    
    
    printf("Done after %d measurements\n",result->size);
   
    
}







void takedaysh(logh log,logh* result){
    
    int i;
    int temp=log.measurement[0].hum;
    int measurements=1;
    
    for (i=1;i<log.size;i++)
    {
        //Αν ακόμη βρισκόμαστε στην ίδια μέρα
        if (log.measurement[i].timestamp.day==log.measurement[i-1].timestamp.day)
            {
                measurements++;
                temp+=log.measurement[i].hum;
            }
        else
            {
                //κάνε χώρο στον πίνακα των αποτελεσμάτων για άλλη μια μέρα
                result->size++;
                result->measurement=(measureh *)realloc((*result).measurement, result->size*sizeof(measureh));
                result->measurement[result->size-1].hum=temp/measurements;
                result->measurement[result->size-1].timestamp.day=log.measurement[i-1].timestamp.day;
                result->measurement[result->size-1].timestamp.year=log.measurement[i-1].timestamp.year;
                result->measurement[result->size-1].timestamp.month=log.measurement[i-1].timestamp.month;
                result->measurement[result->size-1].timestamp.hour=0;
                result->measurement[result->size-1].timestamp.minute=0;
                result->measurement[result->size-1].timestamp.sec=0;
                
                
                temp=log.measurement[i].hum;
                measurements=1;
                
            }
        
    }
    
    //Βάλε την τελευταία μέρα
   result->size++;
   result->measurement=(measureh *)realloc((*result).measurement, result->size*sizeof(measureh));
   result->measurement[result->size-1].hum=temp/measurements;
   result->measurement[result->size-1].timestamp.day=log.measurement[i-1].timestamp.day;
   result->measurement[result->size-1].timestamp.year=log.measurement[i-1].timestamp.year;
   result->measurement[result->size-1].timestamp.month=log.measurement[i-1].timestamp.month;
   result->measurement[result->size-1].timestamp.hour=0;
   result->measurement[result->size-1].timestamp.minute=0;
   result->measurement[result->size-1].timestamp.sec=0;
    
    
    printf("Done after %d measurements\n",result->size);
   
    
}







//################ Printers #####################




//Οι παρακάτω συναρτήσεις διευκολύνουν την εκτύπωση των logs


void printlogh(logh log){
    
    int i;
    for(i=0;i<log.size;i++) printf(" %02d/%02d/%04d \t %02d:%02d:%02d \t Measurement: %d\n",log.measurement[i].timestamp.day,log.measurement[i].timestamp.month,log.measurement[i].timestamp.year,log.measurement[i].timestamp.hour,log.measurement[i].timestamp.minute,log.measurement[i].timestamp.sec,log.measurement[i].hum);
}

void printlogt(logt log){
    
    int i;
    for(i=0;i<log.size;i++) printf(" %02d/%02d/%04d \t %02d:%02d:%02d \t Measurement: %02.02f\n",log.measurement[i].timestamp.day,log.measurement[i].timestamp.month,log.measurement[i].timestamp.year,log.measurement[i].timestamp.hour,log.measurement[i].timestamp.minute,log.measurement[i].timestamp.sec,log.measurement[i].temp);
}




//################ Βοηθητικές Συναρτήσεις #####################



void passlogh(logh* a,logh* b){
   
    b->size = a->size;
    b->measurement = (measureh *)realloc(b->measurement, b->size*sizeof(measureh));
    int i;
    for (i = 0; i < b->size; i++) {
        b->measurement[i].hum=a->measurement[i].hum;
        b->measurement[i].timestamp.year = a->measurement[i].timestamp.year;
        b->measurement[i].timestamp.month = a->measurement[i].timestamp.month;
        b->measurement[i].timestamp.day = a->measurement[i].timestamp.day;
        b->measurement[i].timestamp.hour = a->measurement[i].timestamp.hour;
        b->measurement[i].timestamp.minute = a->measurement[i].timestamp.minute;
        b->measurement[i].timestamp.sec = a->measurement[i].timestamp.sec;
        b->measurement[i].timestamp.together = a->measurement[i].timestamp.together;
        
    }
    
    
}


void passlogt(logt* a,logt* b){
   
    b->size = a->size;
    b->measurement=(measuret *)malloc(b->size*sizeof(measuret));
    
    for (int i=0; i<b->size; i++) {
        b->measurement[i].temp = (float)a->measurement[i].temp;
        b->measurement[i].timestamp.year = a->measurement[i].timestamp.year;
        b->measurement[i].timestamp.month = a->measurement[i].timestamp.month;
        b->measurement[i].timestamp.day = a->measurement[i].timestamp.day;
        b->measurement[i].timestamp.hour = a->measurement[i].timestamp.hour;
        b->measurement[i].timestamp.minute = a->measurement[i].timestamp.minute;
        b->measurement[i].timestamp.sec = a->measurement[i].timestamp.sec;
        b->measurement[i].timestamp.together = a->measurement[i].timestamp.together;
        
    }
    
    
}


void ask4date(timej* t){
    printf("Σε ποια στιγμή είναι η μέτρηση που ψάχνεις;\n");//Ο χρήστης δίνει τα στοιχεία της αναζήτησης
    t->year=2014;
    t->sec=00;
    printf("Μήνας:");
    scanf("%d",&(t->month));
    printf("Μέρα:");
    scanf("%d",&(t->day));
    printf("Ώρα:");
    scanf("%d",&(t->hour));
    printf("Λεπτό:");
    scanf("%d",&(t->minute));
    int dump = getchar();
    t->together=(long)100000000* (long)t->year+(long)1000000*t->month+(long)10000*t->day+100*t->hour+t->minute;
}

void ask4day(timej* t){
    printf("Σε ποια στιγμή είναι η μέτρηση που ψάχνεις;\n");//Ο χρήστης δίνει τα στοιχεία της αναζήτησης
    t->year=2014;
    t->sec=00;
    t->hour=00;
    t->minute=00;
    printf("Μήνας:");
    scanf("%d",&(t->month));
    printf("Μέρα:");
    scanf("%d",&(t->day));
    int dump = getchar();
    t->together=(long)100000000* (long)t->year+(long)1000000*t->month+(long)10000*t->day+100*t->hour+t->minute;
}

//############ Swaps ##############

void swaph(measureh *a, measureh *b)
{
    measureh temp;
    temp.hum = a->hum;
    temp.timestamp.year = a->timestamp.year;
    temp.timestamp.month = a->timestamp.month;
    temp.timestamp.day = a->timestamp.day;
    temp.timestamp.minute = a->timestamp.minute;
    temp.timestamp.hour = a->timestamp.hour;
    temp.timestamp.sec = a->timestamp.sec;
    temp.timestamp.together = a->timestamp.together;
    
    a->hum = b->hum;
    a->timestamp.year = b->timestamp.year;
    a->timestamp.month = b->timestamp.month;
    a->timestamp.day = b->timestamp.day;
    a->timestamp.minute = b->timestamp.minute;
    a->timestamp.hour = b->timestamp.hour;
    a->timestamp.sec = b->timestamp.sec;
    a->timestamp.together = b->timestamp.together;
    
    b->hum = temp.hum;
    b->timestamp.year = temp.timestamp.year;
    b->timestamp.month = temp.timestamp.month;
    b->timestamp.day = temp.timestamp.day;
    b->timestamp.minute = temp.timestamp.minute;
    b->timestamp.hour = temp.timestamp.hour;
    b->timestamp.sec = temp.timestamp.sec;
    b->timestamp.together = temp.timestamp.together;
    
    
}

void swapt(measuret *a, measuret *b)
{
    measuret temp;
    temp.temp = a->temp;
    temp.timestamp.year = a->timestamp.year;
    temp.timestamp.month = a->timestamp.month;
    temp.timestamp.day = a->timestamp.day;
    temp.timestamp.minute = a->timestamp.minute;
    temp.timestamp.hour = a->timestamp.hour;
    temp.timestamp.sec = a->timestamp.sec;
    temp.timestamp.together = a->timestamp.together;
    
    
    
    a->temp = b->temp;
    a->timestamp.year = b->timestamp.year;
    a->timestamp.month = b->timestamp.month;
    a->timestamp.day = b->timestamp.day;
    a->timestamp.minute = b->timestamp.minute;
    a->timestamp.hour = b->timestamp.hour;
    a->timestamp.sec = b->timestamp.sec;
    a->timestamp.together = b->timestamp.together;
    
    b->temp = temp.temp;
    b->timestamp.year = temp.timestamp.year;
    b->timestamp.month = temp.timestamp.month;
    b->timestamp.day = temp.timestamp.day;
    b->timestamp.minute = temp.timestamp.minute;
    b->timestamp.hour = temp.timestamp.hour;
    b->timestamp.sec = temp.timestamp.sec;
    b->timestamp.together = temp.timestamp.together;
    
    
}


void passmeasurementt(measuret *a, measuret *b)
{
    
    b->temp = a->temp;
    b->timestamp.year = a->timestamp.year;
    b->timestamp.month = a->timestamp.month;
    b->timestamp.day = a->timestamp.day;
    b->timestamp.minute = a->timestamp.minute;
    b->timestamp.hour = a->timestamp.hour;
    b->timestamp.sec = a->timestamp.sec;
    b->timestamp.together = a->timestamp.together;
    
}

void passmeasurementh(measureh *a, measureh *b)
{
    
    b->hum = a->hum;
    b->timestamp.year = a->timestamp.year;
    b->timestamp.month = a->timestamp.month;
    b->timestamp.day = a->timestamp.day;
    b->timestamp.minute = a->timestamp.minute;
    b->timestamp.hour = a->timestamp.hour;
    b->timestamp.sec = a->timestamp.sec;
    b->timestamp.together = a->timestamp.together;
    
}




