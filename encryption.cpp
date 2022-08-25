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

void Encryption::encrypt() {

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
