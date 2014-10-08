// Definizione dei pin utilizzati dai sensori
#define sensore1 A0                           // Definisce il pin del sensore1
#define sensore2 A1                           // Definisce il pin del sensore2

// Definizione dei pin utilizzati dai motori
#define motore1 3                             // Definisce il pin PWM a cui il motore1 è collegato
#define motore2 5                             // Definisce il pin PWM a cui il motore2 è collegato

// Variabile per le distanze anteriore e posteriore
int distanzas1 = 0;                           // Valore letto dal sensore1
int distanzas2 = 0;                           // Valore letto dal sensore2

// Contatori d'appoggio 
int C;                                        // Contatore d'appoggio
int J;                                        // Contatore d'appoggio

// Variabili per il filtraggio delle misure
int arrays1[10];                              // Array di appoggio per la rilevazione della distanza1
int arrays2[10];                              // Array di appoggio per la rilevazione della distanza2
int n = 20;                                   // Numero di volte che si deve effettuare la misura, per il calcolo della media 
int somma = 0;                                // Variabile di appoggio per il calcolo della media
int appoggio;                                 // Variabie di appoggio per l'ordinamento degli array

// Variabili per il calcolo dell'orientamento
int orientamento = 0;                         // Inclinazione dell'AutoBalance, in funzione delle distanze rilevate
int previousorientamento = 0;

// Variabili temporali, per il calcolo del PID
float time = 0;                               // Tempo auttuale, dall'accensione del dispositivo
float previousTime = 0;                       // Tempo del precedente ciclo
float interval = 0;                           // Tempo impiegato per l'esecuzione di un ciclo di programma

// Variabili PID
float P = 0;                                  // Variabile PROPORZIONALE  del PID
float I = 0;                                  // Variabile INTEGRATIVA del PID
float D = 0;                                  // Variabile DERIVATIVA del PID
float PID = 0;                                // Parametro PID

// Guadagni dei coefficenti PID
float kP = 12;                                // Variabile PID PROPORZIONALE
float kI = 430;                               // Variabile PID INTEGRATIVA
float kD = 20;                                // Variabile PID DERIVATRIVA

void setup() 
{
    Serial.begin( 9600 );                     // Avvia la comunicazzione seriale
    pinMode( motore1, OUTPUT );               // Imposta il pin del motore1 come uscita
    pinMode( motore2, OUTPUT );               // Imposta il pin del motore2 come uscita 
}

void loop()                                   //  In Debug
{  
  controllomotori( getPID() );
  
  Serial.print( "Distanza S1 =  " );    
  Serial.print(  analogRead( sensore1 ));                       
  Serial.print( "  DS1 Filtrata =  " );    
  Serial.print( distanzas1 );  
  Serial.print( "  Distanza S2 =  " );    
  Serial.print(  analogRead( sensore2 ));                       
  Serial.print( "  DS2 Filtrata =  " );    
  Serial.print( distanzas2 ); 
  Serial.print( "  Orientamento =  " );    
  Serial.print( orientamento ); 
  Serial.print( "  P =  " );    
  Serial.print( P ); 
  Serial.print( "  I =  " );    
  Serial.print( I ); 
  Serial.print( "  D =  " );    
  Serial.print( D ); 
  Serial.print( "  PID =  " );    
  Serial.print( PID ); 
  Serial.print( "  To Motori =  " );
  Serial.println( ( 255 / 2 ) + PID );
  
  previousorientamento = orientamento;         // Orientamento al ciclo precedente   
}

int getdistanzas1()                              // Algoritmo per l'acquisizione della distanza s1
{
  for ( C = 1; C <= n; C++ )                     // Legge 20 valori dal sensore1, e li trascrive in un array
  {
    arrays1[C] = analogRead( sensore1 );
  }  
    
  for ( C = 1; C <= n; C++ )                     // Ordina l'array in modo crescente
  {
    for ( J = (C + 1); J <= n; J++ )
    {
      if ( arrays1[C] > arrays1[J] )
      {
        appoggio = arrays1[C];
       	arrays1[C] = arrays1[J];
	arrays1[J] = appoggio;
      }
    }
  }
  
  somma = 0;
  
  for ( C = 6; C <= ( n - 5 ); C++ )             // Non considera gli estremi
  {
    somma = somma + arrays1[C];
  }
  
  distanzas1 = somma / ( n - 10 );               // Effettua la media dei valori
  
  return( distanzas1 );                          // Ritorna il valore distanzas2 oppurtunamente filtrato
}   




int getdistanzas2()                              // Algoritmo per l'acquisizione della distanza s2
{
  for ( C = 1; C <= n; C++ )                     // Legge 20 valori dal sensore2, e li trascrive in un array
  {
    arrays2[C] = analogRead( sensore2 );
  }  
    
  for ( C = 1; C <= n; C++ )                     // Ordina l'array in modo crescente
  {
    for ( J = (C + 1); J <= n; J++ )
    {
      if ( arrays2[C] > arrays2[J] )
      {
        appoggio = arrays2[C];
       	arrays2[C] = arrays2[J];
	arrays2[J] = appoggio;
      }
    }
  }
  
  somma = 0;
  
  for ( C = 6; C <= ( n - 5 ); C++ )             // Non considera gli estremi
  {
    somma = somma + arrays2[C];
  }
  
  distanzas2 = somma / ( n - 10 );               // Effettua la media dei valori
  
  return( distanzas2 );                          // Ritorna il valore distanzas2 oppurtunamente filtrato
}


int getPID()                                                      // Algoritmo per il calcolo del PID
{
  getorientation();                                               // Richiama la funzione per ottenere l'orientamento
  
  previousTime = time;                                            // Salva il tempo al ciclo precedente
  time = millis();
  interval = time - previousTime;                           // Calcola il tempo impiegato per eseguire un ciclo
  
  P = orientamento * kP;                                          // Calcola la variabile proporzionale 
  I = I + interval * kI * P;                                      // Calcola la variabile integrativa
  D = kD * ( orientamento - previousorientamento ) / interval;    // Calcola la variabile derivativa
  
  PID = P + I + D;                                                // Calcola il PID
  
  if( PID >= 255/2 ) PID = 255/2;                                 // Se il valore supera 255/2, il valore è impostato a 255/2   
  if( PID <= -255/2 ) PID = -255/2;                               // Se il valore è minore di -255/2, il valore è impostato a -255/2   
  
  if(PID <= 1 && PID > 0) PID = 0;                                // Approssima il PID a 0
  if(PID >= -1 && PID < 0) PID = 0;                               // Approssima il PID a 0
  
  return( PID );                                                  // Ritorna il valore del PID
}

void getorientation()                                             // Algoritmo per il calcolo dell'orientamento
{
  orientamento = getdistanzas1() - getdistanzas2();               // Differenza fra le distanze rilevate dai due sensori
}


void controllomotori ( int PID )                      // Algoritmo per il controlla i due motori DC
{
  analogWrite( motore1, ( 255 / 2 ) + PID );          // Controlla il motore1 in base al PID
  analogWrite( motore2, ( 255 / 2 ) + PID );          // Controlla il motore2 in base al PID
}
