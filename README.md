# QTCrypt

### Description
A simple txt file encryption program to safely guard text data. 

### Usage

![UI Image](/images/mainUI.png?raw=true)

- All Fields in the UI are optional.       
    - While the default password and salt are usable, it is not reccommended to ensure maximum security of your data.          
- Currently (8/30/2022), the output file name input does not support passing path like strings. 
    - The output file will always be created in the same directory as the file you are encrypting/decrypting. 
    - The only supported file type is *.txt
    - You do not need to add .txt to the output file name field, although it will not hurt anything if you do. 
- Fill out all input fields before pressing the Encrypt/Decrypt button
    - Once the target file has been specified, the operation will immediately begin. 