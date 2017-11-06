float R1 = 1e6;                     //Este resistor es el que junto con C1 determina la constante de tiempo
float C1 = 0;                       //Aquí guardaremos la capacidad medida.
volatile int led_pin = 12;         //terminal al que conectamos un led que encenderemos lo que dure el pulso de entrada.
volatile long t_inicio = 0;        //Guardamos el instante de inicio.
volatile long t_final = 0;         //Guardamos el instante del final del pulso.
long T = 0;                         //Duración del pulso

void setup()
{
    Serial.begin(9600);                 //Habilitamos el puerto serie
    attachInterrupt(0, start, RISING);  //El interrupt 0 está asociado al pin 2. RISING indica que la interrupcion
                                        //se dispara con el flanco ascendente del pulso.
    attachInterrupt(1, stop, FALLING);  //El interrupt 1 está asociado al pin 3. FALLING indica que la interrupcion
                                        //se dispara con el flanco descendiente del pulso.
    pinMode(led_pin, OUTPUT);           //Habilitamos el pin del led como salida.
}

void loop()
{
    //El loop se ejecutara indefinidamente hasta que la variable t_final
    //ya no sea 0, lo que ocurrira luego de la interrupcion de final de pulso.
    if (t_final > 0)
    {
        T = t_final - t_inicio;         //Determinamos el tiempo de duracion del pulso.
        C1 = T * 1000 / (1.1 * R1);     //Calculamos la capacidad desconocida (en microFaradios)
        //A traves del puerto serie enviamos la información con los resultados.
        Serial.print("t = ");
        Serial.print(T);
        Serial.println(" microSeg");
        Serial.print("C = ");
        Serial.print(C1 );
        Serial.println(" microF");
        Serial.print("C = ");
        Serial.print(C1 * 1000);        //Convertimos a nanoFaradios
        Serial.println(" nanoF");
        Serial.println();
        t_inicio = 0;                   //Reasignamos cero a las variables que guardan los
        t_final = 0;                    //tiempos inicial y final y asi habilitar una nueva medicion.
    }
}

// Funcion asociada al interrupt 0. Se ejecuta al iniciar el pulso.
void start()
{
    t_inicio = millis();                //millis me da el tiempo transcurrido desde que el programa inicio su ejecucion.
    digitalWrite(led_pin, HIGH);        //Encendemos el led como indicador de que la interrupcion fue activada.
}

// Funcion asociada al interrupt 1. Se ejecuta al finalizar el pulso.
void stop()
{
    t_final = millis();                 //Guardamos el tiempo retornado por millis().
    digitalWrite(led_pin, LOW);         //Apagamos el led indicando el final del pulso
}

