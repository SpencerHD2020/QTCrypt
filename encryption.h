#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <QString>
#include <QObject>
#include <QFileInfo>

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
signals:
    void invalidFileName();
    void fileNonExistantOrDir();
};

#endif // ENCRYPTION_H
