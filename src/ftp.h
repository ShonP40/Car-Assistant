#pragma once

#include "globals.h"

#include <SD.h>
#include <ESP-FTP-Server-Lib.h>
#include <FTPFilesystem.h>

// Export functions
void initFTP();
void loopFTP();

// Extern FTP
extern FTPServer ftp;