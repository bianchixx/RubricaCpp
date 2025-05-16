#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QInputDialog>
#include <QPushButton>
#include <QVBoxLayout>

// COSTRUTTORE, INIZIALIZZA LA FINESTRA PRINCIPALE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , infoVecchie("", "", "", "", "")
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(0);

    // Inizializza tutti gli input come non corretti
    for (int i = 0; i < 5; ++i) {
        correctInputs[i] = false;
    }

    // Configura finestra di output
    treeWidget = ui->treeWidget;
    treeWidget->setHeaderHidden(true);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);

    // Collega gli input in aggiunta o modifica alle funzioni di verifica
    connect(treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::espandi);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &MainWindow::onNome);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &MainWindow::onNumero);
    connect(ui->lineEdit_3, &QLineEdit::textEdited, this, &MainWindow::onCitta);
    connect(ui->lineEdit_4, &QLineEdit::textEdited, this, &MainWindow::onEmail);
    connect(ui->lineEdit_5, &QLineEdit::textEdited, this, &MainWindow::onEta);

    // Collega i bottoni alle funzioni
    connect(ui->btnAggiungi, &QPushButton::clicked, this, &MainWindow::aggiungiContatto);
    connect(ui->btnModifica, &QPushButton::clicked, this, &MainWindow::modificaContatto);
    connect(ui->btnSvuota, &QPushButton::clicked, this, &MainWindow::svuotaContatti);
    connect(ui->btnRimuovi, &QPushButton::clicked, this, &MainWindow::rimuoviContatto);
    connect(ui->btnEspandi, &QPushButton::clicked, this, &MainWindow::espandiContatti);
    connect(ui->newAdd, &QPushButton::clicked, this, &MainWindow::aggiungiConferma);
    connect(ui->newGoBack, &QPushButton::clicked, this, &MainWindow::tornaIndietro);

    aggiornaLista();
}

// DISTRUTTORE

MainWindow::~MainWindow()
{
    delete ui;
}

// AGGIUNGI UN NUOVO CONTATTO

void MainWindow::aggiungiContatto() {
    // Configura l’interfaccia per l’aggiunta
    ui->label->setText("AGGIUNGI CONTATTO:");
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(1);
}

// CONFERMA E SALVA UN NUOVO CONTATTO O LO MODIFICA

void MainWindow::aggiungiConferma(){
    // Salva nuovo contatto nella rubrica o aggiorna vecchio contatto con nuove informazioni
    Info info(ui->lineEdit->text().toStdString(),ui->lineEdit_3->text().toStdString(),ui->lineEdit_2->text().toStdString(),ui->lineEdit_4->text().toStdString(),ui->lineEdit_5->text().toStdString());
    if(!editMode){
        rubrica.aggiungi(info);
    } else{
        rubrica.modifica(infoVecchie.nome,info);
    }
    tornaIndietro();
}

// TORNA ALLA LISTA DEI CONTATTI E RIPRISTINA L’INTERFACCIA

void MainWindow::tornaIndietro(){
    // Svuota i campi e resetta il tutto
    ui->lineEdit->setText(""); onNome("");
    ui->lineEdit_2->setText(""); onNumero("");
    ui->lineEdit_3->setText(""); onCitta("");
    ui->lineEdit_4->setText(""); onEmail("");
    ui->lineEdit_5->setText(""); onEta("");
     ui->stackedWidget->setCurrentIndex(1);
    for (int i = 0; i < 5; ++i) {
        correctInputs[i] = false;
    }
    editMode = false;
    // Ripristina i bottoni iniziali
    ui->stackedWidget_2->setCurrentIndex(0);
    expand = false;
    espandiContatti();
    aggiornaLista();
    ui->label->setText("CONTATTI MEMORIZZATI:");
    ui->newAdd->setText("Aggiungi");
}

// MODIFICA UN CONTATTO ESISTENTE

void MainWindow::modificaContatto() {
    // Cerca il contatto in base al nome
    bool ok;
    QString nomeVecchio = QInputDialog::getText(this, "Modifica Contatto", "Inserisci il nome del contatto da modificare:", QLineEdit::Normal, "", &ok);
    if (!ok || nomeVecchio.isEmpty() ) {
        QMessageBox::warning(this, "Errore", "Inserisci un nome valido!");
        return;
    }

    Info infoMod(nomeVecchio.toStdString(),"","","","");

    if (rubrica.trova(infoMod)){
        editMode = true;
        // Configura l'interfaccia per la modifica
        ui->label->setText("MODIFICA CONTATTO:");
        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget_2->setCurrentIndex(1);
        ui->newAdd->setText("Modifica");

        Info data = rubrica.getInfoDaNome(nomeVecchio.toStdString());
        infoVecchie = data;
        ui->lineEdit->setText(QString::fromStdString(data.nome)); onNome(ui->lineEdit->text());
        ui->lineEdit_2->setText(QString::fromStdString(data.numero)); onNumero(ui->lineEdit_2->text());
        ui->lineEdit_3->setText(QString::fromStdString(data.città)); onCitta(ui->lineEdit_3->text());
        ui->lineEdit_4->setText(QString::fromStdString(data.email)); onEmail(ui->lineEdit_4->text());
        ui->lineEdit_5->setText(QString::fromStdString(data.età)); onEta(ui->lineEdit_5->text());
    }
    else{
        QMessageBox::warning(this, "Errore", "Inserisci un nome valido!");
        return;
    }
}

// RIMUOVE UN CONTATTO DALLA RUBRICA

void MainWindow::rimuoviContatto() {
    // Cerca il contatto in base al nome
    bool ok;
    QString nome = QInputDialog::getText(this, "Rimuovi Contatto", "Inserisci il nome:", QLineEdit::Normal, "", &ok);
    if (!ok || nome.isEmpty()) {
        QMessageBox::warning(this, "Errore", "Inserisci un nome valido!");
        return;
    }

    Info infoRim(nome.toStdString(),"","","","");

    if (rubrica.trova(infoRim)){
        rubrica.rimuovi(nome.toStdString());
    }
    else{
        QMessageBox::warning(this, "Errore", "Nessun contatto con questo nome è stato trovato!");
        return;
    }

    QMessageBox::information(this, "Eliminato", "Contatto rimosso.");
    aggiornaLista();
    expand = false; espandiContatti();
}

// RIMUOVE TUTTI I CONTATTI

void MainWindow::svuotaContatti() {
    if(rubrica.lunghezza() != 0){
        // Crea messaggio per chiedere la conferma
        QMessageBox msgBox; msgBox.setWindowTitle("Svuota Contatti"); msgBox.setText("Sei sicuro di voler eliminare tutti i contatti?");
        QPushButton *siButton = msgBox.addButton(tr("Sì"), QMessageBox::YesRole);
        QPushButton *noButton = msgBox.addButton(tr("No"), QMessageBox::NoRole);
        msgBox.setDefaultButton(noButton); msgBox.exec();
        if(msgBox.clickedButton() == siButton){
            rubrica.svuota();
            QMessageBox::information(this, "Svuotato", "Tutti i contatti sono stati eliminati.");
            aggiornaLista();
        }
    }
    else{
        QMessageBox::information(this, "Non Svuotato", "Non è memorizzato nessun contatto.");
    }
}

// AGGIORNA LA LISTA DEI CONTATTI NELLA FINESTRA PRINCIPALE

void MainWindow::aggiornaLista(){
    // Formatta output per visualizzarlo correttamente
    rubrica.memorizza();
    treeWidget->clear();
    Nodo* current = rubrica.getTesta();
    while(current!=nullptr){
        QString nome = QString::fromStdString(current->info.nome);
        QString città = QString::fromStdString(current->info.città);
        QString numero = QString::fromStdString(current->info.numero);
        QString email = QString::fromStdString(current->info.email);
        QString età = QString::fromStdString(current->info.età);
        current = current->prossimo;
        QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);
        item->setText(0, nome);
        item->setText(1, "Nome: " + nome + "\nNumero di Tel: " + numero + "\nCittà: " + città +  "\nEmail: " + email + "\nEtà: " + età + "\n");
        item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        treeWidget->addTopLevelItem(item);
    }
}

// ESPANDE O RIDUCE I DETTAGLI DI UN CONTATTO QUANDO VIENE CLICCATO

void MainWindow::espandi(QTreeWidgetItem *item, int colonna){
    if (!item->parent() && item->childCount() == 0) {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0,item->text(1));
        item->addChild(child);
    }
    item->setExpanded(!item->isExpanded());
}

// ESPANDE O RIDUCE TUTTI I CONTATTI NELLA FINESTRA PRINCIPALE

void MainWindow::espandiContatti(){
    if(expand){
        for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
            QTreeWidgetItem *item = treeWidget->topLevelItem(i);
            if (item->childCount() == 0) {
                QTreeWidgetItem *child = new QTreeWidgetItem();
                child->setText(0, item->text(1));
                item->addChild(child);
            }
            item->setExpanded(true);
        }
        ui->btnEspandi->setText("Riduci");
    }
    else{
        for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
            QTreeWidgetItem *item = treeWidget->topLevelItem(i);
            item->setExpanded(false);
        }
        ui->btnEspandi->setText("Espandi");
    }
    // Espandi e riduci si alternano
    expand = !expand;
}

// VERIFICA CHE TUTTI GLI INPUT SIANO VALIDI PER ATTIVARE I BOTTONI

void MainWindow::checkInputs() {
    bool allCorrect = true;
    for (int i = 0; i < 5; ++i) {
        if (!correctInputs[i]) {
            allCorrect = false;
            break;
        }
    }

    // Stile separato per aggiunta e modifica
    if (!editMode && allCorrect) {
        ui->newAdd->setEnabled(true);
        ui->newAdd->setStyleSheet(
            "QPushButton {""background-color: #4CAF50;""color: white;""font-size: 15px;""font-weight: bold;""border-radius: 20%;""padding: 10px;""}""QPushButton:hover {""background-color: #45a049;""}""QPushButton:pressed {""background-color: #388E3C;""}"
            );
    } else if (allCorrect){
        ui->newAdd->setEnabled(true);
        ui->newAdd->setStyleSheet(
            "QPushButton {""background-color: #4A6FCF;""color: white;""font-size: 15px;""font-weight: bold;""border-radius: 20%;""padding: 10px;""}""QPushButton:hover {""background-color: #456AA0;""}""QPushButton:pressed {""background-color: #385C8E;""}"
            );
    }
    else {
        ui->newAdd->setEnabled(false);
        ui->newAdd->setStyleSheet(
            "QPushButton {""background-color: #eb6b34;""color: white;""font-size: 15px;""font-weight: bold;""border-radius: 20%;""padding: 10px;""}""QPushButton:hover {""background-color: #d65f2f;""}""QPushButton:pressed {""background-color: #bf5329;""}"
            );
    }
}

// VERIFICANO CHE L'IMPUT QUANDO SI AGGIUNGE O MODIFICA SIA CORRETTO

// Imput per il nome
void MainWindow::onNome(const QString &text) {
    if (text.isEmpty()) {
        ui->label_2->setText("Nome non valido!");
        ui->label_2->setStyleSheet("color: gold;");
        correctInputs[0] = false;
    } else if (std::any_of(text.begin(), text.end(), [](QChar c) { return std::isdigit(c.toLatin1()); })) {
        ui->label_2->setText("Il nome non può contenere cifre!");
        ui->label_2->setStyleSheet("color: red;");
        correctInputs[0] = false;
    } else if (infoVecchie.nome!=text.toStdString() && rubrica.trova(Info(text.toStdString(), "", "", "", ""))) {
        ui->label_2->setText("Nome già inserito!");
        ui->label_2->setStyleSheet("color: gold;");
        correctInputs[0] = false;
    } else {
        ui->label_2->setText("Informazioni corrette!");
        ui->label_2->setStyleSheet("color: green;");
        correctInputs[0] = true;
    }
    checkInputs();
}

// Imput per il numero di telefono
void MainWindow::onNumero(const QString &text) {
    if (text.isEmpty()) {
        ui->label_3->setText("Numero non valido!");
        ui->label_3->setStyleSheet("color: gold;");
        correctInputs[2] = false;
    } else if (text.length() != 10 || !std::all_of(text.begin(), text.end(), [](QChar c) { return std::isdigit(c.toLatin1()); })) {
        ui->label_3->setText("Il numero deve essere di 10 cifre!");
        ui->label_3->setStyleSheet("color: red;");
        correctInputs[2] = false;
    } else if (infoVecchie.numero!=text.toStdString() && rubrica.trova(Info("", "", text.toStdString(),"", ""))) {
        ui->label_3->setText("Numero già inserito!");
        ui->label_3->setStyleSheet("color: gold;");
        correctInputs[2] = false;
    } else {
        ui->label_3->setText("Informazioni corrette!");
        ui->label_3->setStyleSheet("color: green;");
        correctInputs[2] = true;
    }
    checkInputs();
}

// Imput per la città
void MainWindow::onCitta(const QString &text) {
    if (text.isEmpty()) {
        ui->label_4->setText("Città non valida!");
        ui->label_4->setStyleSheet("color: gold;");
        correctInputs[1] = false;
    } else if  (std::any_of(text.begin(), text.end(), [](QChar c) { return std::isdigit(c.toLatin1()); })) {
        ui->label_4->setText("La città non può contenere cifre!");
        ui->label_4->setStyleSheet("color: red;");
        correctInputs[0] = false;
    } else {
        ui->label_4->setText("Informazioni corrette!");
        ui->label_4->setStyleSheet("color: green;");
        correctInputs[1] = true;
    }
    checkInputs();
}

// Imput per l'email
void MainWindow::onEmail(const QString &text) {
    if (text.isEmpty()) {
        ui->label_5->setText("Email non valida!");
        ui->label_5->setStyleSheet("color: gold;");
        correctInputs[3] = false;
    } else if (text.indexOf('@') == -1) {
        ui->label_5->setText("L'email deve contenere '@'!");
        ui->label_5->setStyleSheet("color: red;");
        correctInputs[3] = false;
    } else if (infoVecchie.email!=text.toStdString() && rubrica.trova(Info( "", "", "",text.toStdString(), ""))) {
        ui->label_5->setText("Email già inserita!");
        ui->label_5->setStyleSheet("color: gold;");
        correctInputs[3] = false;
    } else {
        ui->label_5->setText("Informazioni corrette!");
        ui->label_5->setStyleSheet("color: green;");
        correctInputs[3] = true;
    }
    checkInputs();
}

// Imput per l'età
void MainWindow::onEta(const QString &text) {
    if (text.isEmpty()) {
        ui->label_6->setText("Età non valida!");
        ui->label_6->setStyleSheet("color: gold;");
        correctInputs[4] = false;
    } else if (!std::all_of(text.begin(), text.end(), [](QChar c) { return std::isdigit(c.toLatin1()); })) {
        ui->label_6->setText("L'età deve contenere solo cifre!");
        ui->label_6->setStyleSheet("color: red;");
        correctInputs[4] = false;
    } else {
        ui->label_6->setText("Informazioni corrette!");
        ui->label_6->setStyleSheet("color: green;");
        correctInputs[4] = true;
    }
    checkInputs();
}
