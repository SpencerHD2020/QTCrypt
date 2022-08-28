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
    // Change Label Color
    QPalette mainLabelPallete;
    if (!err.contains("Complete")) {
        mainLabelPallete.setColor(QPalette::Window, Qt::red);
    }
    else {
        mainLabelPallete.setColor(QPalette::Window, Qt::green);
    }
    this->ui->mainLabel->setPalette(mainLabelPallete);
}


void MainWindow::connectCurrentEncryptor() {
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

void MainWindow::processFile(int functor) {
    // Collect Password and Salt
    QString password {this->ui->passEdit->text()};
    QString salt {this->ui->saltEdit->text()};
    // Once passed to the Encrypt class we will check for null entry
    QString outputFileName {this->ui->outFileName->text()};
    // Spawn File Dialog - Turn this into a function
    QList<QString> fileNames {spawnFileDialog()};
    if (fileNames[0] != "null") {
        // Proceed with encryption
        for (int i {0}; i < fileNames.size(); ++i) {
            QString file {fileNames[i]};
            // qDebug() << file; RETURNS ABSOLUTE PATH
            // Currently, we need to wait until the function completes before proceeding
            Encryption *en = new Encryption(functor, file, outputFileName, salt, password);
            encryptor = en;
            connectCurrentEncryptor();
            en->begin();
            break;
        }
        // We could disable the buttons at this point, but if encryption failed the app may need to be restarted, although we could reenable them on each failure signal
    }
}

void MainWindow::on_encryptButton_clicked()
{
    processFile(ENCRYPT);
}


void MainWindow::on_decryptButton_clicked()
{
    processFile(DECRYPT);
}

