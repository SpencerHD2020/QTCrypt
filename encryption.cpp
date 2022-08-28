#include "encryption.h"

// Constructors
Encryption::Encryption(int type, QString fileName) : purpose{type}, absolutePath{fileName} {};
Encryption::Encryption(int type, QString fileName, QString outFile) : purpose{type}, absolutePath{fileName} {
    setOutFileFromInput(outFile);
};

Encryption::Encryption(int type, QString fileName, QString outFile, QString salt, QString pass) : purpose{type}, absolutePath{fileName} {
    setOutFileFromInput(outFile);
    // Validate salt and pass and convert to standard strings
    if (salt.length() > 0) {
        ckeyText = salt.toStdString();
    }
    if (pass.length() > 0) {
        ivecText = pass.toStdString();
    }
};

void Encryption::setOutFileFromInput(QString input) {
    // Validate FileName
    if (input.contains(".")) {
        // Ensure extension is txt
        outFileName = (input.split(".")[0] + ".txt");
    }
    else if (input.length() > 0) {
        // Append .txt on end of fileName
        outFileName = (input + ".txt");
    }
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
    // TODO: Need to consider if user passes a path versus filename here
    std::string outFilePath {(dirPath + outFileName.toStdString())};
    QFile encryptedFile(QString::fromStdString(outFilePath));
    CryptFileDevice cryptDevice(&encryptedFile, QByteArray::fromStdString(ckeyText), QByteArray::fromStdString(ivecText));
    // Read source file and write to encrypted file
    QFile sourceFile(absolutePath);
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        emit fileNotAccessible();
    }
    else {
        QTextStream in(&sourceFile);
        cryptDevice.open(QIODevice::WriteOnly);
        while (!in.atEnd()) {
            QString line {in.readLine()};
            cryptDevice.write(QByteArray::fromStdString(line.toStdString()));
        }
        // Close Both files
        cryptDevice.close();
        sourceFile.close();
        emit encryptionComplete();
    }
}

void Encryption::decrypt() {
    std::string encryptedFilePath {absolutePath.toStdString()};
    std::string dirPath {getDirName(encryptedFilePath)};
    // TODO: Need to consider if user passes a path versus filename here
    std::string outFilePath {(dirPath + outFileName.toStdString())};
    QFile decryptedFile(QString::fromStdString(outFilePath));
    QFile encryptedFile(absolutePath);
    CryptFileDevice cryptDevice(&encryptedFile, QByteArray::fromStdString(ckeyText), QByteArray::fromStdString(ivecText));
    // Open Files
    cryptDevice.open(QIODevice::ReadOnly);
    decryptedFile.open(QIODevice::WriteOnly);
    // Decrypt Data
    QByteArray decryptedData {cryptDevice.readAll()};
    // Write Decrypted Data to output File
    decryptedFile.write(decryptedData);
    // Close Both Files
    cryptDevice.close();
    decryptedFile.close();
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
