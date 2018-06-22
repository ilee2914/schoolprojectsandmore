#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int main () {
	struct passwd * temp_pw;
	struct group * temp_gr;
	gid_t temp_gid;
	for (int i=0; i <= INT_MAX; i++) {
		temp_pw = getpwuid (i);
		if (temp_pw != NULL) {
			temp_gid = temp_pw->pw_gid;
			temp_gr = getgrgid (temp_gid);
			printf ("%*i - ", 5, temp_gid);
			printf ("%*s\n", -10, temp_gr->gr_name);
		}
	}
	return 0;
}