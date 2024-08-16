# xor_shred
It's a file shredder tool that securely "shreds" a file by overwriting its contents with random data. It XORs the bytes of the file with a totally random one. It utilizes the [`arc4random`](https://man.openbsd.org/arc4random) function to generate secure random bytes.

## Usage
```
./xor_shred <filename>
```

# xor_encrypt
It's a simple file encryption tool that uses the XOR to encrypt or decrypt a file. The encryption is done by XORing each byte of the file's content with the key. This key is modified incrementally for each byte.

## Usage
```
./xor_encrypt <filename> <key>
<filename>: The name of the file to be encrypted or decrypted.
<key>: An integer key used for the XOR operation.
```