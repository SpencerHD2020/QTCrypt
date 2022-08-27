#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::spawnFileDialog() {
    QFileDialog dialog(this);
    dialog.setNameFilter("Text (*.txt)");
    if (dialog.exec()) {
        return dialog.selectedFiles();
    }
    else {
        QList<QString> temp {"null"};
        return temp;
    }
}

void MainWindow::updateErrorString(QString err) {
    this->ui->mainLabel->setText(("QTCrypt \n" + err));
    // TODO: Change Label Color
}


void MainWindow::connectCurrentEncryptor() {
    /*
    void invalidFileName();
    void fileNonExistantOrDir();
    void fileNotAccessible();
    void encryptionComplete();
    */
    connect(encryptor, &Encryption::invalidFileName, this, [=]() {
       updateErrorString("Invalid File Name!");
    });
    connect(encryptor, &Encryption::fileNonExistantOrDir, this, [=](){
       updateErrorString("File does not exist!");
    });
    connect(encryptor, &Encryption::fileNotAccessible, this, [=](){
       updateErrorString("File not accessible!");
    });
    connect(encryptor, &Encryption::encryptionComplete, this, [=](){
       updateErrorString("Encryption Complete!");
    });
}


// Consider breaking Designer connects and making this one function
void MainWindow::on_encryptButton_clicked()
{
    // Once passed to the Encrypt class we will check for null entry
    QString outputFileName {this->ui->outFileName->toPlainText()};
    // Spawn File Dialog - Turn this into a function
    QList<QString> fileNames {spawnFileDialog()};
    if (fileNames[0] != "null") {
        // Proceed with encryption
        for (int i {0}; i < fileNames.size(); ++i) {
            QString file {fileNames[i]};
            // qDebug() << file; RETURNS ABSOLUTE PATH
            Encryption *en = new Encryption(ENCRYPT, file);
            encryptor = en;
            connectCurrentEncryptor();
            en->begin();
        }
    }
}


void MainWindow::on_decryptButton_clicked()
{

}

