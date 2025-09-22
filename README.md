# Progetto-TCP
Il progetto consiste nella realizzazione di un'applicazione TCP client/server, in cui il server funge da generatore di password in base alle richieste ricevute dal client. Una volta stabilita la connessione, il server registra l'indirizzo e la porta del client, mostrando un messaggio di log sullo standard output.

Il client, tramite input da tastiera, specifica il tipo di password desiderata e la lunghezza, utilizzando un protocollo semplice definito attraverso caratteri identificativi:
n: password numerica (solo cifre)
a: password alfabetica (solo lettere minuscole)
m: password mista (lettere minuscole e numeri)
s: password sicura (lettere maiuscole, minuscole, numeri e simboli)
Ad esempio, inserendo n 8 viene richiesta al server una password numerica di 8 caratteri. Il server elabora la richiesta, genera la password utilizzando le funzioni dedicate (generate_numeric(), generate_alpha(), generate_mixed(), generate_secure()) e la restituisce al client, che la visualizza sul proprio terminale.
Il sistema rimane attivo fino all'inserimento del comando q, che provoca la chiusura della connessione lato client. Il server, invece, resta sempre in esecuzione ed è in grado di gestire fino a 5 connessioni contemporanee.

Questo progetto ha permesso di approfondire i concetti di programmazione di rete in C, gestione delle socket, protocolli applicativi e sicurezza nella generazione delle password, con particolare attenzione all'affidabilità e alla modularità del codice.
