#include "ftp.h"

FTPServer ftp;

void initFTP() {
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS); // SCLK, MISO, MOSI, CS

    if (SD.begin(SD_CS, SPI)) {
        #if DEBUG
        SerialMon.println("SD opened!");
        #endif
    }

    ftp.addUser(ftpuser, ftppass);

    ftp.addFilesystem("SD", &SD);

    ftp.begin();
}

void loopFTP() {
  ftp.handle();
}