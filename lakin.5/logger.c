/***
	-2 ways to log
		-verbose
		-nonverbose
	-note when resurce granted/released
	-time when resource isn't granted
	-output table after every 20 resources granted

	***/

#include "logger.h"

void logger(const char * string_buf) {
    int fid;
    fid = open(LOG_FILENAME, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (fid == -1) {
        snprintf(perror_buf, sizeof(perror_buf), "%s: open: ", perror_arg2);
        perror(perror_buf);
    } else {
        //printf("%s", string_buf);
        write(fid, (void *) string_buf, strlen(string_buf));
        close(fid);
    }
}
