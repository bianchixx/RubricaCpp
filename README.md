
# DOCUMENTAZIONE - RUBRICA IN C++


Semplice implementazione di una rubrica di contatti basata su una lista, con memorizzazione su file CSV. Ogni contatto è composto da:

- Nome
- Città
- Numero di telefono
- Email
- Età

I dati sono letti/scritti dal file: data.csv


STRUCT: Info
--------------
Contiene i dati di un singolo contatto.

    struct Info {
        std::string nome;     // Nome del contatto
        std::string città;    // Città di residenza
        std::string numero;   // Numero di telefono
        std::string email;    // Indirizzo email
        std::string età;      // Età
    };

CLASSE: Nodo
------------
Rappresenta un nodo della lista collegata.

    class Nodo {
    public:
        Info info;            // Dati del contatto
        Nodo* prossimo;       // Puntatore al nodo successivo

        Nodo(Info i);         // Costruttore
    };

------------------------------------------------------------
CLASSE: Rubrica
------------------------------------------------------------

Gestisce la lista collegata e le operazioni principali
sui contatti.

COSTRUTTORE:

    Rubrica();
    - Inizializza la lista a vuota e carica i dati dal file CSV.

METODI PRINCIPALI:

    void crea_file_vuoto_data()
    - Crea la cartella 'data/' e un file CSV vuoto se non esistono.

    std::string convertiMinuscolo(std::string s)
    - Converte una stringa in minuscolo (usata per confronti/ordinamento).

    size_t conta_contatti()
    - Conta il numero di righe (contatti) nel file CSV.

    void riempi()
    - Legge tutti i contatti dal file CSV e li carica nella lista collegata.
    - Dopo il caricamento, la lista viene ordinata per nome.

    void aggiungi(Info nuovo)
    - Aggiunge un nuovo contatto alla lista e ordina.

    void modifica(std::string nomeVecchio, Info aggiornato)
    - Cerca il contatto con 'nomeVecchio' e lo aggiorna con nuovi dati.

    bool trova(Info i)
    - Restituisce true se esiste un contatto che corrisponde a uno dei campi.

    void ordina()
    - Ordina alfabeticamente la lista collegata per nome.

    void rimuovi(std::string nome)
    - Rimuove il contatto con il nome specificato.

    void svuota()
    - Cancella tutta la lista e libera la memoria.

    void memorizza()
    - Scrive tutti i contatti della lista nel file 'data/data.csv'.

    void memorizza_data(Info i)
    - Aggiunge un singolo contatto alla fine del file CSV.

    Info getInfoDaNome(std::string nome)
    - Restituisce i dati completi di un contatto cercato per nome.

    size_t lunghezza()
    - Restituisce il numero di nodi (contatti) nella lista.

------------------------------------------------------------
Formato del file data.csv
------------------------------------------------------------

Ogni riga rappresenta un contatto, con i campi separati da virgole:

    nome,città,numero,email,età

Esempio:
    Mario Rossi,Roma,3331234567,mario@gmail.com,45

------------------------------------------------------------



## IMPLEMENTAZIONE SU QT
La rubrica è implementata sotto forma di un'applicazione GUI di gestione di contatti, sviluppata utilizzando Qt in C++. La sua interfaccia grafica permette agli utenti di aggiungere, modificare, rimuovere e visualizzare contatti.

Struttura del Codice
--------------
Il codice è strutturato all'interno della classe MainWindow, che rappresenta la finestra principale dell'applicazione. La classe gestisce vari aspetti dell'interfaccia utente, inclusi i moduli per inserire o modificare i contatti e la visualizzazione di questi.

Funzionalità Principali
--------------
    void aggiungiContatto()
    - Aggiunge un nuovo contatto. Vengono visualizzati due pulsanti: "Aggiungi" e "Torna Indietro".
    - Il pulsante "Aggiungi" è inizialmente disabilitato e viene abilitato solo se tutti i campi sono corretti.

    void aggiungiConferma()
    - Dopo aver cliccato "Aggiungi", il contatto viene memorizzato nell'archivio (rubrica).

    void tornaIndietro()
    - Resetta i campi e riporta l'utente alla visualizzazione principale.

    void modificaContatto()
    - Permette di modificare un contatto esistente. I campi di input vengono popolati con i dati del contatto esistente, se trovato.

    void rimuoviContatto()
    - Permette di rimuovere un contatto dalla rubrica. Se il contatto esiste, verrà eliminato.

    void svuotaRubrica()
    - Elimina tutti i contatti salvati. L'utente viene avvisato prima di procedere con l'eliminazione.

    void aggiornaLista()
    - Aggiorna e visualizza la lista dei contatti, mostrando i dettagli di ciascun contatto in un albero.

    void espandiContatti()
    - Permette di espandere o ridurre i dettagli di ciascun contatto. Quando espanso, vengono mostrati dettagli aggiuntivi.


Gestione Input e Validazione
--------------
- Nome: Viene validato per assicurarsi che non contenga numeri e deve essere univoco.

- Numero di Telefono: Deve essere composto da 10 cifre e deve essere univoco.

- Città: Non può contenere numeri e deve essere valida.

- Email: Deve essere univoca.

- Età: Deve essere composta solo da numeri.

Dettagli Tecnici
--------------
Gestione dei Pulsanti:
-  Ogni pulsante ha uno stile personalizzato tramite l'uso di setStyleSheet, che definisce il colore di sfondo, la dimensione del testo, e gli effetti di hover/pressione.

QInputDialog e QMessageBox:

- Vengono utilizzati rispettivamente per ottenere input da parte dell'utente e per mostrare messaggi di errore o conferma (ad esempio, quando si elimina un contatto o si tenta di aggiungere un contatto con un nome già esistente).

