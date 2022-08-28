#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QDebug>
#include <QPalette>
#include "encryption.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum types {
        ENCRYPT,
        DECRYPT
    };

private slots:
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();
    void connectCurrentEncryptor();

private:
    Ui::MainWindow *ui;
    QList<QString> spawnFileDialog();
    void updateErrorString(QString err);
    Encryption *encryptor {nullptr};
    void processFile(int functor);
};
#endif // MAINWINDOW_H
