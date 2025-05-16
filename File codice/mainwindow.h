#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "common/rubrica.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // metodi
    void aggiungiContatto();
    void modificaContatto();
    void rimuoviContatto();
    void svuotaContatti();
    void aggiornaLista();
    void espandiContatti();
    void onNome(const QString &text);
    void onCitta(const QString &text);
    void onNumero(const QString &text);
    void onEmail(const QString &text);
    void onEta(const QString &text);
    void checkInputs();
    void espandi(QTreeWidgetItem *item, int colonna);
    void aggiungiConferma();
    void tornaIndietro();

private:
    // attributi
    Ui::MainWindow *ui;
    Rubrica rubrica;
    QTreeWidget *treeWidget;
    bool expand = true;
    bool editMode = false;
    // attributi utilizzata per la modifica e l'aggiunta
    Info infoVecchie;
    bool correctInputs[5];
    void aggiungiListaRiga(const QString &nome, const QString &numero);
};

#endif
