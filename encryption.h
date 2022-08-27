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
    // Minimum Constructor
    Encryption(int type, QString fileName);
    // Min + OutFileName
    Encryption(int type, QString fileName, QString outFile);
    // MAX Constructor
    Encryption(int type, QString fileName, QString outFile, QString salt, QString pass);
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
    QString outFileName {"output_file.txt"};
signals:
    void invalidFileName();
    void fileNonExistantOrDir();
    void fileNotAccessible();
    void encryptionComplete();
};

#endif // ENCRYPTION_H
