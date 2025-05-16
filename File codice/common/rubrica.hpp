#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "nodo.hpp"

// Gestisce una rubrica di contatti utilizzando una lista di nodi.
class Rubrica {
private:
    // Puntatore alla testa
    Nodo* testa;

public:
    // Costruttore, inizializza la lista e la riempie in base ai dati presenti nel file CSV
    Rubrica() : testa(nullptr) {
        this->riempi();
    }

    // Crea la cartella "data" e un file CSV vuoto per i contatti, se non esistono già
    void crea_file_vuoto_data() {
        std::filesystem::path cartella = "data";
        if (!std::filesystem::exists(cartella)) {
            std::filesystem::create_directory(cartella);
        }
        std::ofstream file("data/data.csv");
        file.close();
    }

    // Converte una stringa in minuscolo per confronti case-insensitive
    std::string convertiMinuscolo(std::string stringaMinuscolo) {
        for (char& c : stringaMinuscolo) {
            c = std::tolower(c);
        }
        return stringaMinuscolo;
    }

    // Conta il numero di righe (quindi i contatti) presenti nel file CSV
    size_t conta_contatti() {
        std::ifstream file("data/data.csv");
        size_t cont = 0;
        std::string riga;
        while (getline(file, riga)) {
            cont++;
        }
        file.close();
        return cont;
    }

    // Legge tutti i contatti dal file CSV e li inserisce nella lista
    void riempi() {
        Info info("","","","","");
        Nodo* nuovoNodo;
        size_t righeTotali = this->conta_contatti();
        for (size_t i = 0; i < righeTotali; i++) {
            riempi_data_riga(i,&info);
            nuovoNodo = new Nodo(info);
            nuovoNodo->prossimo = testa;
            testa = nuovoNodo;
        }
        this->ordina();
    }

    // Estrae i dati di una riga specifica del file e li inserisce in un nodo
    void riempi_data_riga(size_t rigaAttuale, Info* info) {
        std::ifstream file("data/data.csv");
        std::string riga;
        size_t i = 0;
        while (getline(file, riga)) {
            if (i == rigaAttuale) {
                std::stringstream ss(riga);
                getline(ss, info->nome, ',');
                getline(ss, info->città, ',');
                getline(ss, info->numero, ',');
                getline(ss, info->email, ',');
                getline(ss, info->età);
                file.close();
                return;
            }
            i++;
        }
        file.close();
    }

    // Aggiunge un nuovo contatto alla lista
    void aggiungi(Info info) {
        Nodo* nuovoNodo = new Nodo(info);
        nuovoNodo->prossimo = testa;
        testa = nuovoNodo;
        this->ordina();
    }

    // Modifica un contatto esistente in base al nome attuale e lo sovrascrive con nuove informazioni
    void modifica(std::string nomeVecchio, Info info) {
        Nodo* temp = testa;
        while (temp && temp->info.nome != nomeVecchio) {
            temp = temp->prossimo;
        }
        if (!temp) return;

        temp->info = info;
        this->ordina();
    }

    // Verifica se esiste un contatto con uno o più campi corrispondenti
    bool trova(Info info) {
        Nodo* temp = testa;
        while (temp) {
            if (temp->info.nome == info.nome || temp->info.città == info.città || temp->info.numero == info.numero || temp->info.email == info.email || temp->info.età == info.età) return true;
            temp = temp->prossimo;
        }
        return false;
    }

    // Ordina la lista dei contatti in ordine alfabetico in base al nome
    void ordina() {
        Nodo* corrente = testa;
        while (corrente) {
            Nodo* minimo = corrente;
            Nodo* temp = corrente->prossimo;
            while (temp) {
                if (convertiMinuscolo(temp->info.nome) < convertiMinuscolo(minimo->info.nome)) {
                    minimo = temp;
                }
                temp = temp->prossimo;
            }
            std::swap(minimo->info, corrente->info);
            corrente = corrente->prossimo;
        }
    }

    // Elimina tutti i nodi della lista
    void svuota() {
        while (testa) {
            Nodo* temp = testa;
            testa = testa->prossimo;
            delete temp;
        }
    }

    // Rimuove un contatto dalla lista in base al nome attuale
    void rimuovi(std::string nome) {
        if (!testa) return;
        if (testa->info.nome == nome) {
            Nodo* temp = testa;
            testa = testa->prossimo;
            delete temp;
            return;
        }
        Nodo* temp = testa;
        while (temp->prossimo && temp->prossimo->info.nome != nome) {
            temp = temp->prossimo;
        }
        if (temp->prossimo) {
            Nodo* rimozione = temp->prossimo;
            temp->prossimo = temp->prossimo->prossimo;
            delete rimozione;
        }
    }

    // Restituisce il puntatore alla testa della lista (accesso diretto ai nodi)
    Nodo* getTesta() {
        return testa;
    }

    // Scrive tutti i contatti nel file CSV
    void memorizza() {
        Nodo* corrente = testa;
        this->crea_file_vuoto_data();
        while (corrente) {
            this->memorizza_data(corrente->info);
            corrente = corrente->prossimo;
        }
    }

    // Restituisce le informazioni di un nodo in base al nome attuale
    Info getInfoDaNome(std::string nome){
        Nodo* corrente = testa;
        Info res("","","","","");
        while (corrente) {
            if(nome == corrente->info.nome){
                res = corrente->info;
            }
            corrente = corrente->prossimo;
        }
        return res;
    }

    // Scrive un singolo contatto nel file CSV senza sovrascrivere
    void memorizza_data(Info info) {
        std::ofstream file("data/data.csv", std::ios::app);
        file << info.nome << "," << info.città << "," << info.numero << "," << info.email << "," << info.età <<"\n";
        file.close();
    }

    // Restituisce il numero di contatti attualmente memorizzati nella lista
    size_t lunghezza() {
        Nodo* corrente = testa;
        size_t cont = 0;
        while (corrente) {
            cont++;
            corrente = corrente->prossimo;
        }
        return cont;
    }
};
