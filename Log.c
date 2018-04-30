#include "Log.h"

int Log_abrir(Log *LG, const char *filename, int debug) {
	LG->debug = debug;

	if (LG->debug) {
		LG->fl.l_type = F_WRLCK;
		LG->fl.l_whence = SEEK_SET;
		LG->fl.l_start = 0;
		LG->fl.l_len = 0;

		LG->fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);

		if (LG->fd == -1)
			return ERROR_LOG_OPEN;
	}

	return LOG_OK;
}

int Log_escribir(Log *LG, const char *msg, ...) {
	int return_value = LOG_OK;

	if (LG->debug) {
		char buffer[MSG_MAX_SIZE];

		va_list args;
		va_start(args, msg);
		vsprintf(buffer, msg, args);
		va_end(args);

		if (strlen(msg) > MSG_MAX_SIZE) {
			return ERROR_LOG_WRITE_MSGTOOLONG;
		}

		// Adquiero el lock
		LG->fl.l_type = F_WRLCK;
		fcntl(LG->fd, F_SETLKW, &(LG->fl));

		// Me muevo al final del archivo y escribo
		lseek(LG->fd, 0, SEEK_END);
		if (ERROR_LOG_WRITE == write(LG->fd, buffer, strlen(buffer))) {
			return_value = ERROR_LOG_WRITE;
		}

		// Libero el lock
		LG->fl.l_type = F_UNLCK;
		fcntl(LG->fd, F_SETLKW, &(LG->fl));
	}

	return return_value;
}

int Log_cerrar(Log *LG) {
	if (LG->debug)
		return close(LG->fd);
	return LOG_OK;
}
