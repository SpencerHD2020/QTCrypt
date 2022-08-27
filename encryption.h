#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <QString>
#include <QObject>
#include <QFileInfo>
#include <openssl/aes.h>
#include <QDebug>
#include <QFile>
#include "cryptfiledevice.h"
#include <QTextStream>
#include <QStringList>

class Encryption : public QObject
{
    Q_OBJECT
public:
    Encryption(int type, QString fileName);
    enum types {
        ENCRYPT,
        DECRYPT
    };
    void begin();
private:
    int purpose;
    QString absolutePath;
    void encrypt();
    void decrypt();
    bool fileExists();
    bool userInfoValid();
    bool DEBUG {true};
    std::string getDirName(std::string absPath);
    int getFileSize(QString path);
    std::string ckeyText {"jxtlwamtgalyxeth"};
    std::string ivecText {"apemctyukkdlawyt"};
signals:
    void invalidFileName();
    void fileNonExistantOrDir();
    void fileNotAccessible();
    void encryptionComplete();
};

#endif // ENCRYPTION_H
