#include "encryption.h"

Encryption::Encryption(int type, QString fileName)
{
    purpose = type;
    absolutePath = fileName;
}

bool Encryption::fileExists() {
    QFileInfo checkFile(absolutePath);
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    }
    else {
        return false;
    }
}

bool Encryption::userInfoValid() {
    bool userInfoValid {true};
    // Ideally we would ensure the last chars were .txt but this is fine for now
    if (!absolutePath.contains(".txt")) {
        userInfoValid = false;
        // Emit Failure signal
        emit invalidFileName();
    }
    // Check if file exists - Most likely unnecessary but good to do
    if (!fileExists()) {
        userInfoValid = false;
        emit fileNonExistantOrDir();
    }
    return userInfoValid;
}


std::string Encryption::getDirName(std::string absPath) {
    std::string dirName {absPath};
    for (int i {(static_cast<int>(absolutePath.size()) - 1)}; i > 0; --i) {
        char curChar {dirName[i]};
        if (curChar != '/') {
            dirName = dirName.erase(i, 1);
        }
        else {
            break;
        }
    }
    if (DEBUG) {
        qDebug() << "Dir Path: " << QString::fromStdString(dirName);
    }
    return dirName;
}

int Encryption::getFileSize(QString path) {
    int size {0};
    QFile myFile(path);
    if (myFile.open(QIODevice::ReadOnly)) {
        size = myFile.size();
        myFile.close();
    }
    return size;
}


void Encryption::encrypt() {
    std::string inputFile {absolutePath.toStdString()};
    std::string dirPath {getDirName(inputFile)};
    std::string outFilePath {(dirPath + "output_file.txt")};
    QFile encryptedFile(QString::fromStdString(outFilePath));
    CryptFileDevice cryptDevice(&encryptedFile, QByteArray::fromStdString(ckeyText), QByteArray::fromStdString(ivecText));
    // Read source file and write to encrypted file
    QFile sourceFile(absolutePath);
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        emit fileNotAccessible();
    }
    else {
        QTextStream in(&sourceFile);
        //encryptedFile.open(QIODevice::WriteOnly);
        cryptDevice.open(QIODevice::WriteOnly);
        //QTextStream outStream(&encryptedFile);
        while (!in.atEnd()) {
            QString line {in.readLine()};
            cryptDevice.write(QByteArray::fromStdString(line.toStdString()));
        }
        // Close Both files
       // encryptedFile.close();
        cryptDevice.close();
        sourceFile.close();
        emit encryptionComplete();
    }
    /*
    FILE *ifp = fopen(inputFile.c_str(), "rb");
    std::string dirPath {getDirName(inputFile)};
    std::string outFilePath {(dirPath + "output_file.txt")};
    FILE *ofp = fopen(outFilePath.c_str(), "wb");

    int bytesRead, bytesWritten;
    int blockSize {getFileSize(absolutePath)};
    if (blockSize > 0) {
        // Continue Encryption
        unsigned char indata[blockSize];
        unsigned char outdata[blockSize];
        // Define Keys
        unsigned char *ckey = new unsigned char[ckeyText.length()+1];
        strcpy((char *)ckey, ckeyText.c_str());
        unsigned char *ivec = new unsigned char[ivecText.length()+1];
        strcpy((char *)ivec, ivecText.c_str());

        AES_KEY key;
        AES_set_encrypt_key(ckey, 128, &key);

        int num {0};

        while (true) {
            bytesRead = fread(indata, 1, blockSize, ifp);
            AES_cfb128_encrypt(indata, outdata, bytesRead, &key, ivec, &num, AES_ENCRYPT);
            bytesWritten = fwrite(outdata, 1, bytesRead, ofp);
            if (bytesRead < blockSize) {
                break;
            }
        }
    }
    else {
        // Emit non accessible signal
        emit fileNotAccessible();
    }
    */
}

void Encryption::decrypt() {

}

void Encryption::begin() {
    // Pre Checks
    bool valid {userInfoValid()};
    if (valid) {
        // Call cooresponding function
        switch (purpose) {
            case ENCRYPT:
                encrypt();
                break;
            case DECRYPT:
                decrypt();
                break;
        }
    }
}
