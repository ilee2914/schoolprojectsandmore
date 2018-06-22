#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

int main () {
	struct passwd * temp_pw;
	for (int i=0; i <= INT_MAX; i++) {
		temp_pw = getpwuid (i);
		if (temp_pw != NULL) {
			printf ("%*i - ", 5, temp_pw->pw_uid);
			printf ("%*s\n", -10, temp_pw->pw_name);
		}
	}
	return 0;
}