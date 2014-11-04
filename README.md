elo330_tarea3_Acevedo_Tampier
=============================

ELO330 Tarea 3: Emunador de Variaciones de Retardo y Pérdidas en Transferencias UDP

Compilación:
    $ make
    
Uso:
    ./erp_udp retardo_promedio(usecs) variación_retardo(usecs) porcentaje_pérdida(0-1) puerto_local [host_remoto] puerto_remoto
    
Descripción:
    Este programa funciona como intermediario entre un cliente y un servidor.
    Su función es simular un retraso en los paquetes UDP como también las posibles perdidas.
    
Implementación:
    La implementación del programa se basa en 4 hebras.
        - Envío desde Cliente a erp_udp
        - Envío desde erp_udp al Servidor
        - Envío desde el Servidor a erp_udp
        - Envío desde erp_udp al Cliente
    Además para el paso de paquetes entre hilos se utilizan 2 listas fifo.

Bugs:
    -El primer paquete al parecer no vuelve al cliente, volver a intentar sin parar el programa
    (posiblemente problema de la lista)
    
    